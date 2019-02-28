#include <stdio.h>
#include <string.h>
#include <math.h>

int max(int a, int b);
int check_palin(int argc, char** argv);

int main (int argc, char *argv[]) {

  int bool;
  char *string;
  string = strcpy(string, *argv);
  bool = check_palin(argc, (char **) string);

for (size_t i = 0; i < argc; i++) {
  printf("%s\n", (argv+1)[i]);
}
  // if (bool == 0) {
  //   printf("\"%s\" is a palindrome!\n", *(argv+1));
  // }
  // else {
  //   printf("\"%s\" is NOT a palindrome!\n", *(argv+1));
  // }

  printf("Boolean: %d\n", bool);
}

int max(int a, int b) {
  int ret = (a > b) ? a : b;
  return ret;
}


int check_palin(int argc, char** argv){
  int length = 0;
  for (int i = 1; i < argc; i++) {
    length += strlen(argv[i]);
  }

  int loop = 0;
  char *string;

  for (int i = 0; i < length; i++) {
    do {
      string[i] = *((*argv)+i)
    } while(*argv++ != '\0');
  }
  string = argv[1];
  for (int i = 2; i < argc; i++) {
    string = strcat(string, argv[i]);
  }

  char string_rev[length+1];

  for (int i = 0; i < length+1; i++) {
    string_rev[i] = (char) string[length-i];
  }
  int bool = strcmp(string, (string_rev+1));
}
  //
  // char *s[argc-2];
  // int arr[argc-1];
  // // char *new_s[argc];
  // // s = argv[-2];
  //
  //
  // for (int i = 0; i < argc-1; i++) {
  //   s[i] = argv[argc-i-1];
  //   printf("s: %s\n", s[i]);
  //   printf("argv: %s\n", argv[i+1]);
  //   // printf("argv: %s\n", argv[argc-i-1]);
  //
  //   int a = strcmp(s[i], argv[i+1]);
  //   printf("bool: %d\n", a);
  //
  //   if (a != 0) {
  //     printf("\"%s ", argv[1]);
  //     for (int i = 2; i < argc-1; i++) {
  //       printf("%s "  ,argv[i]);
  //     }
  //     printf("%s\" is NOT a palindrome!\n", argv[argc-1]);
  //     return 0;
  //   }
  //   // arr[i]  = a;
  // }
  // printf("\"%s ", argv[1]);
  // for (int i = 2; i < argc-1; i++) {
  //   printf("%s "  ,argv[i]);
  // }
  // printf("%s\" is a palindrome!\n", argv[argc-1]);
  // return 0;
  // //
  // // int arr_sum, loop;
  // //
  // // for (arr_sum = 0;  loop <= argc-1; loop++) {
  // //   arr_sum += arr[loop];
  // // }
  // //
  // // if (arr_sum == 0) {
  // //   printf("\"%s\" is a palindrome!\n", *s);
  // // }
  // // else {
  // //
  // // }
  // // int a = strcmp(*s, *argv);
  // // printf("bool: %d\n", a);



  // printf("original string: %s\n", s);
  // printf("string length: %d\n", argc);

//   return 0;
// }
