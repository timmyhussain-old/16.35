#include <stdio.h>
#include <stdlib.h>
#include "robot.h"

//robot * create_robot(int ix, cv_data * data) {
robot * create_robot(int ix, pthread_cond_t * cond) {
  robot * rob = malloc(sizeof(robot));

  rob->ix = ix;
  // rob->data = data;
  rob->robot_cond = cond;
  rob->run = &run;
  pthread_mutex_init(&rob->robot_lock, NULL);
  pthread_cond_init(rob->robot_cond, NULL);
  return rob;
}

void run(struct t_robot * robot) {

  printf("Robot index: %d ---- waiting for signal\n", robot->ix);
  pthread_mutex_lock(&robot->robot_lock);
  // pthread_cond_wait(robot->data.robot_cond, &robot->robot_lock);
  // pthread_cond_wait(robot->robot_cond, &robot->robot_lock);
  // pthread_cond_signal(robot->robot_cond);
  pthread_mutex_unlock(&robot->robot_lock);

  printf("Robot index: %d\n", robot->ix);
  FILE *command;

  char inputFilename[] = "command.list";
  char motor[2];
  int angle;

  command = fopen(inputFilename, "r");

  while (fscanf(command, "%s %d", motor, &angle) == 2) {
    printf("Motor: %s\n", motor);
    printf("Angle: %d\n", angle);
/*
    switch (motor[0]) {
      case 'w':
        printf("Motor: %s\n", motor);
        printf("Angle: %d\n", angle);
        break;
      case 'z':
        printf("Motor: %s\n", motor);
        printf("Angle: %d\n", angle);
        break;
      default:
        break;
    }
*/
  }

  fclose(command);
}
