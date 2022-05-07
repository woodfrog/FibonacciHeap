#ifndef _FIB_HEAP_H
#define _FIB_HEAP_H

#include <cmath>
#include <vector>
using namespace std;

namespace fib_heap{


struct FibHeapNode
{
	int key; // assume the element is int...
	FibHeapNode* left;
	FibHeapNode* right;
	FibHeapNode* parent;
	FibHeapNode* child;
	int degree;
	bool mark;
	int id; // the vertex id
};

		
class FibHeap {
public:
	FibHeapNode* m_minNode;
	int m_numOfNodes;

	FibHeap(){  // initialize a new and empty Fib Heap
		m_minNode = nullptr;
		m_numOfNodes = 0;
	}

	~FibHeap() {
		_clear(m_minNode);
	}

	/* Insert a node with key value new_key
	   and return the inserted node*/
	FibHeapNode* insert(int newKey);

	/* Merge current heap with another*/  	
	void merge(FibHeap &another); 

	/* Return the key of the minimum node*/				
	int  extract_min(); 

	/* Decrease the key of node x to newKey*/					
	void decrease_key(FibHeapNode* x, int newKey);

	/*Delete a specified node*/
	void delete_node(FibHeapNode* x);

// private:
	static const int m_minimumKey; 
	FibHeapNode* _create_node(int newKey);
	void _insert_node(FibHeapNode* newNode);
	void _remove_from_circular_list(FibHeapNode* x);
	FibHeapNode* _merge(FibHeapNode* a, FibHeapNode* b);
	void _make_child(FibHeapNode *child, FibHeapNode *parent);
	void _consolidate();
	void _unparent_all(FibHeapNode* x);
	FibHeapNode* _extract_min_node();
	void _decrease_key(FibHeapNode* x, int newKey);
	void _cut(FibHeapNode* x, FibHeapNode* y);
	void _cascading_cut(FibHeapNode* y);
	void _clear(FibHeapNode* x);
};


}

#endif