#include <stdio.h>
#include <stdbool.h>
#include "graph.h"

#define MAX_NODES 1000

int visited[MAX_NODES];

bool dfs_path_check(Graph g, int nv, Vertex v, Vertex dest){
    Vertex w;
    for(w = 0; w < nv; w++){
        if(adjacent(g,v,w) && visited[w] == -1){
            visited[w] = v;
            if(w == dest)
                return true;
            else if(dfs_path_check(g, nv,w,dest))
                return true;
        }
    }
    return false;
}

bool find_path_dfs(Graph g, int nv, Vertex src, Vertex dest){
    Vertex v;
    for(v =0; v < nv;v++)
        visited[v] = -1;
    visited[src] = src;
    return dfs_path_check(g,nv,src,dest);
}

int main(void){
    int v = 6;
    Graph g = new_graph(v);

    Edge e;
    e.v = 0;
    e.w = 1;
    insert_edge(g, e);
    e.v = 0;
    e.w = 4;
    insert_edge(g, e);
    e.v = 0;
    e.w = 5;
    insert_edge(g, e);
    e.v = 5;
    e.w = 4;
    insert_edge(g, e);
    e.v = 4;
    e.w = 2;
    insert_edge(g, e);
    e.v = 4;
    e.w = 3;
    insert_edge(g, e);
    e.v = 5;
    e.w = 3;
    insert_edge(g, e);
    e.v = 1;
    e.w = 2;
    insert_edge(g, e);
    e.v = 3;
    e.w = 2;
    insert_edge(g, e);

    int src = 0, dest = 5;
    if(find_path_dfs(g,v,src,dest)){
        Vertex a = dest;
        while(a != src){
            printf("%d - ",a);
            a = visited[a];
        }
        printf("\n");
    }
    return 0;

}