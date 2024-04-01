////////////////////////////////////////////////////////////////////////
//
// Author: Corinne Jones
// Date: 4/1/24
//
// CS 6013
//
// Tests for ConcurrentQueue class.
// 1. Compile via Cmdline: clang++ -std=c++17 ConcurrentQueueTests.cpp -o executable
// 2. Run executable: ./executable <num1><num1><num3>
//
////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <thread>
#include "ConcurrentQueue.h"
using namespace std;

//Helper function for producers
void producerAction(ConcurrentQueue<int>& queue, int numInts) {
    for (int i = 0; i < numInts; ++i) {
        queue.enqueue(i);
    }
}

// Helper function for consumers
void consumerAction(ConcurrentQueue<int>& queue, int numInts) {
    int value;
    for (int i = 0; i < numInts; ++i) {
        queue.dequeue(&value);
    }
}

bool testQueue( int num_producers, int num_consumers, int num_ints ) {

    vector<thread> threads;

    int space = num_producers + num_consumers;

    threads.reserve(space);

    ConcurrentQueue<int> queue;

    // Create num_producer producer threads that enqueue num_ints ints into the ConcurrentQueue
     for (int i = 0; i < num_producers; ++i) {
         threads.push_back(thread(producerAction, ref(queue), num_ints));
     }

     // Create num_consumer consumer threads that dequeue num_ints ints from the ConcurrentQueue
     for (int i = 0; i < num_consumers; ++i) {
         threads.push_back(thread(consumerAction, ref(queue), num_ints));
     }

    //Wait for all threads to join
    for(thread& thread : threads){
        thread.join();
    }

    //Return true if the number of elements in the queue matches (num_producers - num_consumers)*num_ints, false otherwise
    return queue.size() == (num_producers - num_consumers) * num_ints;
}

int main(int argc, char** argv){
    
    //Pass cmdline arguements is as params for testQueue
    int num_producers = stoi(argv[1]);
    int num_consumers = stoi(argv[2]);
    int num_ints = stoi(argv[3]);
    
    if(testQueue(num_producers, num_consumers, num_ints)){
        //Tests pass if no locking, freezing, and the test returns true
        cout << "Tests PASSED" << endl;
    }
    else {
        cerr << "Tests FAILED" << endl;
    }
    
    return 0;
}
