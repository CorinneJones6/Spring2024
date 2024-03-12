//
//  MyMalloc.hpp
//  A4_MallocReplacement
//
//  Created by Corinne Jones on 3/11/24.
//

#pragma once
#include <stdio.h>
#include "HashTable.hpp"

class MyMalloc {

    HashTable hashTable;
    
public:
    //constructor and destructor
    MyMalloc(size_t initialCapacity) : hashTable(initialCapacity) {
        
    }
    
    ~MyMalloc();

    void* allocate( size_t bytesToAllocate );
    
    void deallocate( void* ptr );

};
