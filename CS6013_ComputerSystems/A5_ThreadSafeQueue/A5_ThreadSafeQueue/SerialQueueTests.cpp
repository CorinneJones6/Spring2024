
////////////////////////////////////////////////////////////////////////
//
// Author: Corinne Jones
// Date: 3/18/24
//
// CS 6013
//
// Tests for SerialQueue class.  
//
////////////////////////////////////////////////////////////////////////

#include <iostream>
#include "SerialQueue.h"

using namespace std;

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

bool testDynamicAllocationAndDeallocation() {
    SerialQueue<int>* queue = new SerialQueue<int>();

    queue->enqueue(1);
    queue->enqueue(2);

    delete queue;

    return true;
}

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

int main(int argc, const char * argv[]) {
    
    if(testStaticAllocation()){

        cout << "Static allocation test PASSED" << endl;
    }
    else {
        cerr << "Static allocation test FAILED" << endl;
    }
    
    if(testDynamicAllocationAndDeallocation()){

        cout << "Dynamic Allocation/Deallocation test PASSED" << endl;
    }
    else {
        cerr << "Dynamic Allocation/Deallocation test FAILED" << endl;
    }
    
    if(testEnqueueDequeue()){
        cout << "Enqueue/Dequeue test PASSED" << endl;
    }
    else {
        cerr << "Enqueue/Dequeue test FAILED" << endl;
    }
    
    return 0;
}
