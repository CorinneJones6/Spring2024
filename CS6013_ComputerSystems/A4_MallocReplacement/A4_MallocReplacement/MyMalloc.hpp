/**
 * @file MyMalloc.cpp
 * @brief Assignment 4: Malloc Replacement - Implementation of the MyMalloc class for custom memory allocation.
 *
 * Provides detailed implementations of the MyMalloc class methods, including custom memory allocation
 * and deallocation. The class leverages system calls like mmap and munmap for memory management and
 * maintains a hash table to track allocated blocks. This implementation aims to provide a basic
 * understanding of how memory allocation works beneath higher-level abstractions.
 *
 * Created by Corinne Jones on 3/11/24.
 */

#pragma once
#include <stdio.h>
#include "HashTable.hpp"
#include <unistd.h>

class MyMalloc {

    HashTable hashTable;
    
public:
    
    /**
     * @brief Default constructor initializing the MyMalloc with default hash table size.
     */
    MyMalloc() : hashTable() {}

    /**
     * @brief Parameterized constructor to specify initial capacity of the internal hash table.
     *
     * @param initialCapacity The initial capacity of the hash table used for memory block tracking.
     */
    MyMalloc(size_t initialCapacity) : hashTable(initialCapacity) {}
    
    ~MyMalloc(){}

    /**
     * @brief Allocates a block of memory of the specified size.
     *
     * @param bytesToAllocate The number of bytes to allocate.
     * @return void* Pointer to the allocated memory block.
     */
    void* allocate( size_t bytesToAllocate );
    
    /**
     * @brief Deallocates a previously allocated block of memory.
     *
     * @param ptr Pointer to the memory block to be deallocated.
     */
    void deallocate( void* ptr );

};
