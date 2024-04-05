/**
 * @file SerialQueue.h
 * @brief Implementation for a serial queue.
 *
 * The SerialQueue class provides a single-ended queue implementation. The outline was provided
 * and then was filled in by Corinne.
 *
 *
 * @tparam T Type of the elements stored in the queue.
 * @author Corinne Jones
 * @date 3/18/24
 * @course CS 6013
 *
 */

#pragma once

template <typename T>
class SerialQueue {

public:
   SerialQueue() :
      head_( new Node{ T{}, nullptr } ), size_( 0 ) {
      tail_ = head_;
   }
    
    /**
     * @brief Adds a new node at the tail of the linked list.
     *
     * This method creates a new Node object that contains the data provided by the user
     * and appends it to the end of the queue.
     *
     * @param x A constant reference to the data to be added to the queue.
     */
   void enqueue( const T & x ) {
       Node* newNode = new Node{x, nullptr};
       
       tail_->next = newNode;
       tail_ = newNode;
       
       size_++;
   }
    
    /**
     * @brief Removes a new node at the head of the linked list.
     *
     * This method removes the node at the head of the linked list (queue) and returns its value.
     * 1. If the queue is empty (i.e., `head_->next` is `nullptr`), the function returns false and does not modify the value pointed by `ret`.
     * 2. Otherwise, it updates the head of the list to the next node, effectively removing the current head.
     * 3. If the removed node was also the tail of the list, it adjusts the tail to point to the head (indicating an empty list).
     *
     * @param ret Pointer to a variable of type `T` where the data of the removed element (if removed) will be stored.
     * @return `true` if an element was successfully removed from the queue, otherwise `false`.
     */
   bool dequeue( T * ret ) {
       int oldSize = size_;
       
       if(head_->next == nullptr){
           return false;
       }
       
       Node* oldNode = head_->next;
       *ret = oldNode->data;
       
       head_->next = oldNode->next;
       
       if(tail_ == oldNode){
           tail_ = head_;
       }
      
       delete oldNode;
       size_--;
       
       int newSize = size_;
       
       return newSize < oldSize;
   }
    
    /**
     * @brief Destructor for the SerialQueue class.
     *
     * Cleans up the resources used by the SerialQueue. It iteratively deletes all the elements in the queue.
     *
     */
   ~SerialQueue() {

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
   int    size_;
};
