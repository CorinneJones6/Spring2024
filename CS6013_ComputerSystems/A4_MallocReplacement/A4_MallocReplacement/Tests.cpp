//
//  Tests.cpp
//  A4_MallocReplacement
//
//  Created by Corinne Jones on 3/11/24.
//
#include "HashTable.hpp"
#include <cassert>
#include <cstring>

using namespace std;

//void testFind() {
//
//    assert(add(2, 3) == 5);
//    assert(add(-1, -1) == -2);
//    cout << "All find() tests passed.\n";
//}

void testInsert() {
    HashTable table(4); // Small initial capacity to test growth

    int key1 = 1;
    size_t size1 = 100;
    assert(table.insert(&key1, size1) == true && "Insert key1");

    int key2 = 2;
    size_t size2 = 200;
    assert(table.insert(&key2, size2) == true && "Insert key2");

    // Insert until it grows
    int key3 = 3;
    size_t size3 = 300;
    assert(table.insert(&key3, size3) == true && "Insert key3 triggers grow");

    cout << "testInsert: Passed\n";
}

void testRemove() {
    HashTable table(10);

    int key1 = 1;
    size_t size1 = 50;
    table.insert(&key1, size1);

    assert(table.remove(&key1) == true && "Remove existing key");
    assert(table.remove(&key1) == false && "Remove non-existent key");

    cout << "testRemove: Passed\n";
}

void testFindAndProbe() {
    HashTable table(10);

    int key1 = 1;
    size_t size1 = 100;
    table.insert(&key1, size1);

    size_t foundIndex = table.find(&key1);
    assert(foundIndex != static_cast<size_t>(-1) && "Find existing key");

    int key2 = 11; // Assuming this causes a collision due to the hash function
    assert(table.find(&key2) == static_cast<size_t>(-1) && "Find non-existent key");

    cout << "testFindAndProbe: Passed\n";
}

int main() {
    testInsert();
    testRemove();
    testFindAndProbe();
    
    return 0;
}
