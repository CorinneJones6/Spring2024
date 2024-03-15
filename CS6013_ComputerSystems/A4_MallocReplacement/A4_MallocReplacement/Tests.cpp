/**
 * @file Tests.cpp
 * @brief Assignment 4: Malloc Replacement - Test suite for the custom memory allocator MyMalloc and the supporting HashTable.
 *
 * This file includes various tests to evaluate the performance, correctness,
 * and efficiency of the custom memory allocation implementation (MyMalloc)
 * and its underlying data structure (HashTable). Tests include timed allocations/deallocations,
 * data integrity checks, and HashTable operation verifications.
 *
 * Created by Corinne Jones on 3/11/24.
 */

#include "HashTable.hpp"
#include "MyMalloc.hpp"
#include <cassert>
#include <cstring>
#include <ctime>

using namespace std;

//=================== GLOBAL VARIABLES ===================//

MyMalloc myMalloc;

//void* malloc(size_t size){
//    return myMalloc.allocate(size);
//}
//
//void free(void* ptr){
//    myMalloc.deallocate(ptr);
//}

//=================== TIMED TESTS ===================//

/**
 * @brief Tests allocation and deallocation time for small objects.
 *
 * Allocates, manipulates, and then deallocates a large number of small objects,
 * measuring the time taken for these operations. This test provides insights into
 * the allocator's performance for small-size memory allocations.
 */
void testSmallObj() {
    vector<void*> smallObjs;
    
    const int NUM_OBJS = 10000;

    const int SMALL_SIZE = 128;

    clock_t start_time_total = clock();
    clock_t start_time_alloc = clock();
    
    for (int i = 0; i < NUM_OBJS; ++i) {
        void* ptr = malloc(SMALL_SIZE);
        if (ptr) {
            // Manually manipulate data
            unsigned char* bytePtr = static_cast<unsigned char*>(ptr); // Cast to unsigned char* for bit manip
            for (size_t j = 0; j < SMALL_SIZE; ++j) {
                bytePtr[j] = 0xab;
            }
            smallObjs.push_back(ptr);
        }
    }

    clock_t end_time_alloc = clock();
    
    clock_t start_time_dealloc = clock();
//    cout << "before free : " << myMalloc.hashTable.getCount()<< "\n";
    
    //Verify and deallocate
    for (void* ptr : smallObjs) {
        free(ptr);
    }
    
//    cout << "after free : " << myMalloc.hashTable.getCount()<< "\n";
    clock_t end_time_total = clock();
    clock_t end_time_dealloc = clock();
    
    // Calculate the time used
    double time_used_alloc = static_cast<double>(end_time_alloc - start_time_alloc);
    double time_used_dealloc = static_cast<double>(end_time_dealloc - start_time_dealloc);
    double time_used_total = static_cast<double>(end_time_total - start_time_total);
     
    // Convert to ms
    double cpu_time_in_ms_alloc = (time_used_alloc / CLOCKS_PER_SEC) * 1000;
    double cpu_time_in_ms_dealloc = (time_used_dealloc / CLOCKS_PER_SEC) * 1000;
    double cpu_time_in_ms_total = (time_used_total / CLOCKS_PER_SEC) * 1000;
    
    cout << "Small Object Allocation time: " << cpu_time_in_ms_alloc << " ms\n";
    cout << "Small Object Deallocation time: " << cpu_time_in_ms_dealloc << " ms\n";
    cout << "Small Object Total time: " << cpu_time_in_ms_total << " ms\n";
}

/**
 * @brief Tests allocation and deallocation time for large objects.
 *
 * Similar to testSmallObj, but for large objects. This test is designed to evaluate
 * the allocator's efficiency and performance when handling large-size memory allocations.
 */
