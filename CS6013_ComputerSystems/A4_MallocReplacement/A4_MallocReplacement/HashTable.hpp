//
//  HashTable.hpp
//  A4_MallocReplacement
//
//  Created by Corinne Jones on 3/11/24.
//

#pragma once

#include <iostream>
#include <sys/mman.h>
#include <stdio.h>
#include <unistd.h>

using namespace std; 

class HashTable {

    //struct to create each entry into the hash table
    struct Entry {
        
        //pointer to where the allocated memory starts
        void* address;

        //size of memory allocated
        size_t memSize;
        
        //bool if the memory is available, used for lazy deletion
        bool isAvailable;
    };

    Entry* table;
    size_t capacity;
    size_t count;

    size_t hash(void* address);
    size_t probe(size_t i);
    void grow();
    
public:
    //constructors and destructor
    HashTable();
    HashTable(size_t initialCapacity);
    ~HashTable();

    size_t getSize(void* address);
    size_t find(void* address);
    bool insert(void* address, size_t size);
    bool remove(void* address);
};
