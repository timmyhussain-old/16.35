#include <stdio.h>
#include <stdlib.h>

int main (int argc, char ** argv) {

  FILE * command;
  char outputFilename[] = "command.list";

  char motor[2];
  int angle;

  command = fopen(outputFilename, "w");

  fprintf(command, "%s %d\n", "w", 0000);
  fprintf(command, "%s %d\n", "w", 90);
  fprintf(command, "%s %d\n", "z", -120);

  fclose(command);

  return 0;
}
