#include <stdlib.h>
#include <iostream>
#include <stdio.h>
#include <string.h>
#include "bfs.h"
#include <time.h>
#include <queue>
using namespace std;
// int get_neighbors(const Grid *grid, Point p, Point neighb[])
// {

//     int dx[] = { -1,0,1,0 };
//     int dy[] = { 0,1,0,-1 };
//     int size = 0, x, y;
//     for (int i = 0; i < 4; i++)
//     {
//         x = p.row + dx[i];
//         y = p.col + dy[i];
//         if((x>=0)&&(x<=grid->rows))
//             if((y>=0)&&y<=grid->cols)
//                 if (grid->mat[x][y] == 0)
//                 {
//                     neighb[size].row = x;
//                     neighb[size].col = y;
//                     size++;
//                 }
//     }

//     return size;
// }

int get_neighbors(const Grid* grid, Point p, Point neighb[])
{
   int dx[] = { -2,-1,1,2,2,1,-1,-2 };
   int dy[] = { 1,2,2,1,-1,-2,-2,-1 };
   int size = 0, x, y;
   for (int i = 0; i < 8; i++)
   {
       x = p.row + dx[i];
       y = p.col + dy[i];
       if ((x >= 0) && (x <= grid->rows))
           if ((y >= 0) && y <= grid->cols)
               if (grid->mat[x][y] == 0)
               {
                   neighb[size].row = x;
                   neighb[size].col = y;
                   size++;
               }
   }

   return size;
}

void grid_to_graph(const Grid *grid, Graph *graph)
{
    Node *nodes[MAX_ROWS][MAX_COLS];
    int i, j, k;
    Point neighb[4];
   // Point neighb[8];

    graph->nrNodes = 0;
    for(i=0; i<grid->rows; ++i){
        for(j=0; j<grid->cols; ++j){
            if(grid->mat[i][j] == 0){
                nodes[i][j] = (Node*)malloc(sizeof(Node));
                memset(nodes[i][j], 0, sizeof(Node));
                nodes[i][j]->position.row = i;
                nodes[i][j]->position.col = j;
                ++graph->nrNodes;
            }else{
                nodes[i][j] = NULL;
            }
        }
    }
    graph->v = (Node**)malloc(graph->nrNodes * sizeof(Node*));
    k = 0;
    for(i=0; i<grid->rows; ++i){
        for(j=0; j<grid->cols; ++j){
            if(nodes[i][j] != NULL){
                graph->v[k++] = nodes[i][j];
            }
        }
    }

    for(i=0; i<graph->nrNodes; ++i){
        graph->v[i]->adjSize = get_neighbors(grid, graph->v[i]->position, neighb);
        if(graph->v[i]->adjSize != 0){
            graph->v[i]->adj = (Node**)malloc(graph->v[i]->adjSize * sizeof(Node*));
            k = 0;
            for(j=0; j<graph->v[i]->adjSize; ++j){
                if( neighb[j].row >= 0 && neighb[j].row < grid->rows &&
                    neighb[j].col >= 0 && neighb[j].col < grid->cols &&
                    grid->mat[neighb[j].row][neighb[j].col] == 0){
                        graph->v[i]->adj[k++] = nodes[neighb[j].row][neighb[j].col];
                }
            }
            if(k < graph->v[i]->adjSize){
                graph->v[i]->adjSize = k;
                graph->v[i]->adj = (Node**)realloc(graph->v[i]->adj, k * sizeof(Node*));
            }
        }
    }
}

void free_graph(Graph *graph)
{
    if(graph->v != NULL){
        for(int i=0; i<graph->nrNodes; ++i){
            if(graph->v[i] != NULL){
                if(graph->v[i]->adj != NULL){
                    free(graph->v[i]->adj);
                    graph->v[i]->adj = NULL;
                }
                graph->v[i]->adjSize = 0;
                free(graph->v[i]);
                graph->v[i] = NULL;
            }
        }
        free(graph->v);
        graph->v = NULL;
    }
    graph->nrNodes = 0;
}

