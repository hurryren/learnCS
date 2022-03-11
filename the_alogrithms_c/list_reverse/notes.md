## 答案

```c
void reverse(node_t **head)
{
    node_t *prev=NULL;
    node_t *current = *head;
    node_t *next = *head;
    
    while(current){
        next = current->next;
        current->next = prev;
        
        prev = current;
        current = next;
    }
    *head = prev;
}
```

