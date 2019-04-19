#include "controller.h"
#include "vehicle.h"
#include "simulator.h"
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <math.h>

// pthread_mutex_t lock;

vehicle * create_vehicle(double * starting_position, struct t_simulator * sim){
    vehicle * v = malloc(sizeof(vehicle));
    v->simulator = sim;
    memcpy(v->position,starting_position,sizeof(double)*3);
    // assign function pointers to defaults
    v->set_position = &set_position;
    v->set_velocity = &set_velocity;
    v->control_vehicle = &control_vehicle;
    v->update_state = &update_state;
    v->run = &vehicle_run;
    v->control_data = NULL;
    pthread_mutex_init(&v->vehicle_lock, NULL);
    return v;
}
// standard vehicle methods
void set_position   (struct t_vehicle * v,double * values){
    // set the vehicle positions. assumes vehicle is properly initialized.
    pthread_mutex_lock(&v->vehicle_lock);
    memcpy(v->position,values,3*sizeof(double));
    if (v->position[0] > 100.0) v->position[0] = 100.0;
    if (v->position[1] > 100.0) v->position[1] = 100.0;
    if (v->position[2] > M_PI)  v->position[2] = M_PI;
    if (v->position[0] < 0.0)   v->position[0] = 0.0;
    if (v->position[1] < 0.0)   v->position[1] = 0.0;
    if (v->position[2] < -M_PI) v->position[2] = -M_PI;
    pthread_mutex_unlock(&v->vehicle_lock);
}
void set_velocity   (struct t_vehicle * v,double * values){
    memcpy(v->velocity,values,3*sizeof(double));
    // split velocities by components if they're exceeded.
    double heading = v->position[2];
    if (sqrt(pow(v->velocity[0],2)+pow(v->velocity[1],2)) > 10.0) {
        v->velocity[0] = 10*cos(heading);
        v->velocity[1] = 10*sin(heading);
    } else if (sqrt(pow(v->velocity[0],2)+pow(v->velocity[1],2)) < 5.0) {
        v->velocity[0] = 5*cos(heading);
        v->velocity[1] = 5*sin(heading);
    }
    if (v->velocity[2] > M_PI_4)  v->velocity[2] = M_PI_4;
    if (v->velocity[2] < -M_PI_4) v->velocity[2] = -M_PI_4;
}

void control_vehicle(struct t_vehicle * v){
    // First calculate what control to apply:
    control c = v->get_control(v);
    // apply controls to vehicle
    double velocities[3] = {c.speed*cos(v->position[2]),c.speed*sin(v->position[2]),c.angular_velocity};
    v->set_velocity(v,velocities);
}
void update_state   (struct t_vehicle * v, double time){
    // construct floating point representation of time
    double dx = v->velocity[0]; double dy = v->velocity[1]; double dtheta = v->velocity[2];
    double x = v->position[0]; double y = v->position[1]; double theta = v->position[2];
    // update positions
    x += dx*time; y+= dy*time; theta += dtheta*time;
    // clamp theta
    if (theta > M_PI) theta -= 2.0*M_PI;
    if (theta < -M_PI) theta += 2.0*M_PI;
    // update position
    double pos [3] = {x,y,theta};
    v->set_position(v,pos);
}

void vehicle_run (struct t_vehicle * v){
    while (v->simulator->current_time < v->simulator->max_time) {

        v->control_vehicle(v);
        v->update_state(v, v->simulator->time_increment);
        pthread_mutex_lock(&v->simulator->lock);
        //Critical Region
        v->simulator->vehicles_updated++;
        // printf("in thread, current number updated: %d\n", v->simulator->vehicles_updated);
        pthread_cond_broadcast(&v->simulator->cond2);
        pthread_cond_wait(&v->simulator->cond1, &v->simulator->lock);
        pthread_mutex_unlock(&v->simulator->lock);

    }
}
