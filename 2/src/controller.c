#include "controller.h"
#include "vehicle.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

double min(double a, double b) {
	return (a < b) ? a : b;
}

control get_proportional_waypoint_control(struct t_vehicle * vehicle) {
	double x = vehicle->position[0];
	double y = vehicle->position[1];
	if ((fabs(x-vehicle->current_waypoint[0]) < (double )3.0) && fabs(y-vehicle->current_waypoint[1]) < (double) 3.0) {
    vehicle->current_waypoint = vehicle->target_waypoints[vehicle->current_waypoint_idx];
    vehicle->current_waypoint_idx++;
  }
		// printf("x: %f\n", vehicle->current_waypoint[0]);
		// printf("y: %f\n", vehicle->current_waypoint[1]);
		control * new_control = malloc(sizeof(control));
    new_control->speed = 10;
    double theta = vehicle->position[2];
    double alpha = atan((vehicle->current_waypoint[1]-y)/(vehicle->current_waypoint[0]-x));
		switch (alpha < 0) {
			case 0:
				alpha += M_PI_2;
				if (alpha - theta > M_PI) {
					alpha = -(2*M_PI - (alpha-theta));
				}
				else {
					alpha = alpha - theta;
				}
				break;
			case 1:
				alpha -= M_PI_2;
				if (alpha - theta < -M_PI) {
					alpha = 2*M_PI + (alpha-theta);
				}
				else {
					alpha = alpha - theta;
				}
				break;
		}

    //new_control->angular_velocity = min(alpha - theta, 2 * M_PI - (alpha - theta));
		new_control->angular_velocity = alpha; //min(alpha-theta, )
    return *new_control;
}
