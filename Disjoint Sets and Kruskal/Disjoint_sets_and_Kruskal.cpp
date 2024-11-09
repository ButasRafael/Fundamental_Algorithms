/*
* First i will talk about the algorithms and then about their complexities.
* The make set algorithm creates each individual set, making the representatives of each set the single element in it(by putting the parent of x being x).

* The Link algorithm links two sets(merge them), but in a specific way: always link the set with the smaller height(rank) to the one with the larger height.
* We link them this way to not increase the final height after the merge of the sets(it will remain the height of the larger one).
* If the heights are equal, we increase the rank by one and we dont care about which side we link to the other one.

* The Find Set algorithm finds for any element the represantative(root) of the sets in which it belongs.
* Until the parent of an element is not the element itself(the proprety of the representative) we go up in the set.
* This algorithm also uses path compression to reduce the complexity.
* Path compression makes the following thing: when we call find set for an element, we make the elements that we go through until we reach the representative point to the representative.
* We to this because when we call find set again for an element of the same set(which is above the first one), it will find the representative instantly.
 
* The Union algorithm unites 2 sets by calling find set for the two parameters to find their representatives and then calling link on that two representatives.
* Also, before we call the functions we check if the representatives are not equal(the elements are in the same set),case in which we dont have to unite anything.
 
* The Kruskal algorithm creates a minimum spanning tree(the shorthest path that containts all the nodes) from a weighted undirected and connected graph.
* We first order the edges ascending by their weights. Then for each node we create a set using make set.
* After that, for each edge if the x and y nodes are in different sets(we call find set to find out, this means that the edge doesnt close a cycle) we add the set in the MST.
* After adding we need to increase the number of edges in MST and unite the two sets in which the nodes belonged because when we check another edge we need to have the nodes and their sets updated to make sure we dont close a cycle in the future.
* If the edges in the MST reaches the number of nodes-1 we stop by breaking the repetitive structure because we completet our MST.
 
* For counting the operations, i made a function that creates a random graph by creating edges with random x, y and weight.
* I made sure that the edges created are not loop egdges(the x and the y are not equal) and are not duplicates(i made and adjacency matrix and marked the spot [x][y] and [y][x] of the created edge to avoid reapeating it and to make the graph undirected).
* I also made sure that the graph is connected by creating the first (number of nodes-1) edges in different way: one node of the edge, lets say x, will be 1,2,....nodes-1 and the other node of the edge is chosen randomly in the range [0,x-1].
* I chose the first node in this way to make sure that i dont have isolated nodes, case in which MST cannot be created because the graph is not connected.
* I chose the second node randomly in the range [0,x-1] to make sure that i dont have loop edges or duplicates and to keep the random aspect valid.
 
* Now, lets talk about efficency and complexities.

* The make set operation is done in O(1), more accurate O(3) because we just create a set from a node.

* The Link operation is done also in O(1) because we just link the root with the smaller rank to one with the larger rank.
* More accurately, if the ranks of the reperesnatives are not equal, the link operation is done in O(2) and if they are equal, it will be done in O(4) because we also need to update the rank.
* So the complexity of Link operation is smaller or equal to O(4) which is O(1).
 
* The Find Set operation is done in O(h) where h is the height of the path needed to find the representative and its maximum log(n).
* Also, because of path compression, the complexity of Find Set operation for an element which belongs to a set that has already been traveled becomes O(1) because the element is already linked to the representative.
 
* So the complexity of Union operation is also constant because it just uses Link and Find Set which are both constant.

* That being said, lets compute the time and space complexity of Kruskal algorithm. For that, denote the number of edges with E and the number of vertices with V.
*
* The complexity of sorting the edges of the graph is E*log(E).
* The complexity of creating V sets is O(V) since make set is linear.
* Also, because we breake the for if we reached V-1, we do the check of the sets and the union V times.
* Because the check and the union have log(V) complexity(before path compression) the overall complexity of this part is O(V*log(V)).
* So the total complexity remains E*log(E) but beacuse E<V^2, log(E)<2*log(V) and we can say that the complexity of the Kruskal algorithm is E*log(V).
* In our case, because we used V as the size and E as 4*size, the complexity should be between 4*size*log(size) and 8*size*log(size) but in practice its less because of path compression and union by rank.

* Also, the space complexity of the Kruskal algorithm is O(V+E) because we need to keep track of all the vertices of the graph and their sets and of all the edges of the graph to choose the valid ones and put them into the solution.
*/

