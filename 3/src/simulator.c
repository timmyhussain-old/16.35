#include "simulator.h"
#include "vehicle.h"
#include <stdlib.h>
#include "client.h"
#include <math.h>
#include <unistd.h>
#include <stdio.h>
#include <stdbool.h>

// pthread_cond_t cond;
pthread_mutex_t lock;

simulator * create_simulator(){
    // initialization
    simulator * sim = malloc(sizeof(simulator));
    sim->run = &run;
    sim->n_vehicles = 0;
    // update rate for display server
    sim->vehicle_update_rate = 15; // hz
    sim->print_update_rate = 1;
    // simulator settings
    sim->max_time = 100.0;
    sim->current_time = 0.0;
    sim->time_increment = 0.01;
    sim->vehicles = NULL;
    // initialize your synchronization variables here!
    // YOUR CODE HERE
    pthread_cond_init(&sim->cond1, NULL);
    pthread_cond_init(&sim->cond2, NULL);
    pthread_mutex_init(&sim->lock, NULL);

    sim->vehicles_updated = 0;
    return sim;
}
void run(struct t_simulator * sim){
    open_server(IP,PORTNUM);
    sim->current_time = 0.0;
    double time_vehicle_message = 0.0;
    double time_print_update = 0.0;
    while (sim->current_time < sim->max_time) {
        // wait on condition variable here
        // YOUR CODE HERE
        pthread_mutex_lock(&sim->lock);
        if (sim->vehicles_updated != sim->n_vehicles) {
          // printf("Waiting for conditional variable\n");
          // printf("current number updated: %d\n", sim->vehicles_updated);
          pthread_cond_wait(&sim->cond2, &sim->lock);
        }
        else {

        // before waking the other threads, we update the display server if required and increment time
        time_vehicle_message += sim->time_increment;
        time_print_update += sim->time_increment;
        sim->current_time += sim->time_increment;
        sim->vehicles_updated = 0;
        if (time_vehicle_message > 1.0/sim->vehicle_update_rate) {
            send_vehicles(sim->n_vehicles,sim->vehicles);
            time_vehicle_message = 0.0;
        }
        if (time_print_update > 1.0/sim->print_update_rate) {
            printf("t = %.3f\n",sim->current_time);
            time_print_update = 0.0;
        }
        // we're past the condition (everyone is updated) so we can now broadcast to the waiting threads
        // YOUR CODE HERE

        // printf("about to broadcast (sim)\n");
        pthread_cond_broadcast(&sim->cond1);
        usleep(0.7*sim->time_increment*1e6); // sleep for roughly the time increment so we get quasi-realtime behavior
        }

        pthread_mutex_unlock(&sim->lock);
    }

    close_server();

}

void cleanup_simulator(struct t_simulator * sim){
    // YOUR CODE HERE
    pthread_cond_destroy(&sim->cond1);
    pthread_cond_destroy(&sim->cond2);
    pthread_mutex_destroy(&sim->lock);
    // cleanup the synchronization variables you created.
    for (int i = 0; i < sim->n_vehicles; i++) {
        if (sim->vehicles[i].control_data != NULL) free(sim->vehicles[i].control_data);
    }
    if (sim->vehicles != NULL) free(sim->vehicles);
}
