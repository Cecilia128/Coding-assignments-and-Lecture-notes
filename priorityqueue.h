#ifndef PRIORITYQUEUE_H
#define PRIORITYQUEUE_H
#include <functional>
#include <vector>
#include <iostream>
/*
 * PriorityQueue < T, Comparator >
 * --------------------------------------------------------
 * A binary heap based pritority queue.
 * - You should not use any heap related operations provided
 *   in < algorithm >
 * - You should not use ` std::priority_queue ` directly.
 * - The Comparator is provided as a template argument; we
 *   also provide a helper function to should how to use it.
 * - The "higher" its priority with respect to the Comparator,
 *   the ealier the element should be popped from the queue.
 */
template < typename T, typename Comparator = std::less<T> >
class PriorityQueue
{
public:
    // we do not have self-mananged heap memory, therefore
    // default constructor and destructor is okay.
    PriorityQueue () = default;
    ~PriorityQueue () = default;

    /*
     * Copy Constructor
     * -----------------------------------------------------
     * Copy construct a new priority queue. You should write
     * it correctly on yourself.
     */
    PriorityQueue ( const PriorityQueue& that );


    /*
     * Copy Assignment Operator
     * -------------------------------------------------------
     * Copy Assignment the priority queue. You should write it
     * correctly on yourself.
     */
    PriorityQueue & operator= ( const PriorityQueue& that );

    /*
     * void push ( const T& element )
     * -------------------------------------------------------
     * Push a new element to the priority queue.
     */
    void push ( const T& element );

    /*
     * T pop ( )
     * -------------------------------------------------------
     * Pop the element with the highest priority in the queue.
     * and return it;
     * No need for checking whether the queue is empty.
     */
    T pop ( );

    /*
     * const T& top () const
     * -------------------------------------------------------
     * Return a reference to the element with the highest
     * priority.
     * No need for checking whether the queue is empty.
     */
    const T& top () const;

    /*
     * bool empty () const
     * -------------------------------------------------------
     * Check whether the queue is empty
     */
    bool empty () const;

    /*
     * void clear ()
     * -------------------------------------------------------
     * Clear all elements in the queue
     */
    void clear ();

    /*
     * size_t size () const
     * -------------------------------------------------------
     * Return the size of the queue.
     */
    size_t size () const;

    /*
     * int parent(int index);
     * -------------------------------------------------------
     * Return the index of parent node of current node
     */
    int parent(int index);

    /*
     * int left(int index);
     * -------------------------------------------------------
     * Return the index of left child of current node
     */
    int left(int index);

    /*
     * int right(int index);
     * -------------------------------------------------------
     * Return the index of right child of current node
     */
    int right(int index);

private:

    // we use `std::vector` to represet the binary heap
    std::vector< T > heap;

    /*
     * static inline bool compare(const T& a, const T& b)
     * --------------------------------------------------
     * Compare two operands using the comparator.
     * - Return true if a has a higher priority (or equal)
     *   than b
     * - Otherwise, return false.
     */
    static inline bool compare(const T& a, const T& b) {
        return Comparator {} ( b, a );
    }
};

template<typename T, typename Comparator>
PriorityQueue< T, Comparator >::PriorityQueue(const PriorityQueue &that)
{
   if (this != &that) {
       clear();
       int length = that.heap.size();
       for (int i=0; i<length; i++) {
           heap.push_back(that.heap[i]);
       }
   }
}

template<typename T, typename Comparator>
PriorityQueue< T, Comparator > &PriorityQueue< T, Comparator >::operator=(const PriorityQueue &that)
{
    int length = that.heap.size();
    for (int i=0; i<length; i++) {
        heap.push_back(that.heap[i]);
    }
    return *this;
}

template<typename T, typename Comparator>
void PriorityQueue< T, Comparator >::push(const T &element)
{
   heap.push_back(element);
   int index = heap.size();
   // when the priority of the added element is higher than its parent, exchange the position
   while (index>1 && compare(heap[index-1],heap[parent(index)-1])) {
       T temp = heap[index-1];
       heap[index-1] = heap[parent(index)-1];
       heap[parent(index)-1] = temp;
       index = parent(index);
   }
}

template<typename T, typename Comparator>
T PriorityQueue< T, Comparator >::pop()
{
    T highest = heap[0];
    // replace the root with the last element
    heap[0] = heap[size()-1];
    // remove the last element in heap
    heap.pop_back();
    int index = 1;
    // when current node has both left child and right child
    while (right(index) <= size() && (!compare(heap[index-1],heap[left(index)-1]) || !compare(heap[index-1],heap[right(index)-1]))) {
        // largest is index of the element with higher priority among current node and its left and right child
        int largest = index;
        if (!compare(heap[index-1],heap[left(index)-1])) largest = left(index);
        if (!compare(heap[largest-1],heap[right(index)-1])) largest = right(index);
        if (largest != index) {
            T temp = heap[index-1];
            heap[index-1] = heap[largest-1];
            heap[largest-1] = temp;
            index = largest;
        }
    }
    // when current node has only left child
    if (left(index) == size()) {
        if (!compare(heap[index-1],heap[left(index)-1])){
            T temp = heap[index-1];
            heap[index-1] = heap[left(index)-1];
            heap[left(index)-1] = temp;
        }
    }


    return  highest;
}

template<typename T, typename Comparator>
const T &PriorityQueue< T, Comparator >::top() const
{
    return heap[0];
}

template<typename T, typename Comparator>
bool PriorityQueue< T, Comparator >::empty() const
{
    return heap.empty();
}

template<typename T, typename Comparator>
void PriorityQueue< T, Comparator >::clear()
{
   heap.clear();
}

template<typename T, typename Comparator>
size_t PriorityQueue< T, Comparator >::size() const
{

    return heap.size();
}

template<typename T, typename Comparator>
int PriorityQueue<T, Comparator>::parent(int index) {
    return index/2;
}

template<typename T, typename Comparator>
int PriorityQueue<T, Comparator>::left(int index) {
    return 2*index;
}

template<typename T, typename Comparator>
int PriorityQueue<T, Comparator>::right(int index) {
    return 2*index+1;
}

#endif // PRIORITYQUEUE_H
