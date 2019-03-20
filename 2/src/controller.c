#include "controller.h"
#include "vehicle.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

double min(double a, double b) {
	return (a < b) ? a : b;
}

double max (double a, double b);

control get_proportional_waypoint_control(struct t_vehicle * vehicle) {
	double x = vehicle->position[0];
	double y = vehicle->position[1];
	
	if ((fabs(x-vehicle->current_waypoint[0]) < (double ) 3) && fabs(y-vehicle->current_waypoint[1]) < (double) 3) {
    vehicle->current_waypoint = vehicle->target_waypoints[vehicle->current_waypoint_idx];
    vehicle->current_waypoint_idx++;
  }
		// printf("x: %f\n", vehicle->current_waypoint[0]);
		// printf("y: %f\n", vehicle->current_waypoint[1]);
		control * new_control = malloc(sizeof(control));
    new_control->speed = 7;
    double theta = vehicle->position[2];
    // double alpha = atan((vehicle->current_waypoint[1]-y)/(vehicle->current_waypoint[0]-x));
		double alpha = atan2((vehicle->current_waypoint[1]-y), (vehicle->current_waypoint[0]-x));

    double ret = alpha - theta; //max(alpha - theta, theta-alpha);

		// if (vehicle->current_waypoint[0] > x) {
		// 	alpha = alpha;
		// }
		// else {
		//
		// }

		if ((ret < M_PI) && (ret > - M_PI)) {
			ret = ret;
		}
		else if (ret >= M_PI) {
			ret = -(2*M_PI - ret);
		}
		else if (ret < -M_PI) {
			ret = 2 * M_PI + ret;
		}

		if (ret < M_PI_4) {
			ret = max(-M_PI_4, ret);
		}
		else {
			ret = M_PI_4;
		}
		// double ret = theta - alpha;
		new_control->angular_velocity = ret;
		// new_control->angular_velocity = alpha-theta; //min(alpha-theta, )
    return *new_control;
}
