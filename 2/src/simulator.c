#include "simulator.h"
#include "vehicle.h"
#include <stdlib.h>
#include "client.h"
#include <math.h>
#include <unistd.h>
#include <stdio.h>
#include <time.h>
#include <pthread.h>

// void run_threaded(struct t_simulator * sim);

simulator * create_simulator(){
    // initialization
    simulator * sim = malloc(sizeof(simulator));
    // sim->run = &run_threaded;

    // sim->run_threaded = &run_threaded;
    sim->n_vehicles = 0;
    // update rate for display server
    sim->vehicle_update_rate = 25; // hz
    // Create "offset waypoints", a series of waypoints for the vehicles to follow.
    // Note that these waypoints are for the *relative* waypoints relative to the
    // starting point of the vehicle.
    sim->num_waypoints = 5;
    sim->radius = 30;
    sim->offset_waypoints = malloc((sim->num_waypoints + 1) * 2 * sizeof(double));
    for (int i = 0; i < sim->num_waypoints + 1; i++) {
        sim->offset_waypoints[i] = malloc(2 * sizeof(double));
        sim->offset_waypoints[i][0] = sim->radius * cos( i * 2 * M_PI / (sim->num_waypoints));
        sim->offset_waypoints[i][1] = sim->radius * sin( i * 2 * M_PI / (sim->num_waypoints));
    }
    // simulator settings
    sim->max_time = 20.0;
    sim->current_time = 0.0;
    sim->time_increment = 0.01;
    sim->set_run = &set_run;
    return sim;
}

void run(struct t_simulator * sim){
    printf("normal run method");
    open_server(IP,PORTNUM);
    printf("normal run method");
    sim->current_time = 0.0;
    double time_vehicle_message = 0.0;
    int ix = 0;
    double avg_time = 0;
    while (sim->current_time < sim->max_time) {
      clock_t t;
      t = clock();
        printf("\rt = %f",sim->current_time);
        time_vehicle_message += sim->time_increment;
        sim->current_time += sim->time_increment;
        if (time_vehicle_message > 1.0/sim->vehicle_update_rate) {
            send_vehicles(sim->n_vehicles,sim->vehicles);
            time_vehicle_message = 0.0;
        }
        for (vehicle * v = sim->vehicles; v < sim->vehicles + sim->n_vehicles; v++){
            v->control_vehicle(v);
            v->update_state(v,sim->time_increment); // delta t
        }
        usleep(sim->time_increment*1e6); // sleep for roughly the time increment so we get quasi-realtime behavior
        t = clock() - t;
        double time_taken = ((double)t)/CLOCKS_PER_SEC;
        avg_time = ((avg_time * ix) + time_taken)/(ix+1);
        ix++;
        printf("Average time taken: %f\n\n", avg_time);
    }
    close_server();
}

typedef struct v_time{
  vehicle * vehicle;
  double timestep;
} v_time;

void * entry_function(void * v_time_ptr) {

  v_time * vptr = (v_time *) v_time_ptr;
  // printf("%f", vptr->timestep);

  (vptr->vehicle)->control_vehicle(vptr->vehicle);

  (vptr->vehicle)->update_state(vptr->vehicle, (double) vptr->timestep);

  // printf("end of entry function\n");
}

 void run_threaded(struct t_simulator * sim) {
  open_server(IP,PORTNUM);
  sim->current_time = 0.0;
  double time_vehicle_message = 0.0;
  int ix = 0;
  double avg_time = 0;
  while (sim->current_time < sim->max_time) {
      clock_t t;
      t = clock();
      printf("\rt = %f",sim->current_time);
      time_vehicle_message += sim->time_increment;
      sim->current_time += sim->time_increment;
      if (time_vehicle_message > 1.0/sim->vehicle_update_rate) {
          send_vehicles(sim->n_vehicles,sim->vehicles);
          time_vehicle_message = 0.0;
      }

      // printf("this is a test\n\n");

      pthread_t thr[sim->n_vehicles];
      int i = 0;


      for (vehicle * v = sim->vehicles; v < sim->vehicles + sim->n_vehicles; v++){
        v_time * vtime = malloc(sizeof(v_time*));
        vtime->vehicle = v;
        vtime->timestep = sim->time_increment;
        printf("%f\n", vtime->timestep);
        printf("before pthread_create\n");
        if (pthread_create(&thr[i++], NULL, &entry_function, vtime)) {
          fprintf(stderr, ": error creating thread\n");
        }
        // free(vtime);
      }
      i = 0;
      while (i < sim->n_vehicles) {
      printf("before pthread_join\n");
        pthread_join(thr[i++], NULL);
      }
      // pthread_join()

      usleep(sim->time_increment*1e6); // sleep for roughly the time increment so we get quasi-realtime behavior
      t = clock() - t;
      double time_taken = ((double)t)/CLOCKS_PER_SEC;
      avg_time = ((avg_time * ix) + time_taken)/(ix+1);
      ix++;
      printf("Average time taken: %f\n\n", avg_time);
  }
  close_server();
}

void set_run(struct t_simulator * sim, int i) {
  printf("in set_run\n");
  if (i == 0) {
    sim->run = &run;
  }
  else {
    sim->run = &run_threaded;
  }
}