void bfs(Graph *graph, Node *s, Operation *op)
{
    queue<Node*> Q;
    Node* nod;
    if (op != NULL) 
        op->count(4);
    Q.push(s);
    s->color = COLOR_GRAY;
    s->dist = 0;
    s->parent = NULL;
    while (!Q.empty()) 
    {
        if (op != NULL) 
            op->count(2);
        nod = Q.front();
        for (int i = 0; i < nod->adjSize; i++) 
        {
            if (op != NULL) 
                op->count();
            if (nod->adj[i]->color == COLOR_WHITE) 
            {
                if (op != NULL) 
                    op->count(4);
                Q.push(nod->adj[i]);
                nod->adj[i]->parent = nod;
                nod->adj[i]->color = COLOR_GRAY;
                nod->adj[i]->dist = nod->dist + 1;
            }
        }
        if (op != NULL) 
            op->count(2);
        Q.pop();
        nod->color = COLOR_BLACK;
    }

}

void PP1(int* a, Point* b, int size, int parent, int spaces)
{
    for (int i = 0; i < size; i++)
    {
        if (a[i] == parent)
        {
            for (int j = 0; j < spaces; j++)
            {
                printf("  ");
            }
            printf("(%d, %d)\t", b[i].row, b[i].col);
            printf("\n");
            PP1(a,b,size, i, spaces + 4);
        }
    }
}
void print_bfs_tree(Graph *graph)
{

    int n = 0; 
    int *p = NULL; 
    Point *repr = NULL;


    int *transf = (int*)malloc(graph->nrNodes * sizeof(int));
    for(int i=0; i<graph->nrNodes; ++i){
        if(graph->v[i]->color == COLOR_BLACK){
            transf[i] = n;
            ++n;
        }else{
            transf[i] = -1;
        }
    }
    if(n == 0){
        free(transf);
        return;
    }

    int err = 0;
    p = (int*)malloc(n * sizeof(int));
    repr = (Point*)malloc(n * sizeof(Node));
    for(int i=0; i<graph->nrNodes && !err; ++i){
        if(graph->v[i]->color == COLOR_BLACK){
            if(transf[i] < 0 || transf[i] >= n){
                err = 1;
            }else{
                repr[transf[i]] = graph->v[i]->position;
                if(graph->v[i]->parent == NULL){
                    p[transf[i]] = -1;
                }else{
                    err = 1;
                    for(int j=0; j<graph->nrNodes; ++j){
                        if(graph->v[i]->parent == graph->v[j]){
                            if(transf[j] >= 0 && transf[j] < n){
                                p[transf[i]] = transf[j];
                                err = 0;
                            }
                            break;
                        }
                    }
                }
            }
        }
    }
    free(transf);
    transf = NULL;

    if(!err){
        PP1(p, repr,n, -1, 0);
    }

    if(p != NULL){
        free(p);
        p = NULL;
    }
    if(repr != NULL){
        free(repr);
        repr = NULL;
    }
}

int shortest_path(Graph *graph, Node *start, Node *end, Node *path[])
{
    bfs(graph, end, NULL);
    int length = 0;
    Node* nod = start;
    while (nod != end)
    {
        path[length] = nod;
        nod = nod->parent;
        length++;
    }
    if (nod == end)
        return length;
    return -1;
}

bool is_edge_present(int** adjacencyMatrix, int x, int y)
{
    return adjacencyMatrix[x][y] || adjacencyMatrix[y][x];
}

void free_adjacency_matrix(int** adjacencyMatrix, int numNodes)
{
    for (int i = 0; i < numNodes; ++i)
    {
        free(adjacencyMatrix[i]);
    }
    free(adjacencyMatrix);
}

