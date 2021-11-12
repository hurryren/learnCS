#include <stdbool.h>
#include <stdio.h>
#include "graph.h"
#include "queue.h"

#define MAX_NODES 1000

int visited[MAX_NODES]; // array to store visiting order
                        // indexed by vertex 0..nv-1

bool find_path_bfs(Graph g, int nv, Vertex src, Vertex dest)
{
    Vertex v;
    for (v = 0; v < nv; v++){
        visited[v] = -1;
    }

    visited[src] = src;
    queue q = new_queue();
    queue_enqueue(q, src);
    while(!queue_is_empty(q)){
        v = queue_dequeue(q);
        Vertex w;
        for(w=0;w<nv;w++){
            if(adjacent(g,v,w) && visited[w] == -1){
                visited[w] = v;
                if(w == dest)
                    return true;
                else
                    queue_enqueue(q,w);
            }
        }
    }
    return false;
}


int main(){
    int v =10;
    Graph g = new_graph(v);
    Edge e;
    e.v=0;
    e.w=1;
    insert_edge(g,e);
    e.v=0;
    e.w=2;
    insert_edge(g,e);
    e.v=0;
    e.w=5;
    insert_edge(g,e);
    e.v=1;
    e.w=5;
    insert_edge(g,e);
    e.v=2;
    e.w=3;
    insert_edge(g,e);
    e.v=3;
    e.w=4;
    insert_edge(g,e);
    e.v=3;
    e.w=5;
    insert_edge(g,e);
    e.v=3;
    e.w=8;
    insert_edge(g,e);
    e.v=4;
    e.w=5;
    insert_edge(g,e);
    e.v=4;
    e.w=7;
    insert_edge(g,e);
    e.v=4;
    e.w=8;
    insert_edge(g,e);
    e.v=5;
    e.w=6;
    insert_edge(g,e);
    e.v=7;
    e.w=8;
    insert_edge(g,e);
    e.v=7;
    e.w=9;
    insert_edge(g,e);
    e.v=8;
    e.w=9;
    insert_edge(g,e);
    // e.v=2;
    // e.w=6;
    // insert_edge(g,e);




    int src = 0;
    int dest = 6;
    if(find_path_bfs(g,v,src,dest)){
        Vertex v = dest;
        while(v != src){
            printf("%d - ",v);
            v = visited[v];
        }
        printf("%d\n", src);
    }


    return 0;
}