#include "graph.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct GraphRep
{
    int **edges; // adjacency matric
    int nv;      // vertices
    int ne;      // edges
} GraphRep;

Graph new_graph(int v)
{
    assert(v >= 0);
    int i;

    Graph g = malloc(sizeof(GraphRep));
    assert(g != NULL);
    g->nv = v;
    g->ne = 0;

    // allocate memory for each row
    g->edges = malloc(v * sizeof(int *));
    assert(g->edges != NULL);

    // allocate memory for each column and initialise with 0
    for (i = 0; i < v; i++)
    {
        g->edges[i] = calloc(v, sizeof(int));
        assert(g->edges[i] != NULL);
    }

    return g;
}

// check if vertex is valid in a graph
bool valid_v(Graph g, Vertex v)
{
    return (g != NULL && v >= 0 && v < g->nv);
}

void insert_edge(Graph g, Edge e)
{
    assert(g != NULL && valid_v(g, e.v) && valid_v(g, e.w));

    if (!g->edges[e.v][e.w])
    {
        // edge e in grapg
        g->edges[e.v][e.w] = 1;
        g->edges[e.w][e.v] = 1;
        g->ne++;
    }
}

void remove_edge(Graph g, Edge e)
{
    assert(g != NULL && valid_v(g, e.v) && valid_v(g, e.w));

    if (g->edges[e.v][e.w])
    {
        // edge e in grapg
        g->edges[e.v][e.w] = 0;
        g->edges[e.w][e.v] = 0;
        g->ne--;
    }
}

bool adjacent(Graph g, Vertex v, Vertex w)
{
    assert(g != NULL && valid_v(g, v) && valid_v(g, w));

    return (g->edges[v][w] != 0);
}

void show_graph(Graph g)
{
    assert(g != NULL);
    int i, j;

    printf("Number of vertives: %d\n", g->nv);
    printf("Number of edges: %d\n", g->ne);
    for (i = 0; i < g->nv; i++)
    {
        for (j = i + 1; j < g->nv; j++)
        {
            if (g->edges[i][j])
                printf("Edge %d - %d\n", i, j);
        }
    }
}


void free_graph(Graph g){
    assert(g != NULL);
    int i;
    for(i=0;i<g->nv;i++)
        free(g->edges[i]);
    free(g->edges);
    free(g);
}
