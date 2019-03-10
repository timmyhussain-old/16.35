//
//  q5.c
//  
//
//  Created by Timmy A Hussain on 3/6/19.
//

#include <stdio.h>
#include <stdlib.h>

#include <string.h>
#include <stdbool.h>
#include <time.h>
#include "sorters.h"
// Test data for the algorithms
// Add more elements, and see how the algorithms scale!
#define N_ELEMENTS 10
char * data[N_ELEMENTS] = {"frog","gerbil","aardvark","horse","zebra",
    // "monkey","penguin","blesbok","gazelle","nyala",
    "elephant","seal","shark","hamster","bear"};
// Print function
void print_line(const char * name, double time, bool correct);
// Compares array (returns true if the array is sorted correctly).
bool sorted_correctly(int size, char ** arr);
// Copies the array
char ** get_copied_array(int size, char ** arr);
// Frees the copied array
void free_copied_array(int size,char ** copied_array);
// Structure for different sorting methods
typedef struct {
    char * name;
    void (*fnc) (int argc, char ** args);
} sorting_methods;
// Vector of methods - Add your method here!
#define N_SORTERS 3
sorting_methods sorters[N_SORTERS] = {{"bubble",bubble_sort},{"bogo",bogo_sort},{"quicksort", quicksort}};
// main function
int main(int argc, char ** argv) {
    // MODIFY THIS MAIN LOOP
    // print a table of the results:
    printf("----------------------------------------------------\n");
    printf("| name | duration (ms) | correct |\n");
    printf("----------------------------------------------------\n");
    
    //flag creation
    int flag = 1;
    
    //checks all words in passed string and if any aren't function names, flag value changed
    for (int requested_sorter = 1; requested_sorter < argc; requested_sorter++ ) {
        for (int sorter_idx = 0; sorter_idx < N_SORTERS; sorter_idx++) {
            if (strcmp(argv[requested_sorter],sorters[sorter_idx].name) != 0) flag = 0; break;
        }
    }
    
    //this loop only executes if flag value is 1 but flag = 0 if any arguments aren't funtion names
    if (flag == 1) {
        /* below is code as given*/
        for (int requested_sorter = 1; requested_sorter < argc; requested_sorter++ ) {
            for (int sorter_idx = 0; sorter_idx < N_SORTERS; sorter_idx++) {
                if (strcmp(argv[requested_sorter],sorters[sorter_idx].name) == 0) {
                    char ** copied_array = get_copied_array(N_ELEMENTS,data);
                    clock_t start_time = clock();
                    sorters[sorter_idx].fnc(N_ELEMENTS, copied_array);
                    clock_t end_time = clock();
                    double time = ((double)(end_time - start_time))/CLOCKS_PER_SEC*1000;
                    bool correct = sorted_correctly(N_ELEMENTS, copied_array);
                    print_line(sorters[sorter_idx].name,time,correct);
                    free_copied_array(N_ELEMENTS,copied_array);
                    break;
                }
            }
        }
    }
    
    //if flag is not = 1 (non-function words present)
    else {
        /* loop through present functions and run sorter on strings*/
        for (int sorter_idx = 0; sorter_idx < N_SORTERS; sorter_idx++) {
            char ** copied_array = get_copied_array(argc, argv);
            clock_t start_time = clock();
            sorters[sorter_idx].fnc(argc, argv);
            clock_t end_time = clock();
            double time = ((double)(end_time - start_time))/CLOCKS_PER_SEC*1000;
            bool correct = sorted_correctly(argc, argv);
            print_line(sorters[sorter_idx].name,time,correct);
            free_copied_array(argc,copied_array);
        }
    }
    printf("----------------------------------------------------\n");
    return 0;
}

/********************** Do not edit these functions ***************************/

// prints a table line
void print_line(const char * name, double time, bool correct){
    int space = 14 - strlen(name);
    printf("|"); if (space > 0) for (int i = 0; i < space/2; i++) printf(" ");
    printf("%s",name);
    if (space > 0) for (int i = 0; i < space/2; i++) printf(" ");
    if (space %2 !=0) printf(" ");
    printf("|"); char tmp [20]; sprintf(tmp,"%.3f ms", time);
    space = 20 - strlen(tmp);
    if (space > 2) for (int i = 0; i < space-2; i++) printf(" ");
    printf("%s",tmp); printf(" |");
    if (correct) printf(" yes |\n");
    else printf(" no |\n");
}
bool sorted_correctly(int size, char ** arr){
    bool v = true; for (int i =1; i < size; i++) v &= strcmp(arr[i-1],arr[i]) <= 0;
    return v;
}
char ** get_copied_array(int size, char ** arr){
    char ** copied_array = malloc(sizeof(char*)*size);
    for (int i = 0; i < size; i++) {
        copied_array[i] = malloc(strlen(arr[i])+1);
        strcpy(copied_array[i], data[i]);
    }
    return copied_array;
}
void free_copied_array(int size,char ** copied_array) {
    for (int i = 0; i < size; i++) free(copied_array[i]);
    free(copied_array);
}
