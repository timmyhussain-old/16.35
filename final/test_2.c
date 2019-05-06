#include <stdio.h>
#include <stdlib.h>

int main (int argc, char ** argv) {

  FILE * command;
  char outputFilename[] = "command.list";

  char motor[2];
  int angle;

  command = fopen(outputFilename, "r");

  while (fscanf(command, "%s %d", motor, &angle) == 2) {
    switch (motor[0]) {
      case 'w':
        printf("This is w case!\n");
        printf("Angle is %d\n", angle);
        break;
      case 'z':
        printf("This is z case!\n");
        printf("Angle is %d\n", angle);
        break;
    }
  }

  return 0;
}
