#include <stdio.h>

int main(int argc, char** argv) {
  printf("int: %ld\n", sizeof(int));
  printf("float: %ld\n", sizeof(float));
  printf("double: %ld\n", sizeof(double));
  printf("char: %ld\n", sizeof(char));

  return 0;
}