void testLargeObj() {
    
    vector<void*> largeObjs;
    const int NUM_OBJS = 10000;
    const int LARGE_SIZE = 1024 * 1024;

    clock_t start_time_total = clock();
    clock_t start_time_alloc = clock();
    
    for (int i = 0; i < NUM_OBJS; ++i) {
        void* ptr = malloc(LARGE_SIZE);
        if (ptr) {
            // Manually manipulate data
            unsigned char* bytePtr = static_cast<unsigned char*>(ptr); // Cast to unsigned char* for bit manip
            for (size_t j = 0; j < LARGE_SIZE; ++j) {
                bytePtr[j] = 0xab;
            }
            largeObjs.push_back(ptr);
        }
    }
    
    clock_t end_time_alloc = clock();
    clock_t start_time_dealloc = clock();
    
//    cout << "before free : " << myMalloc.hashTable.getCount()<< "\n";
    
    //Verify and deallocate
    for (void* ptr : largeObjs) {
        free(ptr);
    }
    
//    cout << "after free : " << myMalloc.hashTable.getCount()<< "\n";
    
    clock_t end_time_total = clock();
    clock_t end_time_dealloc = clock();
    
    // Calculate the time used
    double time_used_alloc = static_cast<double>(end_time_alloc - start_time_alloc);
    double time_used_dealloc = static_cast<double>(end_time_dealloc - start_time_dealloc);
    double time_used_total = static_cast<double>(end_time_total - start_time_total);
     
    // Convert to ms
    double cpu_time_in_ms_alloc = (time_used_alloc / CLOCKS_PER_SEC) * 1000;
    double cpu_time_in_ms_dealloc = (time_used_dealloc / CLOCKS_PER_SEC) * 1000;
    double cpu_time_in_ms_total = (time_used_total / CLOCKS_PER_SEC) * 1000;
    
    cout << "Large Object Allocation time: " << cpu_time_in_ms_alloc << " ms\n";
    cout << "Large Object Deallocation time: " << cpu_time_in_ms_dealloc << " ms\n";
    cout << "Large Object Total time: " << cpu_time_in_ms_total << " ms\n";
}

//=================== DATA CHECK TESTS ===================//

/**
 * @brief Verifies the data integrity of allocated small objects.
 *
 * Allocates multiple small objects, writes specific patterns to them, and then verifies
 * that the data remains intact. This test checks for any data corruption issues.
 */
void testSmallObjData() {
    vector<void*> pointers1;
    vector<void*> pointers2;
    
    const int NUM_OBJS_1 = 10000;
    const int NUM_OBJS_2 = 5000;
    
    const int SMALL_SIZE = 128;

    //Allocate + manipulate pointer1
    for (int i = 0; i < NUM_OBJS_1; ++i) {
        void* ptr = malloc(SMALL_SIZE);
        if (ptr) {
            unsigned char* bytePtr = static_cast<unsigned char*>(ptr); // Cast to unsigned char* for bit manip
            for (size_t j = 0; j < SMALL_SIZE; ++j) {
                bytePtr[j] = 0xab;
            }
            pointers1.push_back(ptr);
        }
    }

    //Allocate + manipulate pointer2
    for (int i = 0; i < NUM_OBJS_2; ++i) {
        void* ptr = malloc(SMALL_SIZE);
        if (ptr) {
            unsigned char* bytePtr = static_cast<unsigned char*>(ptr); // Cast to unsigned char* for bit manip
            for (size_t j = 0; j < SMALL_SIZE; ++j) {
                bytePtr[j] = 0xcd;
            }
            pointers2.push_back(ptr);
        }
    }

    //Check for pointers1 valid data
    for (void* ptr : pointers1) {
        unsigned char* data = reinterpret_cast<unsigned char*>(ptr);
        for (int i = 0; i < SMALL_SIZE; ++i) {
            if (data[i] != 0xab) {
                cout << "DATA CORRUPTION, SMALL BATCH, POINTERS1." << endl;
                break;
            }
        }
    
        free(ptr);
    }

    //Check for pointers2 valid data
    for (void* ptr : pointers2) {
        unsigned char* data = reinterpret_cast<unsigned char*>(ptr);
        for (int i = 0; i < SMALL_SIZE; ++i) {
            if (data[i] != 0xcd) {
                cout << "DATA CORRUPTION, SMALL BATCH, POINTERS2." << endl;
                break;
            }
        }
        free(ptr);
    }
    
    cout << "Small Object Data Test Completed." << endl;
}

/**
 * @brief Verifies the data integrity of allocated large objects.
 *
 * Performs allocations, manipulations, and verifications similar to testSmallObjData,
 * but for large objects. This ensures the allocator's reliability for large-size allocations.
 */
