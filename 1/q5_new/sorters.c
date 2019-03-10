//
//  sorters.c
//  
//
//  Created by Timmy A Hussain on 3/6/19.
//

// Various implementations of sorting algorithms.
#include "sorters.h"
#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

void swap(char ** elem1, char ** elem2) {
    char * temp = *elem1;
    *elem1 = *elem2;
    *elem2 = temp;
}

void bubble_sort(int argc, char ** args) {
    for (int i = 0; i < argc; i++) {
        for (int j = 0; j < argc - i - 1; j++) {
            if (strcmp(args[j], args[j + 1]) > 0) {
                swap(&args[j], &args[j + 1]);
            }
        }
    }
}

void bogo_sort(int argc, char ** args) {
    int num_swaps = 10;
    int seed = 2;
    srand(seed);
    // Idea: randomly shuffle. If the list is sorted, return. If not, try again.
    bool is_sorted = false;
    while (!is_sorted) {
        // Shuffle the list.
        for (int i = 0; i < num_swaps; i++) {
            int index1 = rand() % argc;
            int index2 = rand() % argc;
            swap(&args[index1], &args[index2]);
        }
        // Update is_sorted depending on list.
        is_sorted = true;
        for (int i = 0; i < argc - 1; i++) {
            if (strcmp(args[i], args[i + 1]) > 0) {
                is_sorted = false; break;
            }
        }
    }
}

void partition(char ** arr, int low, int high) {
    
    if (low < high) {
        char * pivot = malloc(strlen(arr[low])+1);//arr[low];
        strcpy(pivot, arr[low]);
        int leftwall = low;
        
        for (int i = low+1; i <= high; i++) {
            if (strcmp(arr[i], pivot) < 0) {
                swap(&arr[i], &arr[leftwall]);
                leftwall++;
            }
        }

        swap(&pivot, &arr[leftwall]);
        
        partition(arr, low, leftwall);
        partition(arr, leftwall+1, high);
    }
    
}

void quicksort(int argc, char ** argv) {
    
    int low = 0;
    int high = argc-1;
    
    partition(argv, low, high);

}



