#include "controller.h"
#include "vehicle.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

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
    return c;
}

control get_follower_control(struct t_vehicle * vehicle) {
    // YOUR CODE HERE
    control c = {0,0};
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

follower_control_data * create_follower_control_data(vehicle * leader){
    // YOUR CODE HERE
    return NULL;
}
