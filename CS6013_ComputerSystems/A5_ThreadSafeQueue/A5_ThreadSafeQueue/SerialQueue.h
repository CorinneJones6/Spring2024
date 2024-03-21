#pragma once

////////////////////////////////////////////////////////////////////////
//
// Author: Corinne Jones
// Date: 3/18/24
//
// CS 6013
//
// Outline for SerialQueue class.  Fill in the missing data, comments, etc.
//
////////////////////////////////////////////////////////////////////////

template <typename T>
class SerialQueue {

public:
   SerialQueue() :
      head_( new Node{ T{}, nullptr } ), size_( 0 ) {
      tail_ = head_;
   }

   void enqueue( const T & x ) {
       Node* newNode = new Node{x, nullptr};
       
       if(head_ == tail_){
           head_->next = newNode;
           tail_ = newNode;
       }
       else {
           tail_->next = newNode;
           tail_ = newNode;
       }
       
       size_++;
   }

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
