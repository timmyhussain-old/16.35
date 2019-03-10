#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Node structure
typedef struct Node {
struct Node * upper;
struct Node * lower;
char * item;
} node;

void join(node *Lower, node *Upper) {
  Lower->upper = Upper;
  Upper->lower = Lower;
}

void insert(node * root, char * str) {
    
    //for recursion, we recur until we reach a null node
    if (root != NULL ) {
        //simple case: node has empty string -> first string allocated
        if (strcmp(root->item, "") == 0) {
            root->item = str;
//            printf("base case: %s\n", str);
        }
        //recursion
        else {
            int bool = strcmp(root->item, str);     //boolean for string comparison
            if (bool > 0 ) {
                insert(root->upper, str);           //recur until hit a null node then progress
                node * node_2 = malloc(sizeof(node));   //create new node and assign it string
                node_2->upper = NULL;
                node_2->lower = NULL;
                node_2->item = str;
                root->upper = node_2;               //link root to new node
//                printf("upper: %s\n", str);       //checker
            }
            else {                                  //same idea but for lower branches
                insert(root->lower, str);
                node * node_2 = malloc(sizeof(node));
                node_2->upper = NULL;
                node_2->lower = NULL;
                node_2->item = str;
                root->lower = node_2;
//                printf("lower: %s\n", str);
            }
        }
    }
  /*base case*/
    
/* Old implementation below
 
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
*/
}
void print_tree(node * root) {

    
    /* Old print for old implementation
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
    */
    
    /* left first until null and then self and then right branches and then recur*/
    if (root != NULL) {
        print_tree(root->lower);
        printf("%s ", root->item);
        print_tree(root->lower);

    }
}

void free_tree(node * root) {
  if (root != NULL) {
    free_tree(root->upper);
    free(root);
    free_tree(root->lower);
    
  }
}
int main (int argc, char ** argv) {
  node * root = malloc(sizeof(node));

  root->upper = NULL;
  root->lower = NULL;
  root->item = "";

  for (int i = 1; i < argc; i++){
    insert(root,argv[i]);
  }

print_tree(root);
free_tree(root);

}
