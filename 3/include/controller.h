#ifndef __CONTROLLER_H__
#define __CONTROLLER_H__
#include "vehicle.h"
// defines the control class
typedef struct t_control {
    double speed;
    double angular_velocity; // rad/s
} control;

typedef struct {
    double ** target_waypoints;  // List of x, y points to drive to.
    double * current_waypoint;  // The current x, y point you're aiming for.
    int num_waypoints;
    int current_waypoint_idx;
} waypoint_control_data;

typedef struct {
    vehicle * leader;
} follower_control_data;

// List of possible controller functions here that, given a vehicle, returns a control.
control get_proportional_waypoint_control(struct t_vehicle * vehicle);
control get_follower_control(struct t_vehicle * vehicle);

// Control data initialization
waypoint_control_data * create_waypoint_control_data(int n_waypoints, double * starting_position, double ** offset_waypoints);
follower_control_data * create_follower_control_data(vehicle * leader);

#endif