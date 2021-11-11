#include <stdio.h>
#include <stdlib.h>

typedef struct node
{
    struct node *left;
    struct node *right;
    int data;
} node;

node *new_node(int data)
{
    node *tmp = (node *)malloc(sizeof(node));

    tmp->data = data;
    tmp->left = NULL;
    tmp->right = NULL;

    return tmp;
}

node *insert(node *root, int data)
{
    if (root == NULL)
    {
        root = new_node(data);
    }
    else if (data > root->data)
        root->right = insert(root->right, data);
    else if (data < root->data)
        root->left = insert(root->left, data);

    return root;
}

node *get_max(node *root)
{
    if (root->right != NULL)
        return get_max(root->right);

    return root;
}

node *delete (node *root, int data)
{
    if (root == NULL)
        return root;
    else if (data > root->data)
        root->right = delete (root->right, data);
    else if (data < root->data)
        root->left = delete (root->left, data);
    else if (data == root->data)
    {
        if ((root->left == NULL) && (root->right == NULL))
        {
            free(root);
            return NULL;
        }
        else if (root->left == NULL)
        {
            node *tmp = root;
            root = root->right;
            free(tmp);
            return root;
        }
        else if (root->right == NULL)
        {
            node *tmp = root;
            root = root->left;
            free(tmp);
            return root;
        }
        else
        {
            node *tmp = get_max(root->left);
            root->data = tmp->data;
            root->left = delete (root->left, tmp->data);
        }
    }
    return root;
}

int find(node *root, int data)
{
    if (root == NULL)
        return 0;
    else if (data > root->data)
        return find(root->right, data);
    else if (data < root->data)
        return find(root->left, data);
    else if (data == root->data)
        return 1;
    else
        return 0;
}

int height(node *root)
{
    if (root == NULL)
        return 0;
    else
    {
        int right_h = height(root->right);
        int left_h = height(root->left);

        if (right_h > left_h)
            return right_h + 1;
        else
            return left_h + 1;
    }
}

void purge(node *root)
{
    if (root != NULL)
    {
        if (root->left != NULL)
            purge(root->left);
        if (root->right != NULL)
            purge(root->right);
        free(root);
        root = NULL;
    }
}

void in_order(node *root)
{
    if (root != NULL)
    {
        in_order(root->left);
        printf("\t[ %d ]\t", root->data);
        in_order(root->right);
    }
}

int main()
{
    node *root = NULL;
    int opt = -1;
    int data = 0;

    while(opt != 0){
        printf("\n\n[1] insert node\n");
        printf("[2] delete node\n");
        printf("[3] find node\n");
        printf("[4] get current height\n");
        printf("[5] print tree in crescent order\n");
        printf("[0] quit\n");
        scanf("%d", &opt);

        switch(opt){
        case 1:
            printf("enter the new node's value: ");
            scanf("%d",&data);
            root = insert(root,data);
            break;

        case 2:
            printf("enter the value to be removed: ");
            if(root != NULL){
                scanf("%d", &data);
                root = delete(root, data);
            }else{
                printf("there is already empty\n");
            }
            break;

        case 3:
            printf("enter the searched value: ");
            scanf("%d", &data);
            find(root,data) ? printf("The value is in the tree.\n") : printf("the value is not in the tree.\n");
            break;

        case 4:
            printf("current height of the tree is : %d\n",height(root));
            break;

        case 5:
            in_order(root);
            break;
        }
    }

    purge(root);

    return 0;
}