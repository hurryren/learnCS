#include "list.h"
#include <assert.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

list_t *list_init(void){
    list_t *list;
    list = (list_t *)malloc(sizeof(list_t));
    list->next = NULL;
    return list;
}

list_t *list_push(list_t * list, void *val){
    list_t *new_elem = (list_t *)malloc(sizeof(list_t));
    new_elem->val = val;
    new_elem->next = list;
    return new_elem;
}

int list_length(list_t *list){
    int n;
    for(n =0;list;list=list->next)n++;
    return n;
}

void **list_to_array(list_t *list){
    int i,n = list_length(list);
    void **array = (void **)malloc((n+1)*sizeof(void *));

    for(i=0;i<n;i++){
        array[i] = list->val;
        list = list->next;
    }
    array[i] = NULL;
    return array;
}


list_t *list_list(list_t *list, void *val, ...){
    va_list ap;
    list_t **p = &list;

    va_start(ap,val);
    for(;val;val=va_arg(ap,void *)){
        *p=malloc(sizeof(list_t));
        (*p)->val = val;
        p = &(*p)->next;
    }
    *p = NULL;
    va_end(ap);
    return list;
}

list_t *list_append(list_t *list, list_t *tail){
    list_t **p = &list;
    while((*p)->next){
        p=&(*p)->next;
    }

    // printf("%s \n",(*p)->val);
    *p = tail;
    return list;
}