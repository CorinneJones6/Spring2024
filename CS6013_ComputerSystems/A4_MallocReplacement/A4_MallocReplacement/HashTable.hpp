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

class HashTable {

    //struct to create each entry into the hash table
    struct Entry {
        
        //pointer to where the allocated memory starts
        void* key;

        //size of memory allocated
        size_t size;
        
        // 
        bool isAvailable;
        
    };

    //MV that is the hash table
    Entry* table;

    //capacity of the hash table
    size_t capacity;

    //num of elems in the hash table
    size_t count;

    //hash method to find a spot to place an entry
    size_t hash(void* key);

    //method to linearly probe the hash table
    size_t probe(size_t hash, size_t i);
    
    

    //method to grow the hash table when necessary
    void grow();
    
public:
    //constructor and destructor
    HashTable(size_t initialCapacity);
    ~HashTable();

    size_t find(void* key);
    //public methods to insert, remove, and find elems in the hash table
    bool insert(void* key, size_t size);
    bool remove(void* key);
};
