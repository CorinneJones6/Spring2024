
/**
 * @file ConcurrentQueueTests.cpp
 * @brief Tests for the ConcurrentQueue class.
 *
 * Provides a set of tests to verify the thread safety of the ConcurrentQueue class.
 * These tests include multiple producers and consumers to simulate concurrent access to the queue.
 *
 * Instructions for compiling and running:
 * 1. Compile via Cmdline: clang++ -std=c++17 ConcurrentQueueTests.cpp -o executable
 * 2. Run executable: ./executable <num_producers> <num_consumers> <num_ints>
 *
 * @author Corinne Jones
 * @date 4/1/24
 * @course CS 6013
 */

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

/**
 * @brief Tests the ConcurrentQueue with a specified number of producers and consumers.
 *
 * This function initializes a ConcurrentQueue and spawns a number of producer and consumer threads.
 * Each producer thread enqueues a set number of integers, and each consumer thread dequeues a set number of integers.
 * The function then waits for all threads to complete and checks if the final size of the queue is as expected.
 *
 * @param num_producers The number of producer threads to create.
 * @param num_consumers The number of consumer threads to create.
 * @param num_ints The number of integers each producer/consumer will enqueue/dequeue.
 * @return true if the final size of the queue matches the expected value, false otherwise.
 */
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
