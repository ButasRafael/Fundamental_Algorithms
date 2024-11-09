#include <iostream>
#include "Profiler.h"
#include <cstdlib>
#include <ctime>
#include <stack>
using namespace std;
Profiler prof("DFS");

enum {
    COLOR_WHITE = 0,
    COLOR_GRAY = 1,
    COLOR_BLACK = 2
};

typedef struct Node {
    int value;
    int adjSize;
    struct Node** adj;
    int color;
    struct Node* parent;
    int index;
    int lowlink;
    int comp;
    bool on_stack;
}node;

typedef struct Graph {
    int nrNodes;
    Node** v;
}graph;

bool areEdgesPresent(int** adjacencyMatrix, int x, int y)
{
    return adjacencyMatrix[x][y] && adjacencyMatrix[y][x];
}

void freeAdjacencymatrix(int** adjacencyMatrix, int numNodes)
{
    for (int i = 0; i < numNodes; ++i)
    {
        free(adjacencyMatrix[i]);
    }
    free(adjacencyMatrix);
}

void freeGraph(graph* graph)
{
    if (graph->v != NULL) {
        for (int i = 0; i < graph->nrNodes; ++i) {
            if (graph->v[i] != NULL) {
                if (graph->v[i]->adj != NULL) {
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

node* createNode(int value)
{
    node* new_node = (node*)malloc(sizeof(node));
    new_node->value = value;
    new_node->adjSize = 0;
    new_node->adj = NULL;
    new_node->color = COLOR_WHITE;
    new_node->parent = NULL;
    new_node->index = 0;
    new_node->lowlink = 0;
    new_node->comp = 0;
    new_node->on_stack = false;
    return new_node;
}

void createEdge(node* node1, node* node2)
{
    node1->adj = (Node**)realloc(node1->adj, (node1->adjSize + 1) * sizeof(Node*));
    node1->adj[node1->adjSize] = node2;
    node1->adjSize++;
}

void createGraph(graph*graph, int numEdges, int numNodes)
{
    int** adjacencyMatrix = (int**)malloc(numNodes * sizeof(int*));
    int node1, node2;
    graph->nrNodes = numNodes;
    for (int i = 0; i < numNodes; ++i)
    {
        adjacencyMatrix[i] = (int*)calloc(numNodes, sizeof(int));
    }
    graph->v = (node**)malloc(numNodes * sizeof(node*));
    for (int i = 0; i < numNodes; i++)
    {
        graph->v[i] = createNode(i);
    }

    for (int i = 0; i < numEdges; i++)
    {
        do
        {
            node1 = rand() % numNodes;
            node2 = rand() % numNodes;
        } while (node1 == node2 || areEdgesPresent(adjacencyMatrix, node1, node2)
            || graph->v[node1]->adjSize > numNodes - 1 || graph->v[node2]->adjSize > numNodes - 1);
        if (adjacencyMatrix[node1][node2] == 0)
        {
            adjacencyMatrix[node1][node2] = 1;
            createEdge(graph->v[node1], graph->v[node2]);
        }
        else
        {
            adjacencyMatrix[node2][node1] = 1;
            createEdge(graph->v[node2], graph->v[node1]);
        }
    }

    freeAdjacencymatrix(adjacencyMatrix, numNodes);
}

void DFS(node* start, Operation& op)
{
    op.count();
    start->color = COLOR_GRAY;
    for (int i = 0; i < start->adjSize; i++)
    {
        op.count();
        if (start->adj[i]->color == COLOR_WHITE)
        {
            op.count();
            start->adj[i]->parent = start;
            DFS(start->adj[i],op);
        }
    }
    op.count();
    start->color = COLOR_BLACK;
}

void DFS_ALL(graph* graph, Operation& op)
{
    for (int i = 0; i < graph->nrNodes; i++)
    {
        op.count();
        if (graph->v[i]->color == COLOR_WHITE)
            DFS(graph->v[i], op);
    }
}

void stackDFS(node* start, stack<int>& OrderedNodes)
{
    start->color = COLOR_GRAY;
    for (int i = 0; i < start->adjSize; i++)
    {
        if (start->adj[i]->color == COLOR_WHITE)
        {
            start->adj[i]->parent = start;
            stackDFS(start->adj[i], OrderedNodes);
        }
    }
    start->color = COLOR_BLACK;
    OrderedNodes.push(start->value);
}

void stackDFS_ALL(graph* graph, stack<int>& OrderedNodes)
{
    for (int i = 0; i < graph->nrNodes; i++) {
        graph->v[i]->color = COLOR_WHITE;
    }
    for (int i = 0; i < graph->nrNodes; i++)
    {
        if (graph->v[i]->color == COLOR_WHITE)
            stackDFS(graph->v[i],OrderedNodes);
    }
}

void TopologicalSort(graph* graph)
{
    stack<int>OrderedNodes;
    stackDFS_ALL(graph, OrderedNodes);
    while (!OrderedNodes.empty())
    {
        cout << OrderedNodes.top() << " ";
        OrderedNodes.pop();
    }
}

void ConnectedComp(graph* graph,node* start, int& index, int&NrofComponents, stack<int>& nstack)
{
    start->index = start->lowlink = index;
    index++;
    nstack.push(start->value);
    start->on_stack = true;
    for (int i = 0; i < start->adjSize; i++)
    {
        if (start->adj[i]->index == -1)
        {
            ConnectedComp(graph,start->adj[i], index, NrofComponents,nstack);
            start->lowlink = min(start->lowlink, start->adj[i]->lowlink);
        }
        else if(start->adj[i]->on_stack)
        {
            start->lowlink = min(start->lowlink, start->adj[i]->index);
        }
    }
    if (start->index == start->lowlink)
    {
        cout << "Component " << NrofComponents << ": ";
        int current;
        do
        {
            current = nstack.top();
            nstack.pop();
            graph->v[current]->on_stack = false;
            graph->v[current]->comp = NrofComponents;
            cout << current << " ";
            

        } while (current != start->value);
        cout << endl;
        NrofComponents++;
    }
}

void Tarjan(graph* graph)
{
    int index = 0, NrofComponents = 0;
    stack <int>nstack;
    for (int i = 0; i < graph->nrNodes; i++)
    {
        graph->v[i]->lowlink = -1;
        graph->v[i]->index = -1;
        graph->v[i]->on_stack = false;
        graph->v[i]->comp = 0;
    }
    for (int i = 0; i < graph->nrNodes; i++)
    {
        if (graph->v[i]->index == -1)
        {
            ConnectedComp(graph, graph->v[i], index, NrofComponents, nstack);
        }
    }
}

void prettyPrint(int* p, int size, int parent, int spaces)
{
    for (int i = 0; i < size; i++)
    {

        if (p[i] == parent)
        {
            for (int j = 0; j < spaces; j++)
            {
                cout << "  ";
            }
            cout << i << endl;
            prettyPrint(p, size, i, spaces + 4);
        }


    }
}

void printDFStree(graph* graph)
{
    if (graph == NULL)
        return;
    int* p = (int*)malloc(graph->nrNodes * sizeof(int));
    for (int i = 0; i < graph->nrNodes; i++)
    {
        if (graph->v[i]->parent == NULL)
            p[graph->v[i]->value] = -1;
        else
            p[graph->v[i]->value] = graph->v[i]->parent->value;
    }
    prettyPrint(p, graph->nrNodes, -1, 0);

}

void printGraph(graph* graph)
{
    if (graph == NULL)
        return;
    for (int i = 0; i < graph->nrNodes; i++)
    {
        cout << graph->v[i]->value << ": ";
        for (int j = 0; j < graph->v[i]->adjSize; j++)
            cout << graph->v[i]->adj[j]->value << " ";
        cout << endl;
    }

}

void demo()
{
    Operation op = prof.createOperation("dummy", 0);
    graph* newGraph = (graph*)malloc(sizeof(graph));
    createGraph(newGraph, 18,10);
    cout << "Original Graph:" << endl;
    printGraph(newGraph);
    cout << endl;

    DFS_ALL(newGraph, op);
    cout << "DFS Tree:" << endl;
    printDFStree(newGraph);
    cout << endl << endl;

    graph* g = (graph*)malloc(sizeof(graph));
    g->v = (node**)malloc(8 * sizeof(node*));
    g->nrNodes = 8;
    for (int i = 0; i < 8; i++)
    {
        g->v[i] = createNode(i);
    }

    createEdge(g->v[0], g->v[1]);  //undershorts->pants
    createEdge(g->v[0], g->v[2]);  //undershorts->shoes
    createEdge(g->v[1], g->v[3]);  //pants->belt
    createEdge(g->v[1], g->v[2]);  //pants->shoes
    createEdge(g->v[4], g->v[2]);  //socks->shoes
    createEdge(g->v[3], g->v[5]);  //belt->jacket
    createEdge(g->v[6], g->v[3]);  //shirt->belt
    createEdge(g->v[6], g->v[7]);  //shirt->tie
    createEdge(g->v[7], g->v[5]);  //tie->jacket
    cout << "Direct Acyclic Graph:" << endl;
    printGraph(g);

    cout << "Topological Sort:" << endl;
    TopologicalSort(g);
    cout << endl;

    cout << "Connected Components using Tarjan Algorithm:" << endl;
    Tarjan(newGraph);
    freeGraph(newGraph);
    freeGraph(g);


}


void perf()
{
    int numEdges, numNodes = 100;
    graph* newGraph=(graph*)malloc(sizeof(graph));
    for (numEdges = 1000; numEdges <= 4500; numEdges += 100) {
        Operation op = prof.createOperation("DFS_Edges", numEdges);
        createGraph(newGraph, numEdges, numNodes);
        DFS_ALL(newGraph,op);
        freeGraph(newGraph);
    }
    numEdges = 4500;
    for (numNodes = 100; numNodes <= 200; numNodes += 10) {
        Operation op = prof.createOperation("DFS_Vertices", numNodes);
        createGraph(newGraph, numEdges, numNodes);
        DFS_ALL(newGraph, op);
        freeGraph(newGraph);
    }

    prof.showReport();
}

int main()
{
    srand(time(NULL));
    demo();
    //perf();
}

