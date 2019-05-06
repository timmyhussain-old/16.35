#include <stdio.h>
#include <stdlib.h>
#include "robot.h"
#include "controller.h"
//include pthread library



// void entry_function(void *ptr) {
//
// }

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

int main (int * argv, char ** argv) {

  FILE *command;

  char outputFilename[] = "command.list";
  char motor[2];
  int angle;

  command = fopen(outputFilename, "w");

  fprintf(command, "%s %d", 'w', 0000);
  fprintf(command, "%s %d", 'w', 90);

  fclose(command);

  // FILE * fp;
  // fp = fopen("")
  // return 0;
}
