#include "queue.h"
#include <assert.h>
#include <stdlib.h>

typedef struct node{
    int data;
    struct node *next;
}node_t;

typedef struct QueueRep{
    int length;
    node_t *head;
    node_t *tail;
}QueueRep;

queue new_queue(){
    queue q = malloc(sizeof(QueueRep));
    q->length = 0;
    q->head = NULL;
    q->tail = NULL;
    return q;
}

void drop_queue(queue q){
    node_t *curr = q->head;
    while(curr != NULL){
        node_t *temp = curr->next;
        free(curr);
        curr = temp;
    }
    free(q);
}

int queue_is_empty(queue q){
    return (q->length == 0);
}

void queue_enqueue(queue q, int v){
    node_t *new = malloc(sizeof(node_t));
    assert(new!= NULL);
    new->data = v;
    new->next = NULL;
    if(q->tail != NULL){
        q->tail->next = new;
        q->tail = new;
    }else{
        q->head = new;
        q->tail = new;
    }
    q->length++;
}

int queue_dequeue(queue q){
    assert(q->length > 0);
    node_t *p = q->head;
    q->head = q->head->next;
    if(q->head == NULL){
        q->tail = NULL;
    }
    q->length--;
    int d = p->data;
    free(p);
    return d;
}