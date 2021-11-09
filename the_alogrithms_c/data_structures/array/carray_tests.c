#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "carray.h"

int carray_tests(){
    printf("\n");
    printf(" +-------------------------------------+\n");
    printf(" |                                     |\n");
    printf(" |               C Array               |\n");
    printf(" |                                     |\n");
    printf(" +-------------------------------------+\n");
    printf("\n");

    CArray *array = get_carray(10);

    int i;
    for(i=0;i<array->size;i++){
        insert_value_carray(array,i,i+1);
    }
    printf("Entered array is:\n");
    display_carray(array);
    printf("\nCode: %d\n",push_value_carray(array,11)); // 5

    for(i=0;i<array->size;i++){
        remove_value_carray(array,i);
    }
    display_carray(array);

    printf("\nCode:%d",remove_value_carray(array,-1)); // 1
    printf("\nCode:%d\n",insert_value_carray(array,-1,1)); // 1

    // erase
    for(i = 0; i<array->size;i++){
        insert_value_carray(array,i,i+1);
    }
    erase_carray(array);
    display_carray(array); // should give all 0s


    // switching
    CArray *arr = get_carray(13);
    for(i=0;i<arr->size;i++){
        insert_value_carray(arr,i,i+1);
    }
    display_carray(arr);

    for(i=0;i<arr->size/2;i++){
        switch_value_carray(arr,i,arr->size-i-1);
    }
    display_carray(arr);


    reverse_carray(arr);
    display_carray(arr);

    // sorting
    srand(time(NULL));
    CArray *barray = get_carray(20);
    for(i=0;i<barray->size;i++){
        insert_value_carray(barray,i,rand());
    }
    CArray *carray = get_copy_carray(barray);
    CArray *darray = get_copy_carray(barray);
    printf("\nNot sorted Array:");
    display_carray(barray);

    printf("\nBubble Sort:");
    clock_t begin1 = clock();
    // Timing bubble sort
    bubble_sort_carray(barray);
    clock_t end1 = clock();
    double time_spend1 = (double)(end1-begin1) / CLOCKS_PER_SEC;
    display_carray(barray);

    printf("\nSelection Sort:");
    clock_t begin2 = clock();
    // Timing selection sort
    selection_sort_carray(carray);
    clock_t end2 = clock();
    double time_spend2 = (double)(end2-begin2) / CLOCKS_PER_SEC;
    display_carray(carray);

    printf("\nInsertion Sort:");
    clock_t begin3 = clock();
    // Timing insertion sort
    insertion_sort_carray(darray);
    clock_t end3 = clock();
    double time_spend3 = (double)(end3-begin3) / CLOCKS_PER_SEC;
    display_carray(darray);

    reverse_carray(barray);

    printf("\nTotal time spent for bubble sort: %lf seconds",time_spend1);
    printf("\nTotal time spent for selection sort: %lf seconds",time_spend2);
    printf("\nTotal time spent for insertion sort: %lf seconds",time_spend3);

    CArray *aarray = get_carray(1000);
    for(i=0;i<aarray->size;i++){
        insert_value_carray(aarray,i,rand() % 100);
    }
    int j=24;
    printf("\nOccurrences of the number %d in the array: %d", j,valueOcurrance_carray(aarray,j));
    printf("\nAnd its positions:\n");
    CArray *positions = value_position_carray(aarray,j);
    display_carray(positions);

    // this should all give value of j
    printf("\nAll %d s",j);
    for(i=0;i<positions->size;i++){
        printf("\nPosition %d has a value of %d",positions->array[i],aarray->array[positions->array[i]]);
    }

    printf("\nThe list has a minimum value of %d and a maximum value of %d", find_min_carray(aarray), find_max_carray(aarray));
    insertion_sort_carray(aarray);
    display_carray(aarray);

    free(arr);
    free(array);
    free(aarray);
    free(barray);
    free(carray);
    free(darray);
    printf("\n");

    return 0;

}


int main(){

    carray_tests();

    return 0;
}