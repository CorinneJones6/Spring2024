/**
 * @file SerialQueueTest.cpp
 * @brief Tests for SerialQueue class
 *
 * Provides a series of tests for the SerialQueue class.
 * Tests include static allocation, dynamic allocation and deallocation, and basic enqueue and dequeue operations.
 *
 * @author Corinne Jones
 * @date 3/18/24
 * @course CS 6013
 */


#include <iostream>
#include "SerialQueue.h"

using namespace std;

/**
 * @brief Test the static allocation of SerialQueue
 *
 * Tests static allocation of a SerialQueue by enqueuing two elements,
 * then dequeuing one and checking the size and correctness of the operation.
 *
 * @return true if the test passes (correct dequeue operation and size), false otherwise.
 */
bool testStaticAllocation() {
    SerialQueue<int> queue;
    
    queue.enqueue(1);
    queue.enqueue(2);

    int result;
    bool dequeued = queue.dequeue(&result);

    if (!dequeued || result != 1 || queue.size() != 1) {
        return false;
    }

    return true;
}

/**
 * @brief Test dynamic allocation and deallocation of SerialQueue
 *
 * Tests the dynamic allocation and deallocation of a SerialQueue by
 * allocating it on the heap, enqueuing two elements, and then immediately deleting the queue.
 *
 * @note This test does not strictly check for memory leaks; it assumes correct deletion behavior.
 *
 * @return true, as the test focuses on memory allocation/deallocation without checking operation correctness.
 */
bool testDynamicAllocationAndDeallocation() {
    SerialQueue<int>* queue = new SerialQueue<int>();

    queue->enqueue(1);
    queue->enqueue(2);

    delete queue;

    return true;
}

/**
 * @brief Test enqueuing and dequeuing operations
 *
 * Tests the SerialQueue's enqueue and dequeue operations by enqueuing two strings,
 * then dequeuing them in order and checking the correctness of the operations and the queue size.
 *
 * @return true if both dequeue operations are correct and the queue is empty at the end, false otherwise.
 */
bool testEnqueueDequeue() {
    SerialQueue<string> queue;

    queue.enqueue("First");
    queue.enqueue("Second");

    string result;
    bool dequeuedFirst = queue.dequeue(&result);
    bool dequeuedSecond = queue.dequeue(&result);

    if (!dequeuedFirst || result != "Second" || !dequeuedSecond || queue.size() != 0) {
        return false;
    }

    return true;
}

/**
 * @brief Main function to run SerialQueue tests
 *
 * Executes each of the test functions and reports their status.
 *
 * @return int Status code (0 for success, non-zero for failure)
 */
int main(int argc, const char * argv[]) {
    
    if(testStaticAllocation()){

        cout << "Static allocation test PASSED" << endl;
    }
    else {
        cerr << "Static allocation test FAILED" << endl;
        return -1;
    }
    
    if(testDynamicAllocationAndDeallocation()){

        cout << "Dynamic Allocation/Deallocation test PASSED" << endl;
    }
    else {
        cerr << "Dynamic Allocation/Deallocation test FAILED" << endl;
        return -1;
    }
    
    if(testEnqueueDequeue()){
        cout << "Enqueue/Dequeue test PASSED" << endl;
    }
    else {
        cerr << "Enqueue/Dequeue test FAILED" << endl;
        return -1;
    }
    
    return 0;
}
