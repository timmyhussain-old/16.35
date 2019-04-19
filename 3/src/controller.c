#include "controller.h"
#include "vehicle.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "simulator.h"

// Helper function.
double get_angle_to_point(struct t_vehicle * vehicle, double targ_x, double targ_y) {
    double angle_to_point = atan2((targ_y - vehicle->position[1]), (targ_x - vehicle->position[0]));
    double angle_error = angle_to_point -  vehicle->position[2];
    if (angle_error > M_PI) {
        angle_error = angle_error - 2 * M_PI;
    } else if (angle_error < -1 * M_PI) {
        angle_error = 2 * M_PI + angle_error;
    }
    return angle_error;
}

// Defines coord type struct
typedef struct t_coord {
    double x;
    double y;
} coord;


//Calculates distance between two coord objects
double distance(coord a, coord b) {
    double dist;
    dist = sqrt(pow((a.x - b.x), 2.0) + pow((a.y - b.y), 2));
    return dist;
}

double variable_speed(double distance) {
  double speed = 5.0 + 0.5*distance;
  return speed;
}

// Not used
void assign_target (vehicle * vehicle) {
  double shortest_distance = 1000;
  int new_target_ix;
  coord target_pos;
  coord vehicle_pos;
  vehicle_pos.x = vehicle->position[0];
  vehicle_pos.y = vehicle->position[1];


  for (int i = vehicle->ix+1; i <= vehicle->simulator->n_vehicles; i++) {
    target_pos.x = vehicle->simulator->vehicles[i].position[0];
    target_pos.y = vehicle->simulator->vehicles[i].position[1];

    if (distance(vehicle_pos, target_pos) < shortest_distance) {
      shortest_distance = distance(vehicle_pos, target_pos);
      new_target_ix = i;
    }
  }
  vehicle->control_data = create_tag_control_data(&vehicle->simulator->vehicles[new_target_ix]);
}

control get_proportional_waypoint_control(struct t_vehicle * vehicle) {
    waypoint_control_data * data = (waypoint_control_data *) vehicle->control_data;
    double * current_waypoint = data->current_waypoint;
    double angle_control_input = get_angle_to_point(vehicle, current_waypoint[0], current_waypoint[1]);
    // Cap the max angular velocity.
    if (angle_control_input > M_PI_4) {
        angle_control_input = M_PI_4;
    } else if (angle_control_input < -1 * M_PI_4) {
        angle_control_input = -1 * M_PI_4;
    }
    control c = {5.0, angle_control_input};
    // update waypoint logic
    double x_error = data->current_waypoint[0] - vehicle->position[0];
    double y_error = data->current_waypoint[1] - vehicle->position[1];
    double waypoint_proximity_threshold = 0.1;
    if (sqrt(x_error * x_error + y_error * y_error) < waypoint_proximity_threshold) {
        int next_waypoint_idx = data->current_waypoint_idx + 1;
        if (next_waypoint_idx == data->num_waypoints) { // Last waypoint. Aim for first again.
            next_waypoint_idx = 0;
        }
        data->current_waypoint_idx = next_waypoint_idx;
        data->current_waypoint = data->target_waypoints[next_waypoint_idx];
    }
    // c.speed = 0.0;
    // c.angular_velocity = 0.0;
    return c;
}

control get_follower_control(struct t_vehicle * vehicle) {
    // YOUR CODE HERE
    // vehicle
    follower_control_data * data = vehicle->control_data;

    // access position for leader
    coord leader_pos;
    pthread_mutex_lock(&data->leader->vehicle_lock);
    leader_pos.x = data->leader->position[0];
    leader_pos.y = data->leader->position[1];
    pthread_mutex_unlock(&data->leader->vehicle_lock);


    //Calculate angle input
    double angle_control_input = get_angle_to_point(vehicle, leader_pos.x, leader_pos.y);

    if (angle_control_input > M_PI_4) {
        angle_control_input = M_PI_4;
    } else if (angle_control_input < -1 * M_PI_4) {
        angle_control_input = -1 * M_PI_4;
    }

    control c = {5.0,angle_control_input};
    return c;
}

