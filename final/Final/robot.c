#include <stdio.h>
#include <stdlib.h>
#include "robot.h"

robot * create_robot(int ix) {
  robot *
}

void run() {

  
  FILE *command;

  char inputFilename[] = "command.list";
  char motor[2];
  int angle;

  command = fopen(inputFilename, "w");

  while (fscanf(command, "%s %d", motor, &angle) == 2) {
    switch (motor) {
      case 'w':
        PWM_4_SetCompare0(get_compare(angle));
        PWM_4_Start();
    }
  }
}
