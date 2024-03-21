#pragma once

////////////////////////////////////////////////////////////////////////
//
// Author: Corinne Jones
// Date: 3/18/24
//
// CS 6013
//
// Tests for SerialQueue class.  Fill in the missing data, comments, etc.
//
////////////////////////////////////////////////////////////////////////

#include <iostream>
#include "SerialQueue.h"

using namespace std;

void testStaticAllocation() {
    SerialQueue<int> queue;

    cout << "Testing static allocation... ";
    
    queue.enqueue(1);
    queue.enqueue(2);

    int result;
    bool dequeued = queue.dequeue(&result);

    if (!dequeued || result != 1 || queue.size() != 1) {
        cerr << "Static allocation test failed.\n";
        exit(1);
    }

    cout << "Passed\n";
}

void testDynamicAllocationAndDeallocation() {
    std::cout << "Testing dynamic allocation and deallocation... ";
    SerialQueue<int>* queue = new SerialQueue<int>();

    // Perform operations
    queue->enqueue(1);
    queue->enqueue(2);

    delete queue; // Test for memory leaks or errors upon deletion

    cout << "Passed\n";
}

void testEnqueueDequeue() {
    SerialQueue<std::string> queue;

    cout << "Testing enqueue and dequeue... ";
    queue.enqueue("First");
    queue.enqueue("Second");

    string result;
    bool dequeuedFirst = queue.dequeue(&result);
    bool dequeuedSecond = queue.dequeue(&result);

    if (!dequeuedFirst || result != "Second" || !dequeuedSecond || queue.size() != 0) {
        std::cerr << "Enqueue/Dequeue test failed.\n";
        exit(1);
    }

    cout << "Passed\n";
}

int main(int argc, const char * argv[]) {
    
    testStaticAllocation();
    testDynamicAllocationAndDeallocation();
    testEnqueueDequeue();
    
    return 0;
}
