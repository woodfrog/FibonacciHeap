#include "FibHeap.h"

namespace fib_heap {

const int FibHeap::m_minimumKey = 0x80000000; // the minimum int value

FibHeapNode* FibHeap::insert(int newKey)
{
	FibHeapNode* newNode = _create_node(newKey);
	_insert_node(newNode);
	return newNode;
}

void FibHeap::merge(FibHeap &another)
{
	m_minNode = _merge(m_minNode, another.m_minNode);
	m_numOfNodes += another.m_numOfNodes;
	another.m_minNode = nullptr; // so that another 
	another.m_numOfNodes = 0;
}

int  FibHeap::extract_min()
{
	FibHeapNode* minNode = _extract_min_node();
	int ret = minNode->key;
	delete minNode;
	return ret;
}

void FibHeap::decrease_key(FibHeapNode* x, int newKey)
{
	_decrease_key(x, newKey);
}

void FibHeap::delete_node(FibHeapNode* x)
{
	_decrease_key(x, m_minimumKey);
	extract_min();
}

FibHeapNode* FibHeap::_create_node(int newKey)
{
	FibHeapNode* newNode = new FibHeapNode;
	newNode->key = newKey;
	newNode->left = newNode;
	newNode->right = newNode;
	newNode->parent = nullptr;
	newNode->child = nullptr;
	newNode->degree = 0;
	newNode->mark = false;
	return newNode;
}


void FibHeap::_insert_node(FibHeapNode* newNode)
{
	m_minNode = _merge( m_minNode, newNode);
	m_numOfNodes++;
}


/*******************************************************************
* Remove x from its circular list
* Without changing the content of x
* Without freeing x's memory space
*******************************************************************/
void FibHeap::_remove_from_circular_list(FibHeapNode* x)
{
    if (x->right == x)
        return;
    FibHeapNode* leftSib = x->left;
    FibHeapNode* rightSib = x->right;
    leftSib->right = rightSib;
    rightSib->left = leftSib;
    x->left = x->right = x;
}


FibHeapNode* FibHeap::_merge(FibHeapNode* a, FibHeapNode* b)
{
	if(a == nullptr)
		return b;
	if(b == nullptr)	
		return a;
	if( a->key > b->key ) // swap node 
	{
		FibHeapNode* temp = a;
		a = b;
		b = temp;
	}
	FibHeapNode* aRight = a->right;
	FibHeapNode* bLeft	= b->left;
	a->right = b;
	b->left = a;
	aRight->left = bLeft;
	bLeft->right = aRight;
	return a;	
}


/***********************************************************
 * Rearrange the heap 
 * Update the m_minNode
 * Return the current minimum node 
***********************************************************/
FibHeapNode* FibHeap::_extract_min_node()
{
	FibHeapNode* mn = m_minNode;
    if (mn == nullptr){
        return nullptr;
    }
    _unparent_all(mn->child);
    _merge(mn, mn->child);
    if (mn == mn->right){
        m_minNode = nullptr;
    }else{
        m_minNode = mn->right;
    }
    _remove_from_circular_list(mn);
    if (m_minNode != nullptr){
        _consolidate();
    }
    m_numOfNodes--;
    return mn;
}

/*make all nodes' parent nullptr in a circular list*/
void FibHeap::_unparent_all(FibHeapNode* x)
{
	if(x == nullptr)
		return;
	FibHeapNode* y = x;
	do {
		y->parent = nullptr;
		y = y->right;
	}while(y != x);
}


void FibHeap::_consolidate()
{
    int Dn = (int)(log2(m_numOfNodes) / log2(1.618));
    FibHeapNode** A = new FibHeapNode*[Dn + 1];
    for(int i = 0; i < Dn + 1; i++){
        A[i] = nullptr;
    }
    vector<FibHeapNode*> nodeList; // It needs optimization! It makes the time longer now.
    auto node = m_minNode;
    do{
        nodeList.emplace_back(node);
        node = node->right;
    } while (node != m_minNode);
    for (auto e: nodeList){
        int d = e->degree;
        _remove_from_circular_list(e);
        while(A[d] != nullptr){
            auto tmp = A[d];
            if (e->key > tmp->key){
                swap(e, tmp);
            }
            _make_child(tmp, e);
            A[d++] = nullptr;
        }
        A[e->degree] = e;
        m_minNode = e;
    }
    for (int i = 0; i < Dn + 1; i++){
        if (A[i] != nullptr && A[i] != m_minNode){
            _merge(m_minNode, A[i]);
        }
    }
    FibHeapNode* flag = m_minNode;
    FibHeapNode* iter = flag;
    do{
        if (iter->key < m_minNode->key){
            m_minNode = iter;
        }
        iter = iter->right;
    } while (iter != flag);
    delete []A;
}


void FibHeap::_make_child(FibHeapNode *child, FibHeapNode *parent)
{
    child->parent = parent;
    parent->child = _merge(parent->child, child);
    parent->degree++;
    child->mark = false;
}

void FibHeap::_decrease_key(FibHeapNode* x, int newKey)
{
	x->key = newKey;
	FibHeapNode* y = x->parent;
	if ( y != nullptr && x->key < y->key )
	{
		_cut(x, y);
		_cascading_cut(y);
	}
	if (x->key < m_minNode->key)
		m_minNode = x;
}


/***********************************************************************
* Remove x from the child list of y, decrement y->degree
* Add x to the root list, make its parent NULL
* And clear the mark of x
***********************************************************************/
void FibHeap::_cut(FibHeapNode* x, FibHeapNode* y)
{
    y->child = (x == x->right ? nullptr : x->right);
    _remove_from_circular_list(x);
    y->degree--;
    _merge(m_minNode, x);
    x->parent = nullptr;
    x->mark = false;
}

/***********************************************************************
* Continue cutting on the path from the decreased node to the root
	Until meet one node, which is a root or is unmarked
***********************************************************************/
void FibHeap::_cascading_cut(FibHeapNode* y)
{
	FibHeapNode* z = y->parent;
	if ( z != nullptr)
	{
		if( y->mark == false)
			y->mark = true;	
		else
		{
			_cut(y,z);
			_cascading_cut(z);
		}
	}
}


/*********************************************************************
* t1 is used to traversal the circular list. 
  When t1 == x for the second time (the first time is at t1's initialization),
  t1 has completed the traversal.
**********************************************************************/
void FibHeap::_clear(FibHeapNode* x)
{
	if ( x != nullptr )
	{
		FibHeapNode* t1 = x;  
		do{
			FibHeapNode* t2 = t1; 
			t1 = t1->right;
			_clear(t2->child);
			delete t2;
		} while(t1 != x); 
	}
}



}