#include <iostream>
#include "Profiler.h"
#include <cstdlib>
#include <ctime>
#include <algorithm> 
using namespace std;
Profiler prof("Sets");

typedef struct Node {
	Node* parent;
	int value;
	int rank;
}node;

typedef struct Edge {
	int x;
	int y;
	int weight;
}edge;

void Make_Set(node*& x, int value, Operation& comp, Operation& assig)
{
    x = (node*)malloc(sizeof(node));
    assig.count(3);
    x->parent = x;
    x->rank = 0;
    x->value = value;
}


void Link(node* x, node* y, Operation& comp, Operation& assig)
{
    comp.count();
    if (x->rank > y->rank)
    {
        assig.count();
        y->parent = x;
    }
    else
    {
        assig.count();
        x->parent = y;
        comp.count();
        if (x->rank == y->rank)
        {
            y->rank++;
            assig.count();
        }
    }
}

node* Find_Set(node* x, Operation& comp, Operation& assig)
{
    comp.count();
    if (x != x->parent)
    {
        assig.count();
        x->parent = Find_Set(x->parent, comp, assig);
    }
    return x->parent;
}

void Union(node* x, node* y, Operation& comp, Operation& assig)
{
    comp.count();
    if(Find_Set(x, comp, assig)!=Find_Set(y, comp, assig))
        Link(Find_Set(x, comp, assig), Find_Set(y, comp, assig), comp, assig);
}

struct Edge* kruskal(edge* edges, int numEdges, int numVertices, int& numMSTEdges, Operation& comp, Operation& assig)
{
    edge* sol = (edge*)malloc((numVertices - 1) * sizeof(edge));

    numMSTEdges = 0;
    node** sets = (node**)malloc(numVertices * sizeof(node*));

    sort(edges, edges + numEdges, [](const edge& a, const edge& b) {
        return a.weight < b.weight;
        });

    for (int i = 0; i < numVertices; i++)
        Make_Set(sets[i], i,comp,assig);

    for (int i = 0; i < numEdges; i++)
    {

        if (Find_Set(sets[edges[i].x],comp,assig) != Find_Set(sets[edges[i].y],comp,assig))
        {
            sol[numMSTEdges] = edges[i];
            numMSTEdges++;
            Union(sets[edges[i].x], sets[edges[i].y], comp, assig);
        }
        if (numMSTEdges == numVertices - 1)
            break;
    }

    for (int i = 0; i < numVertices; i++)
    {
        free(sets[i]);
    }
    free(sets);
    return sol;
}

bool is_edge_present(int** adjacencyMatrix, int x, int y) 
{
    return adjacencyMatrix[x][y] || adjacencyMatrix[y][x];
}

void free_adjacency_matrix(int** adjacencyMatrix, int numVertices) 
{
    for (int i = 0; i < numVertices; ++i) 
    {
        free(adjacencyMatrix[i]);
    }
    free(adjacencyMatrix);
}


void generate_edges(edge* edges, int numEdges, int numVertices) 
{
    int** adjacencyMatrix = (int**)malloc(numVertices * sizeof(int*));
    for (int i = 0; i < numVertices; ++i) 
    {
        adjacencyMatrix[i] = (int*)calloc(numVertices, sizeof(int));
    }

    for (int i = 1; i < numVertices; i++)
    {
        int randVertex = rand() % i;
        edges[i - 1].x = randVertex;
        edges[i - 1].y = i;
        edges[i - 1].weight = rand() % 10000 + 1;
        adjacencyMatrix[randVertex][i] = 1;
        adjacencyMatrix[i][randVertex] = 1;
    }

    for (int i = numVertices-1; i < numEdges; i++)
    {
        do 
        {
            edges[i].x = rand() % numVertices;
            edges[i].y = rand() % numVertices;
        } while (edges[i].x == edges[i].y || is_edge_present(adjacencyMatrix, edges[i].x, edges[i].y));

        edges[i].weight = rand() % 10000 + 1;
        adjacencyMatrix[edges[i].x][edges[i].y] = 1;
        adjacencyMatrix[edges[i].y][edges[i].x] = 1;
    }

    free_adjacency_matrix(adjacencyMatrix, numVertices);
}

