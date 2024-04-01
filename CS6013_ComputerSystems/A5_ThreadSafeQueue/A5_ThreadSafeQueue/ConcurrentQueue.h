////////////////////////////////////////////////////////////////////////
//
// Author: Corinne Jones
// Date: 4/1/24
//
// CS 6013
//
// Thread safe ConcurrentQueue.
//
////////////////////////////////////////////////////////////////////////
///
#include <mutex>

using namespace std;

template <typename T>
class ConcurrentQueue {

public:
    ConcurrentQueue() :
      head_( new Node{ T{}, nullptr } ), size_( 0 ) {
      tail_ = head_;
   }

   void enqueue( const T & x ) {
       Node* newNode = new Node{x, nullptr};

       {
           unique_lock<mutex>lock(m_);
        
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
   }

   void dequeue( T * ret ) {

       Node* oldNode;
       {
           unique_lock<mutex>lock(m_);
           
           oldNode = head_->next;
           
           *ret = oldNode->data;
           
           head_->next = oldNode->next;
           
           if(tail_ == oldNode){
               tail_ = head_;
           }
           
           size_--;
       }
       
       delete oldNode;

   }

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
    int    size_;
    
    std::mutex m_;
};
