#ifndef __DICT_H__
#define __DICT_H__

#define MAXELEMENTS 1000

typedef struct Dictionary{
    void *elements[MAXELEMENTS];

    int number_of_elements;
}Dict;


Dict *create_dict(void);

// 0: success  -1: fals
int add_item_label(Dict *,char label[], void *);

// 0: success  -1: fals
int add_item_index(Dict *, int index, void *);

// return the element at given label
void *get_element_lable(Dict *, char[]);

// return the element at given index
void *get_element_index(Dict *, int);


void destroy(Dict *);

#endif