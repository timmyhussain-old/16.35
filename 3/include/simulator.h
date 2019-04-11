#ifndef __SIMULATOR_H__
#define __SIMULATOR_H__
#include "controller.h"
#include "vehicle.h"
#include <pthread.h>
typedef struct t_simulator{
    // synchronization variables here
    // ADD YOUR CODE HERE
    pthread_cond_t cond1;
    pthread_cond_t cond2;
    pthread_mutex_t lock;
    // don't change these
    int       vehicles_updated; // condition is vehicles_updated == n_vehicles.
    // simulator variables
    int       n_vehicles;
    vehicle * vehicles;
    double    current_time;
    double    max_time;
    double    time_increment;
    double    vehicle_update_rate;
    double    print_update_rate;
    // simulator run function
    void    (*run)(struct t_simulator*);
} simulator;

simulator * create_simulator();
void run(struct t_simulator * sim);
void cleanup_simulator(struct t_simulator * sim);
#endif
