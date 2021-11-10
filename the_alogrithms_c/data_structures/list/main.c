#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"

void print_list(char **array){
    int i;
    for(i=0; array[i];i++)
        printf("%s", array[i]);
    printf("\n");
}

int main(){
    list_t *list1;
    list_t *list2;
    list_t *list3;

    char **str1 = (char **)malloc(100*sizeof(char*));

    list1 = list_init();
    list1 = list_push(list1, "Dang ");
    list1 = list_push(list1, "hoang ");
    list1 = list_push(list1, "hai ");
    printf("List 1: ");
    str1 = (char **)list_to_array(list1);
    print_list(str1);

    list2 = list_init();
    // printf("init addr %p ",list2);
    list2 = list_list(list2,"Mentor ", "Graphics ","Siemens",NULL);
    printf("\nList 2:\n");
    print_list((char **)list_to_array(list2));

    list3 = list_append(list1,list2);
    printf("\nTest append list2 into list1: ");
    print_list((char**)list_to_array(list3));

    return 0;
}