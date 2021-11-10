#ifndef __LIST__
#define __LIST__


typedef struct list_t{
    void *val;
    struct list_t * next;
}list_t;

extern list_t *list_init(void);
extern list_t *list_push(list_t *list, void *val);
extern int list_length(list_t *list);
extern void **list_to_array(list_t *list);
extern list_t *list_append(list_t *list, list_t *tail);
extern list_t *list_list(list_t *list, void *val,...);

#endif