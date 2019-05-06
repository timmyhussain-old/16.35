#include <stdio.h>
#include <stdlib.h>
#include "robot.h"
#include <pthread.h>
//#include "controller.h"
//include pthread library

  //create 3 thread_ids for the robots
  //create robot_structs

//create threads
/*
char c = getchar();
char word[5];
switch (c) {
  case 'w':
    //write forward command to FILE
    //backrobot_semaphore wake
  case 's':
    //write backward command to FILE
    //backrobot_semaphore wake
  case 'a':
    //write left turn command to FILE
    //frontrobot_semaphore wake
  case 'd':
    //write right turn command to FILE
    //frontrobot_semaphore wake

}
*/

cv_data * create_cv_data(pthread_cond_t * cond) {
  cv_data * data = malloc(sizeof(cv_data));
  data->robot_cond = cond;
  return data;
}

void * entry_function(void * robo) {

  robot * rob = (robot*) robo;
  printf("run\n");
  // printf("robot index: %d\n", rob->ix);
  rob->run(rob);
}

int main (int * argc, char ** argv) {

  FILE *command;

  char outputFilename[] = "command.list";
  char motor[2];
  int angle;

  command = fopen(outputFilename, "w");

  fprintf(command, "%s %d", "w", 0000);
  fprintf(command, "%s %d", "w", 90);

  fclose(command);

  cv_data * data = malloc(sizeof(cv_data));
  pthread_cond_t cond1; //front robot conditional variable
  pthread_cond_t cond2; //back robots conditional variable
  pthread_cond_init(&cond1, NULL);
  pthread_cond_init(&cond2, NULL);
  // data.frontrobot_cond = &cond1;
  // data.backrobot_cond = &cond2;

  int n_robots = 3;
  pthread_t thread_ids[n_robots];

  for (int i = 0; i < n_robots; i++) {

    struct t_robot * robot;
    switch (i) {
      case 0:
        robot = create_robot(i, &cond1);
        break;
      default:
        robot = create_robot(i, &cond2);
        break;
    }


    // if (i == 0) {
    //   data = create_cv_data(&cond1);
    // }
    // else {
    //   data = create_cv_data(&cond2);
    // }
    //
    // struct t_robot * robot = create_robot(i, data);
    // printf("Robot index: %d\n", robot->ix);
    if (pthread_create(&thread_ids[i], NULL, &entry_function, (void*) robot)) {
      fprintf(stderr, ": error creating thread\n");
    }
    // printf("pthread_create\n");
    // free(robot);
    // free(data);
  }

  char c = getchar();

  switch (c) {
    case 'w':
      printf("inside case w\n");
      pthread_cond_broadcast(&cond1);
      break;
    case 'a':
      printf("inside case a\n");
      pthread_cond_broadcast(&cond2);
      break;
  }

// printf("before pthreadjoin\n");

for (int i = 0; i < n_robots; i++) pthread_join(thread_ids[i],NULL);

  return 0;
}
