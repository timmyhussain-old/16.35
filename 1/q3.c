#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int get_length(int argc, char **argv);

int main(int argc, char **argv) {
  /* argv is a pointer to an array of pointers*/

  int length = get_length(argc, argv);

  char string[length];
  char* word;
  int a = 0;

  /* preserve original address of argv*/
  char **ptr= argv;

  /* start at index 1 (so we skip 'a.out')*/
  *argv++;

  while ((word = *argv++) != NULL) {
      for (size_t i = 0; i < strlen(word); i++) {
        string[a] = word[i];
        // printf("%c\n", word[i]);
        a++;
    }
  }

  string[length] = '\0';
  // printf("%s\n", (char *) string);

  char string_rev[length];

  for (int i = 0; i < length; i++) {
    string_rev[i] = (char) string[length-i-1];
  }

  string_rev[length] = '\0';
  int bool = strcmp(string, string_rev);

  // if (bool == 0) {
  //   printf("\"%s\" is a palindrome!\n",str);
  // }
  // else {
  //   printf("\"%s\" is a palindrome!\n",str);
  // }

  if (bool == 0) {
    *ptr++;
    printf("\"%s", *ptr++);
    while (*ptr != NULL) {
      printf(" %s", *ptr++);
    }
    printf("\" is a palindrome!\n");
  }
  else {
    *ptr++;
    printf("\"%s", *ptr++);
    while (*ptr != NULL) {
      printf(" %s", *ptr++);
    }
    printf("\" is NOT a palindrome!\n");
  }

}

int get_length(int argc, char** argv) {
  int length = 0;
  for (int i = 1; i < argc; i++) {
    length += strlen(argv[i]);
  }
  return length;
}