//Tag/Zombie controller (NOT USED BUT FUNCTIONAL)
control get_tag_control(struct t_vehicle * vehicle) {
  tag_control_data * data = vehicle->control_data;

  coord target_pos;
  //access position of target
  pthread_mutex_lock(&data->target->vehicle_lock);
  target_pos.x = data->target->position[0];
  target_pos.y = data->target->position[1];
  pthread_mutex_unlock(&data->target->vehicle_lock);

  //get angle control
  double angle_control_input = get_angle_to_point(vehicle, target_pos.x, target_pos.y);

  if (angle_control_input > M_PI_4) {
      angle_control_input = M_PI_4;
  } else if (angle_control_input < -1 * M_PI_4) {
      angle_control_input = -1 * M_PI_4;
  }

  //get vehicle position
  coord vehicle_pos;
  vehicle_pos.x = vehicle->position[0];
  vehicle_pos.y = vehicle->position[1];
  double speed = variable_speed(distance(target_pos, vehicle_pos));

  //if distance close enough; corrupt target and set new target for both vehicles
  if (distance(target_pos, vehicle_pos) < 1) {

      pthread_mutex_lock(&data->target->simulator->lock);
      data->target->get_control = get_tag_control;
      data->target->control_data = create_tag_control_data(&data->target->simulator->vehicles[data->target->ix + 1]);
      pthread_mutex_unlock(&data->target->simulator->lock);

      vehicle->control_data = create_tag_control_data(&vehicle->simulator->vehicles[data->target->ix + 1]);
  }

  //assign_target(vehicle);
  // vehicle->control_data = create_tag_control_data(&vehicle->simulator->vehicles[new_target_ix]);

  control c = {speed, angle_control_input};
  return c;
}

//Obstacle avoid (ACTIVE CONTROLLER FOR OFF ON YOUR OWN)
control get_obstacle_avoid_control(struct t_vehicle * vehicle) {

  control c;
  double time = vehicle->simulator->time_increment;
  coord target_pos;
  coord vehicle_pos;
  double speed = 5.0;
  //Create vehicle coord object
  vehicle_pos.x = vehicle->position[0];
  vehicle_pos.y = vehicle->position[1];

  //loop through all vehicles
  for (int i = 0; i < vehicle->simulator->n_vehicles; i++) {

    //access position and velocity for all vehicles
    pthread_mutex_lock(&vehicle->simulator->vehicles[i].vehicle_lock);

    double dx = vehicle->simulator->vehicles[i].velocity[0]; double dy = vehicle->simulator->vehicles[i].velocity[1];
    double dtheta = vehicle->simulator->vehicles[i].velocity[2];
    double x = vehicle->simulator->vehicles[i].position[0]; double y = vehicle->simulator->vehicles[i].position[1];
    double theta = vehicle->simulator->vehicles[i].position[2];
    // double heading = data->target->position[2];
    pthread_mutex_unlock(&vehicle->simulator->vehicles[i].vehicle_lock);

    //create coord object for target position
    target_pos.x = x;
    target_pos.y = y;

    //if target is too close, avoid
    if ((distance(vehicle_pos, target_pos) > 0) && (distance(vehicle_pos, target_pos) < 5)) {
      //define projected location (where closest vehicle is going to be at next timestep)
      coord projected_location;
      projected_location.x = x+dx*time;
      projected_location.y = y+dy*time;

      coord projected_vehicle_location;
      // update positions
      // x += dx*time; y+= dy*time; theta += dtheta*time;
      double angular_control;
      double best_distance = distance(vehicle_pos, target_pos);
      int best_factor;
      //choose angle that maximizes distance between current location and projected location for the target vehicle
      for (int i = 1; i <= 8; i++) {
        angular_control = -M_PI_4 + i*M_PI_4/4;

        double velocities[3] = {speed*cos(vehicle->position[2] + angular_control*time),speed*sin(vehicle->position[2]),angular_control};

        double dx_v = velocities[0]; double dy_v = velocities[1];
        //double dtheta_v = velocities[2]*time;
        double x_v = vehicle->position[0]; double y_v = vehicle->position[1]; //double theta_v = vehicle->position[2];
        // update positions
        projected_vehicle_location.x = x + dx_v*time; projected_vehicle_location.y = y+ dy_v*time; // theta += dtheta*time;
        if (distance(projected_location, projected_vehicle_location) > best_distance) {
          best_distance = distance(projected_location, projected_vehicle_location);
          best_factor = i;
        }
      }
      //returns this speed and best angle
      c.speed = speed;
      c.angular_velocity = -M_PI_4 + best_factor*M_PI_4/2;
      return c;
    }


  }
  //returns standard control input (no change) if no obstacle avoid needed
  c.speed = 5.0;
  c.angular_velocity = 0.0;
  return c;
}

