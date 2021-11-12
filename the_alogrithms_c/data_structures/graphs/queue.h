#ifndef __QUEUE__
#define __QUEUE__

typedef struct QueueRep *queue;


queue new_queue();
void drop_queue(queue);  // remove unwanted queue
int queue_is_empty(queue);
void queue_enqueue(queue, int);
int queue_dequeue(queue);

#endif