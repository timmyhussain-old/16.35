#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*function for getting total number of characters in string*/
int get_length(int argc, char **argv);

int main(int argc, char **argv) {
  /* argv is a pointer to an array of pointers*/

  int length = get_length(argc, argv);

  /*string will be the concatenated words as one string
  word is each word in the array*/
  char string[length];
  char* word;
  int a = 0; //random counter

  /* preserve original address of argv*/
  char **ptr= argv;

  /* start at index 1 (so we skip 'a.out')*/
  *argv++;

  /* cycle through the array and each subsequent word and append
   each letter to the string array*/
  while ((word = *argv++) != NULL) {
      for (size_t i = 0; i < strlen(word); i++) {
        string[a] = word[i];
        a++;
    }
  }

  string[length] = '\0'; //null byt for string termination

  /*this will be the reversed string which we will compare with string*/
  char string_rev[length];

  /*cycle through the both arrays in opposite directions and
  append letters to string_rev*/
  for (int i = 0; i < length; i++) {
    string_rev[i] = (char) string[length-i-1];
  }

  string_rev[length] = '\0'; //null byt for string termination

  /*compare the two strings and store the integer value as bool*/
  int bool = strcmp(string, string_rev);

  /*string printing gymnastics to get it into the correct form*/
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

return 0;
}

int get_length(int argc, char** argv) {
  int length = 0;
  for (int i = 1; i < argc; i++) {
    length += strlen(argv[i]);
  }
  return length;
}
