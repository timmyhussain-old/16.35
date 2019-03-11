#include "vehicle.h"
#include "controller.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

vehicle * create_vehicle(double * starting_position, int num_waypoints, double ** offset_waypoints) {
    srand(time(0))
    new_vehicle = malloc(sizeof(vehicle));
    double a = 2*M_PI;
    double x = (double) (rand()%99);
    double y = (double) (rand() % 99);
    double theta = ((double)rand()/(double)(RAND_MAX)) * a - M_PI;
    double values[3] = {x, y, theta};
    for (int i = 0; i < 3; i++) {
        new_vehicle->position[i] = values[i];
        //new line
    }
}

double max (double a, double b) {
	return (a < b) ? b : a;
}

void set_position   (struct t_vehicle * v,double * values) {
	for (i=0; i < 3; i++) {
		//v->position[i] = *(values+i);
		switch (i) {
			case 2: //check angle first because x, y have same constraints
				if (*(values + i) < M_PI) {
					v->position[i] = max(-M_PI, *(values+i));
				}
				else {
					v-position[i] = M_PI;
				}
				break;
			default: 
				if (*(values + i) < 100.0) {
					v->position[i] = max((double) 0.0, *(values+i));
				}
				else {
					v-position[i] = (double) 100.0;
				}
				break;
		}
	}
}

void set_velocity   (struct t_vehicle * v,double * values) {
	for (i=0; i < 3; i++) {
		//v->velocity[i] = *(values+i);
		switch (i) {
			case 2: //check angular velocity first because x_dot, y_dot have same constraints
				if (*(values + i) < M_PI_4) {
					v->velocity[i] = max(-M_PI_4, *(values+i));
				}
				else {
					v-velocity[i] = M_PI_4;
				}
				break;
			default: 
				if (*(values + i) < 10.0) {
					v->velocity[i] = max((double) 5.0, *(values+i));
				}
				else {
					v-velocity[i] = (double) 10.0;
				}
                break;
		}
	}
}

void control_vehicle(struct t_vehicle * v) {
	control = get_proportional_waypoint_control(struct t_vehicle * v);
	double theta = v->position[2];
	double x_dot = control->speed * cos(theta);
	double y_dot = control->speed * sin(theta);
	double values[3] = {x_dot, y_dot, control->angular_velocity};
	v->set_velocity(v, values);
}

void update_state   (struct t_vehicle * v, double time) {
	double x = v->position[0] + v->velocity[0]*time;
	double y = v->position[1] + v->velocity[1]*time;
	double theta = v->position[2] + v->velocity[2]*time;
	double values[3] = {x, y, theta};
	v->set_position(v, values);
}

