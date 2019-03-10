//
//  quicksort.c
//  
//
//  Created by Timmy A Hussain on 3/6/19.
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void quicksort(char ** arr, int low, int high);
void swap(char ** elem1, char ** elem2);

int main(int argc, char ** argv) {
    
    int low = 0;
    int high = argc-1;
    
    quicksort(argv, low, high);
    
    for (int i = low; i < argc; i++) {
        printf("%s ", argv[i]);
    }
    printf("\n");
    
}
void swap(char ** elem1, char ** elem2) {
    char * temp = *elem1;
    *elem1 = *elem2;
    *elem2 = temp;
}

void quicksort(char ** arr, int low, int high) {
    
    if (low < high) {
        char * pivot = arr[low];
        int leftwall = low;
        
        for (int i = low+1; i <= high; i++) {
            if (strcmp(arr[i], pivot) < 0) {
//                char ** elem1 = &arr[i];
//                char ** elem2 = &arr[leftwall];
                printf("i: %d\n", i);
                printf("ith element: %s\n", arr[i]);
                printf("Pivot: %s\n", pivot);
                printf("arr[low]: %s\n", arr[low]);
                printf("Entire array: \n");
                int j =1;
                while (arr[j] != '\0') {
                    printf("%s ", arr[j]);
                    j++;
                }
                printf("\n \n");
                swap(&arr[i], &arr[leftwall]);
                leftwall++;
            }
        }
//        char ** elem1 = &pivot;
//        char ** elem2 = & arr[leftwall];
        printf("Pivot Swap\n");
        swap(&pivot, &arr[leftwall]);
//        free(pivot);
        printf("Array after swap: \n");
        int j = 1;
        while (arr[j] != '\0') {
            printf("%s ", arr[j]);
            j++;
        }
        printf("\n \n");
        quicksort(arr, low, leftwall);
        quicksort(arr, leftwall+1, high);
    }
    
}


