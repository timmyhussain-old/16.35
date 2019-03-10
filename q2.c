#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char **argv) {

  double a, b;
  char *s;
  a = atof(argv[1]);
  b = atof(argv[3]);
  s = argv[2];

  // printf("%d\n", atoi(s));
  if (strcmp(s, "+") == 0)
    printf("%.3f", a + b);
  else if (strcmp(s, "-") == 0)
    printf("%.3f", a - b);
  else if (strcmp(s, "/") == 0)
    printf("%.3f", a / b);
  else if (strcmp(s, "x") == 0)
    printf("%.3f", a * b);
  else if (strcmp(s, "X") == 0)
    printf("%.3f", a * b);
  else
    printf("%.3f", a * b);

  return 0;
}
