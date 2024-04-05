/**
 * @file ConcurrentQueue.h
 * @brief Implements a thread safe ConcurrentQueue suitable for multi-threaded environments.
 *
 * The ConcurrentQueue class provides a thread-safe queue implementation. It ensures thread safety by using mutexes during enqueue and dequeue operations. This class enqueue's and dequeue's, both of which are thread-safe.
 *
 * @tparam T Type of the elements stored in the queue.
 * @note This class uses mutexes and locks for thread safety.
 * @author Corinne Jones
 * @date 4/1/24
 * @course CS 6013
 *
 */
#include <mutex>

using namespace std;

template <typename T>
class ConcurrentQueue {

public:
    ConcurrentQueue() :
      head_( new Node{ T{}, nullptr } ), size_( 0 ) {
      tail_ = head_;
   }

    /**
     * @brief Adds a new node at the tail of the linked list.
     *
     * This method creates a new Node object that contains the data provided by the user
     * and appends it to the end of the queue. It uses a mutex to ensure thread-safe access to the tail of the queue.
     *
     * @param x A constant reference to the data to be added to the queue.
     */
   void enqueue( const T & x ) {
       Node* newNode = new Node{x, nullptr};
       {
           unique_lock<mutex>lock(tailMutex_);
           
           tail_->next = newNode;
           tail_ = newNode;
       }
       
       size_++;
       
   }
    
    /**
     * @brief Removes and returns the front node from the queue.
     *
     * This method removes the node at the head of the queue and returns its data. It uses a mutex to ensure thread-safe access to the head of the queue.
     * 1. If the queue is empty (i.e., `head_->next` is `nullptr`), the function returns false and does not modify the value pointed by `ret`.
     * 2. Otherwise, it updates the head of the list to the next node, effectively removing the current head.
     * 3. If the removed node was also the tail of the list, it adjusts the tail to point to the head (indicating an empty list).
     *
     * @param ret Pointer to a variable of type `T` where the data of the removed element (if removed) will be stored.
     * @return `true` if an element was successfully removed from the queue, otherwise `false`.
     */
    bool dequeue( T * ret ) {
        //If the size is 0, can't dequeue
        if(head_->next == nullptr){
            return false;
        }
        
        Node* oldNode;
        
        {
            unique_lock<mutex>lock(headMutex_);
            
            oldNode = head_->next;
            *ret = oldNode->data;
            
            head_->next = oldNode->next;
            
            //If the size is 1
            if(tail_ == oldNode){
                tail_ = head_;
            }
            
        }
        
        size_--;

        delete oldNode;
        
        return true;
    }

    /**
     * @brief Destructor for the SerialQueue class.
     *
     * Cleans up the resources used by the ConcurrentQueue. It iteratively deletes all the elements in the queue.
     *
     */
   ~ConcurrentQueue() {

      while( head_ != nullptr ) {
         Node* temp = head_->next;
         delete head_;
         head_ = temp;
      }
   }

   int size() const { return size_; }

private:

    struct Node {
      T      data;
      Node * next;
    };

    Node * head_;
    Node * tail_;
    atomic<int>    size_; // Size is a critical section. This allows for size to be safely                   // incremented / decremented without a race.
    mutex headMutex_; // Used for synchronizing acces to the head of the queue.
    mutex tailMutex_; // Used for synchronizing access to the tial of the queue.
};