void testLargeObjData() {
    
    vector<void*> pointers1;
    vector<void*> pointers2;

    const int NUM_OBJS_1 = 10;
    const int NUM_OBJS_2 = 5;

    const int LARGE_SIZE = 1024 * 1024;

    //Allocate + manipulate pointers1
    for (int i = 0; i < NUM_OBJS_1; ++i) {
        void* ptr = malloc(LARGE_SIZE);
        if (ptr) {
            unsigned char* bytePtr = static_cast<unsigned char*>(ptr); // Cast to unsigned char* for bit manip
            for (size_t j = 0; j < LARGE_SIZE; ++j) {
                bytePtr[j] = 0xab;
            }
            pointers1.push_back(ptr);
        }
    }

    //Allocate + manipulate pointers2
    for (int i = 0; i < NUM_OBJS_2; ++i) {
        void* ptr = malloc(LARGE_SIZE);
        if (ptr) {
            unsigned char* bytePtr = static_cast<unsigned char*>(ptr); // Cast to unsigned char* for bit manip
            for (size_t j = 0; j < LARGE_SIZE; ++j) {
                bytePtr[j] = 0xcd;
            }
            pointers2.push_back(ptr);
        }
    }

    //Check for pointers1 valid data
    for (void* ptr : pointers1) {
        unsigned char* data = reinterpret_cast<unsigned char*>(ptr);
        for (int i = 0; i < LARGE_SIZE; ++i) {
            if (data[i] != 0xab) {
                cout << "DATA CORRUPTION, LARGE BATCH, POINTERS1." << endl;
                break;
            }
        }
        free(ptr);
    }

    //Check for pointers2 valid data
    for (void* ptr : pointers2) {
        unsigned char* data = reinterpret_cast<unsigned char*>(ptr);
        for (int i = 0; i < LARGE_SIZE; ++i) {
            if (data[i] != 0xcd) {
                cout << "DATA CORRUPTION, LARGE BATCH, POINTERS2." << endl;
                break;
            }
        }
        free(ptr);
    }

    cout << "Large Object Data Test Completed." << endl;
}

//==================== HASHTABLE TESTS ====================//

/**
 * @brief Tests the insert function of the HashTable.
 *
 * Ensures that insertion operations increase the count, respect capacity limits,
 * and handle duplicates correctly.
 */
void testInsert() {
    HashTable table(3);

    int key1 = 1;
    size_t size1 = 100;
    assert(table.insert(&key1, size1) == true && "Insert key1");
    assert(table.getCount() == 1);

    int key2 = 2;
    size_t size2 = 200;
    assert(table.insert(&key2, size2) == true && "Insert key2");
    assert(table.getCount() == 2);

    int key3 = 3;
    size_t size3 = 300;
    assert(table.insert(&key3, size3) == true && "Insert key3");
    assert(table.getCount() == 3);
    assert(table.getCapacity() == 3);
    
    int key4 = 4;
    size_t size4 = 400;
    table.insert(&key4, size4);
    assert(table.getCount() == 4 && "Insert key4");
    assert(table.getCapacity() == 6 && "Insert key4 triggers grow");
    
    assert(table.insert(&key4, size4) == false && "Duplicate key insertion fails");
    assert(table.getCount() == 4 && "Count did not change with duplicate insertion");

    cout << "testInsert: Passed\n";
}


/**
 * @brief Tests the remove function of the HashTable.
 *
 * Verifies that removing entries decreases the count appropriately and that
 * attempts to remove non-existent entries are handled correctly.
 */
void testRemove() {
    HashTable table(10);

    int key1 = 1;
    size_t size1 = 50;
    table.insert(&key1, size1);

    assert(table.getCount()==1);
    assert(table.remove(&key1) == true && "Remove existing key");
    
    assert(table.getCount()==0);
    assert(table.remove(&key1) == false && "Remove non-existent key");

    cout << "testRemove: Passed\n";
}

/**
 * @brief Tests the find function and probing mechanism of the HashTable.
 *
 * Checks if the find function can accurately locate entries and evaluates
 * the probing mechanism's effectiveness in handling collisions.
 */
void testFindAndProbe() {
    HashTable table(10);

    int key1 = 1;
    size_t size1 = 100;
    table.insert(&key1, size1);

    size_t foundIndex1 = table.find(&key1);
    assert(foundIndex1 != -1 && "Find existing key");

    int key2 = 2;
    assert(table.find(&key2) == -1 && "Find non-existent key");
    
    table.insert(&key2, size1);
    size_t foundIndex2 = table.find(&key2);
    
    assert( foundIndex2!=-1 && "Ensure collison provides new index");
    assert(foundIndex1 != foundIndex2 && "Two keys at different places");

    cout << "testFindAndProbe: Passed\n";
}

/**
 * @brief Entry point for running all HashTable-specific tests.
 */
void runHashTableTests(){
    testInsert();
    testRemove();
    testFindAndProbe();
}

/**
 * @brief Entry point for running timed allocation/deallocation tests.
 */
void runTimedTests(){
    testSmallObj();
    testLargeObj();
}

/**
 * @brief Entry point for running data integrity tests.
 */
void runDataCheckTests(){
    testSmallObjData();
    testLargeObjData();
}

int main() {
    
    // Entry point for running all test suites.
    runHashTableTests();
    runTimedTests();
    runDataCheckTests();
    
    return 0;
}
