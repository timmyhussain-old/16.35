#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Node structure
typedef struct Node {
struct Node * upper;
struct Node * lower;
char * item;
} node;

void join(node *lower, node *upper) {
  lower->upper = upper;
  upper->lower = lower;
}

void insert(node * root, char * str) {

  /*base case*/
  if (strcmp(root->item, "") == 0) {
    root->item = str;
    // printf("base case: %s\n", root->item);
  }
  else {
    int bool = strcmp(root->item, str);

    node * node_2 = malloc(sizeof(node));
    node_2->upper = NULL;
    node_2->lower = NULL;
    node_2->item = "";

    if (bool == 0) {
      node_2->item = str;
      if (root->upper == NULL) {
        join(root, node_2);
      }
      else {
        node * upper = root->upper;
        join(root, node_2);
        join(node_2, upper);
      }
      // free(node_2);
    }

    else if (bool > 0) {
      //str = abc --> upper; item = def
      if (root->upper == NULL) {
        node_2->item = str;
        join(root, node_2);
        // free(node_2);
      }
      else {
        node * upper = root->upper;
        if (strcmp(upper->item, str) < 0) {
          node_2->item = str;
          join(root, node_2);
          join(node_2, upper);
          // free(node_2);
        }
        else {
          insert(upper, str);
        }
      }
    }

    else {
      if (root->lower == NULL) {
        node_2->item = str;
        join(node_2, root);
      }
      else {
        node *lower = root->lower;
        if (strcmp(lower->item, str) > 0) {
          node_2->item = str;
          join(node_2, root);
          join(lower, node_2);
          // free(node_2);
        }
        else{
          insert(lower, str);
        }
      }
    }

  }

}
void print_tree(node * root) {
  //base case
// printf("root: %s\n", root->item);
// root = root->upper;
// printf("upper: %s\n", root->upper->item);

  node * test = root;
  while (test->upper != NULL) {
    test = test->upper;
  }
  // do {
  //   printf("%s\n", test->item);
  //   test = test->lower;
  // } while(test->lower != NULL);
  while (test->lower != NULL) {
    printf("%s ", test->item);
    test = test->lower;
  }
  printf("%s", test->item);
  // while (test->lower != NULL) {
  //   printf("%s\n", test->item);
  //   test = test->lower;
  // }
}

void free_tree(node * root) {
  if (root != NULL) {
    free_tree(root->upper);
    free_tree(root->lower);
    free(root);
  }
}
int main (int argc, char ** argv) {
  node * root = malloc(sizeof(node));

  root->upper = NULL;
  root->lower = NULL;
  root->item = "";

  // node * branch = malloc(sizeof(node));
  // branch->upper = NULL;
  // branch->lower = NULL;
  // branch->item = "joined";
  // join(root, branch);
  //
  // printf("%s\n", root->upper->item);



  for (int i = 1; i < argc; i++){
    insert(root,argv[i]);
    // printf("%d\n", i );
  }

  print_tree(root);
  // free_tree(root);

}
