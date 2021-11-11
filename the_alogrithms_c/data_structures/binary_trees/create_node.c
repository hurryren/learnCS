#include<stdlib.h>
#include<stdlib.h>

struct node{
    struct node *left_node;
    int data;
    struct node *right_node;
};

struct node *new_node(int data){
    struct node *node = (struct node *) malloc(sizeof(struct node));

    node->left_node = NULL;
    node->data = data;
    node->right_node = NULL;

    return node;
}

int main(void){



    return 0;
}