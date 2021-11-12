#include <stdio.h>
#include <stdlib.h>
#define SIZE 40
// assume max size of graph is 40 nodes
struct queue
{
    int items[SIZE];
    int front;
    int rear;
};

struct queue *create_queue();
void enquenu(struct queue *q, int);
int dequeue(struct queue *q);
void display(struct queue *q);
int is_empty(struct queue *q);
int poll_queue(struct queue *q);

struct node
{
    int vertex;
    struct node *next;
};

struct node *create_node(int);

// graph data structure
struct Graph
{
    int num_vertices;
    struct node **adj_lists;
    int *visited;
};
struct Graph *create_graph(int vertices);
void add_edge(struct Graph *graph, int src, int dest);
void print_graph(struct Graph *graph);
void bfs(struct Graph *graph, int start_vertex);

int main()
{
    int vertices, edges, source, i, src, dst;

    // printf("Enter the number of vertices\n");
    // scanf("%d", &vertices);
    // struct Graph *graph = create_graph(vertices);
    // printf("Enter the number of edges\n");
    // scanf("%d", &edges);
    // for(i=0;i<edges;i++){
    //     printf("Edge %d \n enter source: ", i+1);
    //     scanf("%d", &src);
    //     printf("enter the destination: ");
    //     scanf("%d", &dst);
    //     add_edge(graph, src, dst);
    // }
    // printf("Enter source of bfs\n");
    // scanf("%d", &source);
    // bfs(graph, source);

    struct Graph *graph = create_graph(6);
    add_edge(graph, 0, 1);
    add_edge(graph, 0, 2);
    add_edge(graph, 1, 2);
    add_edge(graph, 1, 4);
    add_edge(graph, 1, 3);
    add_edge(graph, 2, 4);
    add_edge(graph, 3, 4);
    bfs(graph, 0);

    return 0;
}

void bfs(struct Graph *graph, int start_vertex){
    struct queue *q = create_queue();

    //  add to visted list and put in queue
    graph->visited[start_vertex] = 1;
    enquenu(q, start_vertex);
    printf("Breadth first traversal from vertex %d is: \n", start_vertex);

    while(!is_empty(q)){
        printf("%d ",poll_queue(q));
        int current_vertext = dequeue(q);

        struct node *temp = graph->adj_lists[current_vertext];
        while(temp){
            int adj_vertex = temp->vertex;
            if(graph->visited[adj_vertex] == 0){
                graph->visited[adj_vertex] = 1;
                enquenu(q,adj_vertex);
            }
            temp = temp->next;
        }
    }
    printf("\n");
}

struct queue *create_queue(){
    struct queue *q = malloc(sizeof(struct queue));
    q->front = -1;
    q->rear = -1;
    return q;
}

int is_empty(struct queue *q){
    if(q->rear == -1)
        return 1;
    else
        return 0;
}

void enquenu(struct queue *q, int value){
    if(q->rear == SIZE -1)
        printf("\nqueue is full\n");
    else{
        if(q->front == -1)
            q->front = 0;
        q->rear++;
        q->items[q->rear] = value;
    }
}

int dequeue(struct queue *q){
    int item;
    if(is_empty(q)){
        printf("queue is empty");
        item = -1;
    }else{
        item = q->items[q->front];
        q->front++;
        if(q->front > q->rear){
            q->front = q->rear = -1;
        }
    }
    return item;
}

int poll_queue(struct queue *q){
    return q->items[q->front];
}

struct node *create_node(int v)
{
    struct node *new_node = (struct node *)malloc(sizeof(struct node));
    new_node->vertex = v;
    new_node->next = NULL;
    return new_node;
}

struct Graph *create_graph(int vertices)
{
    struct Graph *graph = (struct Graph *)malloc(sizeof(struct Graph));
    graph->num_vertices = vertices;

    graph->adj_lists = malloc(vertices * sizeof(struct node *));
    graph->visited = (int *)malloc(vertices * (sizeof(int)));

    int i;
    for (i = 0; i < vertices; i++)
    {

        graph->adj_lists[i] = NULL;
        graph->visited[i] = 0;
    }

    return graph;
}


void add_edge(struct Graph *graph, int src, int dest){

    //  add edge from src to dest
    struct node *new_node = create_node(dest);
    new_node->next = graph->adj_lists[src];
    graph->adj_lists[src] = new_node;

    // add edge from dest to src; comment it out for directed graph
    new_node = create_node(src);
    new_node->next = graph->adj_lists[dest];
    graph->adj_lists[dest] = new_node;
}
