#include <stdio.h>
#include <stdlib.h>

struct avlnode
{
    int key;
    struct avlnode *left;
    struct avlnode *right;
    int height;
};
typedef struct avlnode avl_node;

int max(int a, int b) { return (a > b) ? a : b; }

avl_node *new_node(int key){
    avl_node *node = (avl_node *) malloc(sizeof(avl_node));

    if(node == NULL)
        printf("!! out of space !!\n");
    else {
        node->key = key;
        node->left = NULL;
        node->right = NULL;
        node->height = 0;
    }

    return node;
}

int node_height(avl_node *node){
    if(node == NULL)
        return -1;
    else
        return node->height;
}

int height_diff(avl_node *node){
    if(node == NULL)
        return 0;
    else
        return node_height(node->left) - node_height(node->right);
}

avl_node *min_node(avl_node *node){
    avl_node *temp = node;

    while(temp->left != NULL) temp = temp->left;

    return temp;
}

void print_avl(avl_node *node, int level){
    int i;
    if(node != NULL){
        print_avl(node->right,level + 1);
        printf("\n\n");

        for(i=0;i<level;i++) printf("\t");

        printf("%d",node->key);
        print_avl(node->left,level + 1);
    }
}

avl_node *right_rotate(avl_node *z){
    avl_node *y = z->left;
    avl_node *t3 = y->right;
    y->right = z;
    z->left = t3;

    z->height=(max(node_height(z->left),node_height(z->right)) + 1);
    y->height=(max(node_height(y->left),node_height(y->right)) + 1);

    return y;
}

avl_node *left_rotate(avl_node *z){
    avl_node *y = z->right;
    avl_node *t3 = y->left;
    y->left = z;
    z->right = t3;

    z->height=(max(node_height(z->left),node_height(z->right)) + 1);
    y->height=(max(node_height(y->left),node_height(y->right)) + 1);

    return y;
}

avl_node *lr_rotate(avl_node *node){
    node->left = left_rotate(node->left);

    return right_rotate(node);
}

avl_node * rl_rotate(avl_node *node){
    node->right = right_rotate(node->right);
    return left_rotate(node);
}


avl_node *insert(avl_node *node, int key){
    if(node == NULL)
        return new_node(key);

    if(key < node->key)
        node->left = insert(node->left, key);
    else if(key > node->key)
        node->right = insert(node->right, key);

    node->height = (max(node_height(node->left), node_height(node->right)) + 1);

    int balance = height_diff(node);

    if(balance > 1 && key < (node->left->key))
        return right_rotate(node);

    if(balance < -1 && key > (node->right->key))
        return left_rotate(node);

    if(balance > 1 && key > node->left->key)
        node = lr_rotate(node);

    if(balance < -1 && key < node->right->key)
        node = rl_rotate(node);

    return node;
}


avl_node *delete(avl_node *node, int query_num){
    if(node == NULL)
        return node;

    if(query_num < node->key)
        node->left = delete(node->left, query_num);
    else if(query_num > node->key)
        node->right = delete(node->right, query_num);
    else{
        if((node->left == NULL) || (node->right == NULL)){
            avl_node *temp = node->left ? node->left : node->right;

            if(temp == NULL){
                temp = node;
                node = NULL;
            }else{
                *node = *temp;
            }

            free(temp);
        }else{
            avl_node *temp = min_node(node->right);
            node->key = temp->key;
            node->right = delete(node->right,temp->key);
        }
    }

    if(node == NULL)
        return node;

    node->height = (max(node_height(node->left), node_height(node->right)) + 1);

    int balance = height_diff(node);

     if(balance > 1 && height_diff(node->left) >= 0)
        return right_rotate(node);

    if(balance < -1 && height_diff(node->right) >= 0)
        return left_rotate(node);

    if(balance > 1 && height_diff(node->left) < 0)
        node = lr_rotate(node);

    if(balance < -1 && height_diff(node->right) < 0)
        node = rl_rotate(node);

    return node;

}

avl_node *find_node(avl_node *node, int query_num){
    if(node != NULL){
        if(query_num < node->key)
            node = find_node(node->left, query_num);
        else if(query_num > node->key)
            node = find_node(node->right, query_num);
    }
    return node;
}

void print_pre_order(avl_node *node){
    if(node != NULL){
        printf("  %d  ",(node->key));
        print_pre_order(node->left);
        print_pre_order(node->right);
    }
}

void print_in_order(avl_node *node){
    if(node != NULL){
        print_pre_order(node->left);
        printf("  %d  ",(node->key));
        print_pre_order(node->right);
    }
}


void print_post_order(avl_node *node){
    if(node != NULL){
        print_pre_order(node->left);
        print_pre_order(node->right);
        printf("  %d  ",(node->key));
    }
}

int main(){
    int choice;
    int flag = 1;
    int insert_num;
    int query_num;

    avl_node * root = NULL;
    avl_node * temp_node;

    while(flag == 1){
        printf("\n\nEnter the Step to Run : \n");

        printf("\t1: Insert a node into AVL tree\n");
        printf("\t2: Delete a node in AVL tree\n");
        printf("\t3: Search a node into AVL tree\n");
        printf("\t4: printPreOrder (Ro L R) Tree\n");
        printf("\t5: printInOrder (L Ro R) Tree\n");
        printf("\t6: printPostOrder (L R Ro) Tree\n");
        printf("\t7: printAVL Tree\n");

        printf("\t0: EXIT\n");
        scanf("%d",&choice);

        switch (choice){
        case 0:{
            flag = 0;
            printf("\n\t\tExiting, Thank you !!\n");
            break;
        }
        case 1:{
            printf("\n\tEnter the number to insert: ");
            scanf("%d",&insert_num);

            temp_node = find_node(root, insert_num);
            if(temp_node != NULL)
                printf("\n\t %d already exists in the tree\n", insert_num);
            else{
                printf("\n\t Printing avl tree\n");
                print_avl(root,1);
                printf("\n");

                root = insert(root, insert_num);
                printf("\n\t Printing avl tree\n");
                print_avl(root,1);
                printf("\n");
            }
            break;
        }

        case 2:{
            printf("\n\tEnter the number to delete: ");
            scanf("%d",&query_num);

            temp_node = find_node(root, query_num);
            if(temp_node == NULL)
                printf("\n\t %d does not exists in the tree\n", query_num);
            else{
                printf("\n\t Printing avl tree\n");
                print_avl(root,1);
                printf("\n");

                root = delete(root, query_num);
                printf("\n\t Printing avl tree\n");
                print_avl(root,1);
                printf("\n");
            }
            break;
        }

        case 3:{
            printf("\n\tEnter the number to search: ");
            scanf("%d",&query_num);

            temp_node = find_node(root, query_num);
            if(temp_node == NULL)
                printf("\n\t not found\n");
            else{
                printf("\n\t %d found at height %d \n", query_num,temp_node->height);

                printf("\n\t Printing avl tree\n");
                print_avl(root,1);
                printf("\n");
            }
            break;
        }

        case 4:
        {
            printf("\nprint tree pre order\n");
            print_pre_order(root);

            break;
        }

        case 5:
        {
            printf("\nprint tree in order\n");
            print_in_order(root);

            break;
        }

        case 6:
        {
            printf("\nprint tree post order\n");
            print_post_order(root);

            break;
        }

        case 7:
        {
            printf("\n\t Printing avl tree\n");
            print_avl(root,1);
            printf("\n");
            break;
        }

        default:{
            flag = 0;
            printf("\n\t\t exiting, thank you !!\n");
            break;
        }
        }
    }
    return 0;
}