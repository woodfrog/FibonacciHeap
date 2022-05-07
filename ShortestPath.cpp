#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <iostream>
#include "FibHeap.h"

using namespace fib_heap; 
using namespace std;

#define INFI 1e9

	
typedef int Vertex;      /* vertices are numbered from 0 to MaxVertexNum-1 */
typedef int WeightType;


int main()
{
	ios::sync_with_stdio(false);
	cin.tie(0);
	Vertex Vnum, Enum; // The number of vertexes and edges
	cout << "Please input the information of the DAG, firstly the number of vertex and edges" 
		<< " then the information of edges, finally the start point" << endl;
	cin >> Vnum >> Enum;

	vector<vector<pair<int, int>>> G(Vnum);

	for (int i = 0; i < Enum; i++){
		Vertex start, end;
		WeightType weight;
		cin >> start >> end >> weight;
		G[start].emplace_back(end, weight);
	}

	vector<FibHeapNode*> ptr(Vnum, nullptr);
	FibHeap fibheap;
	
	int Startid;
	cin >> Startid;

	for (int i = 0; i < Vnum; i++){
		if(i == Startid){
			ptr[i] = fibheap.insert(0);
			ptr[i]->id = i;
			continue;
		}
		ptr[i] = fibheap.insert(INFI);
		ptr[i]->id = i;
	}

	int cnt = 0;
	vector<bool> vis(Vnum, 0);
	vector<int> dis(Vnum, INFI);

	while(cnt < Vnum){
		int v = fibheap.m_minNode->id;
		int Dis = fibheap.extract_min();
		dis[v] = Dis;
		vis[v] = 1;
		for (auto [end, weight]: G[v]){
			if(!vis[end]){
				if(weight + Dis < ptr[end]->key){
					fibheap.decrease_key(ptr[end], weight + Dis);
				}
			}
		}
		cnt++;
	}
	for (int i = 0; i < Vnum; i++){
		cout << dis[i] << " \n"[i == Vnum - 1];
	}
	return 0;
}