void demo_sets()
{
    int N = 10;
    Operation comp = prof.createOperation("Comparisons_Dummy", N);
    Operation assig = prof.createOperation("Assignments_Dummy", N);
    node** sets = (node**)malloc(N * sizeof(node*));
    for (int i = 0; i < N; i++)
    {
        Make_Set(sets[i], i, comp, assig);
    }

    cout << "This are the sets: " << endl << endl;
    for (int i = 0; i < N; i++)
    {
        cout << "Set " << sets[i]->value << " has representative " << Find_Set(sets[i], comp, assig)->value << endl;

    }

    cout << endl;
    cout << "This are the sets after union: " << endl << endl;
    Union(sets[0], sets[2], comp, assig);
    Union(sets[1], sets[3], comp, assig);
    Union(sets[0], sets[1], comp, assig);
    Union(sets[4], sets[6], comp, assig);
    Union(sets[8], sets[9], comp, assig);
    Union(sets[7], sets[8], comp, assig);

    for (int i = 0; i < N; i++)
    {
        cout << "Set " << sets[i]->value << " has representative " << Find_Set(sets[i], comp, assig)->value << endl;
    }
    cout << endl;
    for (int i = 0; i < N; i++) 
    {
        free(sets[i]);
    }
    free(sets);

}

void demo_kruskal() {
    int numEdges = 10;
    edge* edges = (edge*)malloc(numEdges * sizeof(edge));
    int numVertices = 5;
    int numMSTEdges;
    Operation comp = prof.createOperation("Comparisons_Dummy", numEdges);
    Operation assig = prof.createOperation("Assignments_Dummy", numEdges);

    generate_edges(edges, numEdges, numVertices);
    cout << "Randomly generated edges: " << endl << endl;
    for (int i = 0; i < numEdges; ++i) 
    {
        cout << "Edge " << i + 1 << ": " << edges[i].x << " - " << edges[i].y << " (Weight: " << edges[i].weight << ")" << endl;
    }

    edge* sol = kruskal(edges, numEdges, numVertices, numMSTEdges, comp, assig);

    cout << "\nEdges in the Minimum Spanning Tree: " << endl << endl;
    for (int i = 0; i < numMSTEdges; i++) 
    {
        cout << "Edge " << i + 1 << ": " << sol[i].x << " - " << sol[i].y << " (Weight: " << sol[i].weight << ")" << endl;
        
    }

    free(sol);
    free(edges);
}

void perf()
{
    int increment = 100;
    int start_size = 100;
    int end_size = 10000;
    int number_exp = 5;
    int numEdges, numVertices, numMSTEdges;
    for (int experiment = 0; experiment < number_exp; experiment++)
    {
        for (int size = start_size; size <= end_size; size += increment)
        {
            Operation comp = prof.createOperation("Comparisons_Kruskal", size);
            Operation assig = prof.createOperation("Assignments_Kruskal", size);
            numEdges = 4 * size;
            numVertices = size;
            edge* edges = (edge*)malloc(numEdges * sizeof(edge));
            generate_edges(edges, numEdges, numVertices);
            kruskal(edges, numEdges, numVertices, numMSTEdges,comp,assig);
            free(edges);
            cout << size << " ";
        }
    }
    prof.divideValues("Comparisons_Kruskal", number_exp);
    prof.divideValues("Assignments_Kruskal", number_exp);
    prof.addSeries("Operations_Kruskal", "Assignments_Kruskal", "Comparisons_Kruskal");
    prof.showReport();

}


int main()
{
    srand(time(NULL));
    demo_sets();
    demo_kruskal();
    //perf();
    return 0;
}

