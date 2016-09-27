# Fibonacci Heap

### Reference
Pictures in this article are from [https://www.cs.princeton.edu/~wayne/teaching/fibonacci-heap.pdf](https://www.cs.princeton.edu/~wayne/teaching/fibonacci-heap.pdf).

## Introduction


First of all, to introduce this data structure, we should clarify its usage. Fibonacci Heap is wildly adopted to implement the famous Dijkstra's algorithm because it has a good performance on the function **decrease_key**, which only costs O(1) amortized time. And we know that to implement Dijkstra's algorithm, **V times of insertion, V times of extracting minimum and E times of decreasing key** are needed (E = the number of edges, V = the number of vertex). Therefore, using Fibonacci Heap can reduce the time complexity to O(E+V*logV) while Binary Heap costs O(ElogV+VlogV). The following is the time complexity of Fibonacci Heap compared with other priority queues. And in next chapter we are going to discuss its structure and implementation.

![](https://github.com/woodfrog/FibonacciHeap/blob/master/readme_pics/1.png?raw=true)


Secondly, Fibonacci Heap is **to some extent based on Binomial Heap**. The difference is that Fibonacci Heap **adopt the method of lazy-merge and lazy-insert**, which save a lot of potential(the term of Amortized Analysis). And those saved potential later provide a fantastic amortized time complexity for decrease_key and extract_min. The detailed analyze will be shown in Chapter4. And in next chapter, we are going to introduce the structure and functions first.

##Data Structure and Algorithm Specification
### Overview
 Firstly, we give an overview of the whole structure of  Fibonacci Heap.It's also a collection of heap-ordered trees like Binomial Heap. And all of the roots are in a **circular linked list** rather than an array. And for any non-root node, it's also placed in a circular linked list with all of its siblings. To be more detailed, we use the two pictures to show the implementation of the structure. **(a) is what the heap looks like, and (b) is how it is implemented using pointers and circular linked list. The circular linked list is quite obvious in (b).** 
>![](https://github.com/woodfrog/FibonacciHeap/blob/master/readme_pics/2.png?raw=true)

###Component of Structure
**The second part is to show the component of the structure. We now show the definition of class FibHeap.**

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
    int  extract_min(); 
    
    /* Decrease the key of node x to newKey*/   
    void decrease_key(FibHeapNode* x, int newKey);
    
    /*Delete a specified node*/
    void delete_node(FibHeapNode* x);
private:
	/*omitted, can be checked in source file*/
};

```

As shown in the code, a Fibonacci Heap has **a pointer to the minimum node**, and **an integer recording the amount of nodes**. And for each node, it has pointers to its right/left sibling as well as to its child and parent.**A single node may have many children, but since all children are stored in a circular linked list, we only need a pointer to one of the children to access all of them. This is similar to the implementation of Binomial Heap**. And each node also has a degree to represent how many children it has. At last, each node's mark enables Fibonacci Heap to have a perfect decrease_key function which costs only O(1) amortized time. The importance of **mark** will be explained later when we analyze the time complexity.  

###Functions' Algorithm
- Insertion

    Insertion is simply completed by **adding the new node into the root linked list**.

- Merge 

    Merge is also easy to fulfill. If we need to merge H1 and H2, we only merge the two root lists and refresh the new minimum node. Actually, as is mentioned in Chapter1, Insert and Merge are both **lazy-operation in Fibonacci Heap which save potential for later operations including decrease_key and extract_min**.

- Extract_min
    
    This is the most expensive function in Fibonacci Heap. As we can see, in Insertion and Merge, nodes are simply accumulated in the root list. The task of arrangement is deferred until Extract_min is called. In this function, the minimum node is deleted from the root list first, and its children are put into the root list. Then all nodes in the root list are traversed and the nodes with the same degree are merged. The above operation will not stop until all nodes in the root list have different degree with each other. The following 3 pictures show the effect of Extract_min function, detailed process of traversal and merging are displayed using pseudo-code.
 ![](https://github.com/woodfrog/FibonacciHeap/blob/master/readme_pics/3.png?raw=true)
 ![](https://github.com/woodfrog/FibonacciHeap/blob/master/readme_pics/4.png?raw=true)
 ![](https://github.com/woodfrog/FibonacciHeap/blob/master/readme_pics/5.png?raw=true)


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


- Decrease_key
    
    This function firstly change the key of the specified node, and cut the node from its current parent and add it into the root linked list. However, it's not the end. **We should judge the mark of its parent, if the mark is true, which means the parent has lost one child before, then we should cut the parent using a subroutine named cascading_cut and move the parent to root list too. The process will continue until we meet a node whose mark is false.** Therefore, the amount of nodes being cut is **1+c**, where c is the number of cascading_cut being executed.

Up to now, we have briefly introduced the data structure and algorithm of Fibonacci Heap. **Later we are going to use amortized analysis to analyze its time complexity.** 