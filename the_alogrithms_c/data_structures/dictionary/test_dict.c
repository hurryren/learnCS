#include <stdio.h>

#include "dict.h"

int main(){
    Dict *test_obj1;
    Dict *test_obj2;

    int value = 28;

    test_obj1 = create_dict();
    test_obj2 = create_dict();

    add_item_label(test_obj1, "age", &value);
    add_item_label(test_obj2, "name", "orange");

    printf("my age is %d\n", *((int *)get_element_lable(test_obj1, "age")));
    printf("my name is %s\n", (char *)get_element_lable(test_obj2, "name"));


    if(!add_item_index(test_obj1,0,&value)){
        printf("My age at index %d is %d\n",0,*((int*)get_element_index(test_obj1,0)));
    }

    destroy(test_obj1);
    destroy(test_obj2);

    return 0;
}