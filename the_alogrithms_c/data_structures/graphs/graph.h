#ifndef __GRAPH__
#define __GRAPH__

#include <stdbool.h>


typedef struct GraphRep *Graph;

typedef int Vertex;

typedef struct Edge
{
    Vertex v;
    Vertex w;
} Edge;

Graph new_graph(int);
void insert_edge(Graph, Edge);
void remove_edge(Graph, Edge);
bool adjacent(Graph, Vertex,Vertex);
void show_graph(Graph);
void free_graph(Graph);

#endif