// COULDN'T QUITE GET THIS TO DO WHAT I WANTED; IGNORE
control get_orbit_control(struct t_vehicle * vehicle) {
/*
    orbit_control_data * data = vehicle->control_data;

    coord leader_pos;
    pthread_mutex_lock(&data->leader->vehicle_lock);
    leader_pos.x = data->leader->position[0];
    leader_pos.y = data->leader->position[1];
    pthread_mutex_unlock(&data->leader->vehicle_lock);

    coord target_location;
    target_location.x = leader_pos.x + data->radius*cos(data->theta);
    target_location.y = leader_pos.y + data->radius*sin(data->theta);

    printf("target x: %f\ntarget y: %f\n", target_location.x, target_location.y);

    double angle_control_input = get_angle_to_point(vehicle, target_location.x, target_location.y);


    if (angle_control_input > M_PI_4) {
        angle_control_input = M_PI_4;
        // speed = 0.0;
    } else if (angle_control_input < -1 * M_PI_4) {
        angle_control_input = -1 * M_PI_4;
        // speed = 0.0;
    }

    coord vehicle_pos;
    vehicle_pos.x = vehicle->position[0];
    vehicle_pos.y = vehicle->position[1];
    // double speed = variable_speed(distance(leader_pos, vehicle_pos));


    printf("current x: %f\ncurrent y: %f\n", vehicle_pos.x, vehicle_pos.y);
    // double dist = distance(target_location, vehicle_pos);
    // printf("distance to target: %f\n\n", dist);
    if (distance(target_location, vehicle_pos) < 0.5) {
        data->theta = fmod(data->theta + M_PI_4/2, 2*M_PI);
    }

*/
    // control c = {5.0, angle_control_input};
    control c = {0.0, 0.0};
    return c;
}

waypoint_control_data * create_waypoint_control_data(int n_waypoints, double * starting_position, double ** offset_waypoints){
    waypoint_control_data * data = malloc(sizeof(waypoint_control_data));
    data->target_waypoints = malloc(n_waypoints * 3 * sizeof(double));
    for (int i = 0; i < n_waypoints; i++) {
        data->target_waypoints[i] = malloc(3 * sizeof(double));
        data->target_waypoints[i][0] = starting_position[0] + offset_waypoints[i][0];
        data->target_waypoints[i][1] = starting_position[1] + offset_waypoints[i][1];
    }
    data->num_waypoints = n_waypoints;
    data->current_waypoint = data->target_waypoints[0];
    return data;
}

follower_control_data * create_follower_control_data(struct t_vehicle * leader) {
    // YOUR CODE HERE
    follower_control_data * data = malloc(sizeof(follower_control_data));
    data->leader = leader;
    return data;
    // return NULL;
}

orbit_control_data * create_orbit_control_data(struct t_vehicle * leader, double radius, double theta) {
    orbit_control_data  * data = malloc(sizeof(orbit_control_data));
    data->leader = leader;
    data->radius = radius;
    data->theta = theta;
    return data;
}

tag_control_data * create_tag_control_data(struct t_vehicle * target) {
  tag_control_data * data = malloc(sizeof(tag_control_data));
  data->target = target;
  return data;
}
