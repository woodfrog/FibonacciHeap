#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "FibHeap_sp.h"

using namespace fib_heap; 
using namespace std;

#define NotAVertex -1
#define MaxVertexNum 10  /* maximum number of vertices */
#define INFI 10000

	
typedef int Vertex;      /* vertices are numbered from 0 to MaxVertexNum-1 */
typedef int WeightType;

typedef struct GNode *PtrToGNode;
struct GNode{
	int Nv;
	int Ne;
	WeightType G[MaxVertexNum][MaxVertexNum];
};

typedef PtrToGNode MGraph;
typedef struct Vnode *PtrToVNode;
struct Vnode{
	Vertex V;
	PtrToVNode Next;
};

MGraph ReadG(); 
void ShortestDist(MGraph Graph, int dist[], Vertex S);


MGraph ReadG()
{
	int numOfVertex, numOfEdge;
	cin >> numOfVertex;
	cin >> numOfEdge;
	MGraph MG = (MGraph)malloc(sizeof(struct GNode));
	MG->Nv = numOfVertex;
	MG->Ne = numOfEdge;
	int i, j;
	/* initial the matrix*/
	for (i = 0; i < numOfVertex; i++) // traversal rows
	{
		for (j = 0; j < numOfVertex; j++) // traversal cols
		{
			if (i == j)
				MG->G[i][j] = 0;
			else
				MG->G[i][j] = INFI;
		}
	}
	int tempVertex1, tempVertex2, tempDist;
	for (i = 0; i < numOfEdge; i++)   // store the info according to input
	{
		cin >> tempVertex1;
		cin >> tempVertex2;
		cin >> tempDist;
		MG->G[tempVertex1][tempVertex2] = tempDist;
	}
	return MG;
}



void ShortestDist(MGraph Graph, int dist[],  Vertex S)
{
	int i;
	Vertex V, W;
	FibHeap H; // create the Fibonacci heap
	FibHeapNode** nodes = new FibHeapNode*[Graph->Nv];

	dist[S] = 0;
	for (i = 0; i < Graph->Nv; i++) // insert all information about distance into the heap
	{
		if (i != S)
			dist[i] = INFI;
		nodes[i] = H.insert(dist[i]);
		nodes[i]->V = i;
	}

	for (;;)
	{
		FibHeapNode* min = H._extract_min_node();
		if (min == nullptr || min->key == INFI )
			break;
		V = min->V;
		for (W = 0; W < Graph->Nv; W++)
		{
			if ( W != V && Graph->G[V][W] != INFI )
			{
				if (dist[V] + Graph->G[V][W] < dist[W])
				{
					dist[W] = dist[V] + Graph->G[V][W];
					H.decrease_key(nodes[W], dist[W]);
				}
			}
		}
	}
	for (W = 0; W < Graph->Nv; W++)
		if ( dist[W] == INFI )
			dist[W] = -1;
}




int main()
{
	int dist[MaxVertexNum];
	Vertex S, V;
	cout << "Please input the information of the DAG, firstly the number of vertex and edges" 
		<< " then the information of edges, finally the start point" << endl;

	MGraph G = ReadG();
	cin >> S;
	ShortestDist(G, dist, S);

	for (V = 0; V<G->Nv; V++)
		printf("%d ", dist[V]);
	printf("\n");

	return 0;
}
