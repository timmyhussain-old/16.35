#include "vehicle.h"
#include "controller.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

vehicle * create_vehicle(double * starting_position, int num_waypoints, double ** offset_waypoints) {
//    srand(time(0))

    //Allocating memory
    vehicle * new_vehicle = malloc(sizeof(vehicle));
    /* Randomize initial position and heading
    double a = 2*M_PI;
    double x = (double) (rand()%99);
    double y = (double) (rand() % 99);
    double theta = 0.0; //((double)rand()/(double)(RAND_MAX)) * a - M_PI;
    double values[3] = {x, y, theta};
     */

    //defining position
    for (int i = 0; i < 3; i++) {
        new_vehicle->position[i] = starting_position[i];
        //new line
    }
    //other definitions
    new_vehicle->num_waypoints = num_waypoints;
    new_vehicle->target_waypoints = malloc((num_waypoints) * 2 * sizeof(double));
    new_vehicle->target_waypoints[0] = malloc(2*sizeof(double));
    new_vehicle->target_waypoints[0][0] = starting_position[0] + offset_waypoints[0][0];
    new_vehicle->target_waypoints[0][1] = starting_position[1] + offset_waypoints[0][1];
    // printf("tw x: %f\n", new_vehicle->target_waypoints[0][0]);
    // printf("tw y: %f\n\n", new_vehicle->target_waypoints[0][1]);
    for (int i = 0; i < num_waypoints; i++) {
        new_vehicle->target_waypoints[i] = malloc(2 * sizeof(double));
        new_vehicle->target_waypoints[i][0] = new_vehicle->position[0] + offset_waypoints[i][0];//target_waypoints[i-1][0] + offset_waypoints[i][0];
        new_vehicle->target_waypoints[i][1] = new_vehicle->position[1] + offset_waypoints[i][1];//target_waypoints[i-1][1] + offset_waypoints[i][1];
        // printf("tw x: %f\n", new_vehicle->target_waypoints[i][0]);
        // printf("tw y: %f\n\n", new_vehicle->target_waypoints[i][1]);
    }
    /* testing
    for (int i = 0; i < num_waypoints+1; i++) {
      printf("ow x: %f\n", offset_waypoints[i][0]);
      printf("ow y: %f\n\n", offset_waypoints[i][1]);
    }
    */
    //function definitions
    new_vehicle->current_waypoint = (new_vehicle->target_waypoints[0]);
    new_vehicle->current_waypoint_idx = 0;
    new_vehicle->set_position = set_position;
    new_vehicle->set_velocity = set_velocity;
    new_vehicle->control_vehicle = control_vehicle;
    new_vehicle->update_state = update_state;

    return new_vehicle;
}

double max (double a, double b) {
	return (a < b) ? b : a;
}

void set_position   (struct t_vehicle * v, double * values) {
	for (int i=0; i < 3; i++) {
		// v->position[i] = *(values+i);
		switch (i) {
			case 2: //check angle first because x, y have same constraints
				if ((*(values + i) < M_PI) && (*(values + i) > - M_PI)) {
					v->position[i] = *(values+i);
				}
				else if (*(values + i) >= M_PI) {
					v->position[i] = -(2*M_PI - *(values+i));
				}
        else if (*(values + i) < -M_PI) {
          v->position[i] = 2 * M_PI + *(values+i);
        }
				break;
			default:
				if ((*(values + i) < 100.0) && (*(values + i) > 0)) {
					v->position[i] = *(values+i);
				}
				else if (*(values + i) > (double) 100.0) {
					v->position[i] = (double) 100.0; //*(values+i) - (double) 100.0;
				}
        else if (*(values + i) < 0.0) {
          v->position[i] = (double) 0.0; //(double) 100.0 + *(values + i);
         }
				break;
		}
	}
}

void set_velocity   (struct t_vehicle * v,double * values) {
	for (int i=0; i < 3; i++) {
		//v->velocity[i] = *(values+i);
		switch (i) {
			case 2: //check angular velocity first because x_dot, y_dot have same constraints
				if (*(values + i) < M_PI_4) {
					v->velocity[i] = max(-M_PI_4, *(values+i));
				}
				else {
					v->velocity[i] = M_PI_4;
				}
				break;
			default:
				if (*(values + i) < 10.0) {
					v->velocity[i] = *(values+i); //max((double) 5.0, *(values+i));
				}
				else {
					v->velocity[i] = (double) 10.0;
				}
        break;
		}
	}
}

void control_vehicle(struct t_vehicle * v) {
	control control_obj = get_proportional_waypoint_control(v);
	double theta = v->position[2]; //double theta = control_obj.angular_velocity;
  // printf("speed: %f\n", control_obj.speed);
  // printf("theta: %f\n", theta);
	double x_dot = control_obj.speed * cos(theta);
	double y_dot = control_obj.speed * sin(theta);
	double values[3] = {x_dot, y_dot, control_obj.angular_velocity};
	v->set_velocity(v, values);
}

void update_state   (struct t_vehicle * v, double time) {
	double x = v->position[0] + v->velocity[0]*time;
	double y = v->position[1] + v->velocity[1]*time;
	double theta = v->position[2] + v->velocity[2]*time;
	double values[3] = {x, y, theta};
	v->set_position(v, values);
}