void generate_edges(Graph &graph, int numEdges, int numNodes)
{
    int** adjacencyMatrix = (int**)malloc(numNodes * sizeof(int*));
    int node1, node2, randNode;
    graph.nrNodes = numNodes;
    for (int i = 0; i < numNodes; ++i)
    {
        adjacencyMatrix[i] = (int*)calloc(numNodes, sizeof(int));
    }

    for (int i = 0; i < numNodes; i++)
    {
        graph.v[i]->position.row = rand() % 100;
        graph.v[i]->position.col = rand() % 100;
        graph.v[i]->adjSize = 0;
        graph.v[i]->adj = NULL;
        graph.v[i]->color = 0; 
        graph.v[i]->dist = 0;
        graph.v[i]->parent = NULL;
        
    }

    for (int i = 1; i < numNodes; i++)
    {
        randNode = rand() % i;
        node1 = randNode;
        node2 = i;
        adjacencyMatrix[node1][node2] = 1;
        adjacencyMatrix[node2][node1] = 1;

        graph.v[node1]->adj = (Node**)realloc(graph.v[node1]->adj, (graph.v[node1]->adjSize + 1) * sizeof(Node*));
        graph.v[node1]->adj[graph.v[node1]->adjSize] = graph.v[node2];
        graph.v[node1]->adjSize++;

        graph.v[node2]->adj = (Node**)realloc(graph.v[node2]->adj, (graph.v[node2]->adjSize + 1) * sizeof(Node*));
        graph.v[node2]->adj[graph.v[node2]->adjSize] = graph.v[node2];
        graph.v[node2]->adjSize++;
    }

    for (int i = numNodes-1; i < numEdges; i++)
    {
        do
        {
            node1 = rand() % numNodes;
            node2 = rand() % numNodes;
        } while (node1==node2 || is_edge_present(adjacencyMatrix, node1, node2) 
            || graph.v[node1]->adjSize >= numNodes - 1 || graph.v[node2]->adjSize >= numNodes - 1);

        adjacencyMatrix[node1][node2] = 1;
        adjacencyMatrix[node2][node1] = 1;

        graph.v[node1]->adj = (Node**)realloc(graph.v[node1]->adj, (graph.v[node1]->adjSize + 1) * sizeof(Node*));
        graph.v[node1]->adj[graph.v[node1]->adjSize] = graph.v[node2];
        graph.v[node1]->adjSize++;

        graph.v[node2]->adj = (Node**)realloc(graph.v[node2]->adj, (graph.v[node2]->adjSize + 1) * sizeof(Node*));
        graph.v[node2]->adj[graph.v[node2]->adjSize] = graph.v[node2];
        graph.v[node2]->adjSize++;
    }

    free_adjacency_matrix(adjacencyMatrix, numNodes);
}


void performance()
{
    int n, i;
    Profiler p("bfs");

    for(n=1000; n<=4500; n+=100){
        Operation op = p.createOperation("bfs-edges", n);
        Graph graph;
        graph.nrNodes = 100;
        graph.v = (Node**)malloc(graph.nrNodes * sizeof(Node*));
        for(i=0; i<graph.nrNodes; ++i){
            graph.v[i] = (Node*)malloc(sizeof(Node));
            memset(graph.v[i], 0, sizeof(Node));
        }
        generate_edges(graph, n, graph.nrNodes);

        bfs(&graph, graph.v[0], &op);
        free_graph(&graph);
    }

    for(n=100; n<=200; n+=10){
        Operation op = p.createOperation("bfs-vertices", n);
        Graph graph;
        graph.nrNodes = n;
        graph.v = (Node**)malloc(graph.nrNodes * sizeof(Node*));
        for(i=0; i<graph.nrNodes; ++i){
            graph.v[i] = (Node*)malloc(sizeof(Node));
            memset(graph.v[i], 0, sizeof(Node));
        }
        generate_edges(graph, 4500, graph.nrNodes);

        bfs(&graph, graph.v[0], &op);
        free_graph(&graph);
    }

    p.showReport();
}
