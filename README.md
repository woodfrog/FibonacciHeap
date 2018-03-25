# Fibonacci Heap
## Introduction

Fibonacci Heap, a kind of priority queue, is wildly adopted to implement the famous Dijkstra's algorithm. When implementing Dijkstra's algorithm, **V times of insertion, V times of extracting minimum and E times of decreasing key** are needed (where E = the number of edge and V = the number of vertex). Binary Heap is used in common, which costs O(ElogV+VlogV) time. Using Fibonacci Heap can reduce the time complexity to O(E+V*logV) because its function **decrease_key** only costs O(1) amortized time. The following picture shows the time complexity of Fibonacci Heap compared with other priority queues. [1]

![](https://github.com/woodfrog/FibonacciHeap/blob/master/readme_pics/1.png?raw=true)


Fibonacci Heap is also **to some extent based on Binomial Heap**. The difference is that Fibonacci Heap **adopt the method of lazy-merge and lazy-insert**, which save a lot of potentials(the term in Amortized Analysis). Those saved potentials later reduce the time complexity for decrease_key and extract_min. The detailed analyze will be shown in Chapter4.

## Data Structure and Implementation
### 1. Overview
 This chapter talks about the structure of Fibonacci Heap. A Fibonacci Heap is also a collection of heap-ordered trees like a Binomial Heap. All of its roots are in a **circular linked list**, rather than an array. For any non-root node, it's also placed in a circular linked list with all of its siblings. The following two pictures visualize this data structure. [1] (a) shows what the heap looks like and (b) shows how it is implemented using pointers and circular linked list.
>![](https://github.com/woodfrog/FibonacciHeap/blob/master/readme_pics/2.png?raw=true)

### 2. Data Structure Abstraction
This section shows how a Fibonacci Heap is designed in code. The heap is abstracted as `class FibHeap` and its node is abstracted as `struct FibHeapNode`.

```cpp

/*File: FibHeap.h*/

struct FibHeapNode
{
    int key; // assume the element is int
    FibHeapNode* left;
    FibHeapNode* right;
    FibHeapNode* parent;
    FibHeapNode* child;
    int degree;
    bool mark;
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
    int extract_min(); 
    
    /* Decrease the key of node x to newKey*/   
    void decrease_key(FibHeapNode* x, int newKey);
    
    /*Delete a specified node*/
    void delete_node(FibHeapNode* x);
private:
	/*omitted, can be checked in source file*/
};

```

As shown in the code, a Fibonacci Heap has **a pointer to its minimum node**, and **an integer recording the amount of nodes**. For each node, it has pointers to its right and left sibling(s) as well as its child and parent. **A single node may have multiple children, but we only need to point to one of the them since they are all stored in a circular linked list. This is similar to the implementation of Binomial Heap**. Each node also has a degree showing how many children it has. The `mark` of the node enables Fibonacci Heap to have a perfect decrease_key function which costs only O(1) amortized time. 

### 3. Methods
#### a). `FibHeapNode* insert(int newKey);`

This method `insert()` is simply completed by creating a new node and adding it into the root linked list. This method accepts one input argument `newKey`, the key of the new node, and returns the pointer to the newly created node.

The method of **`insert()` is a lazy operation** to save potential for `decrease_key` and `extract_min`.

#### b). `void merge(FibHeap &another);`

The method `merge()` takes the reference of another `FibHeap` as the only argument and merges it with the current heap.

When merging H1 and H2, we only need to merge the two root lists and refresh the new minimum node. **`merge` is also an lazy operation.**

#### c). `int extract_min();`

`extract_min()` takes no arguments and returns the integer value of the minimum key.
    
This is the most expensive function in a Fibonacci Heap. In `insert()` and `merge()`, nodes are simply accumulated in the root list. The task of arrangement is deferred until `extract_min()` is called. In this method, the minimum node is deleted from the root list first, and its children are then put into the root list. Then all nodes in the root list are traversed and the nodes with the same degree are merged. This operation will not stop until all nodes in the root list have different degrees. The following 3 pictures show how `extract_min()` works.[1]
 ![](https://github.com/woodfrog/FibonacciHeap/blob/master/readme_pics/3.png?raw=true)
 ![](https://github.com/woodfrog/FibonacciHeap/blob/master/readme_pics/4.png?raw=true)
 ![](https://github.com/woodfrog/FibonacciHeap/blob/master/readme_pics/5.png?raw=true)

The following pseudo-code shows the detailed traversal process in `extract_min()` method.

        /**********************************************************************************
         * It can be proved that the node of Fibonacci Heap can have at most [logN] children
         * L is an array of pointer which point to trees
         * And L(R) is the pointer which points to a tree that has R children(child). 
         **********************************************************************************/
        for ( R = 0; R <= [logN]; R++)
            while |LR| >= 2
            {
                Remove two trees from L(R);
                Merge the two trees;
                Add the new tree into L(R+1);
            }


#### d). `void decrease_key(FibHeapNode* x, int newKey);`

This method takes the pointer to a `FibHeapNode` and an integer as input arguments. The integer is used as the new key of the node.
    
This method changes the key of the input node, cut this node from its parent and then add it into the root linked list. The mark of its parent needs to be evaluated. If the mark is true, which means the parent has lost one child before, the parent should be cut by the subroutine `cascading_cut()` and move it to the root list. This process will continue until we meet a node whose mark is false. The amount of nodes being cut is **1+c**, where c is the number execution of `cascading_cut()`.

#### e). `void delete_node(FibHeapNode* x);`

This methods takes the pointer to a `FibHeapNode` as the only input argument and then remove the input node from the heap.

## To Do
Analyze the time complexity of Fibonacci Heap in an **Amortized Analysis** method.

## Reference
[1] The pictures in this article are from [https://www.cs.princeton.edu/~wayne/teaching/fibonacci-heap.pdf](https://www.cs.princeton.edu/~wayne/teaching/fibonacci-heap.pdf).