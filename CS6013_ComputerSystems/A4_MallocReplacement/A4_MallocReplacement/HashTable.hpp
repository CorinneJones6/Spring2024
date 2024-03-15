/**
 * @file HashTable.hpp
 * @brief Assignment 4: Malloc Replacement - Defines a HashTable class for managing memory allocations.
 *
 * The HashTable class is designed to track memory allocations and deallocations efficiently.
 * It supports operations such as insert, find, remove, and resize. The class uses a simple
 * probing mechanism for collision resolution and supports lazy deletion for removing elements.
 *
 * Created by Corinne Jones on 3/11/24.
 */

#pragma once

#include <iostream>
#include <sys/mman.h>
#include <stdio.h>
#include <unistd.h>

using namespace std;

/**
 * @brief A hash table for managing memory allocations.
 *
 * This class implements a hash table specifically designed for tracking allocated memory blocks.
 * It uses hashing to store and retrieve memory addresses and their associated sizes efficiently.
 */
class HashTable {

    /**
     * @brief A structure representing an entry in the hash table.
     */
    struct Entry {
        void* address; //pointer to where the allocated memory starts
        size_t memSize; //size of memory allocated
        bool isAvailable;//bool if the memory is available, used for lazy deletion
    };

    Entry* table;
    size_t capacity;
    size_t count;

    /**
     * @brief Computes a hash value for a given memory address.
     *
     * This hash function shifts the address by a fixed number of bits to the right,
     * 14 which equates to the size of a page (16), where the lower bits are less significant
     * for has distribution.
     *
     * @param address The memory address to hash.
     * @return size_t The hash value calculated by right-shifting the address.
     */
    size_t hash(void* address);
    
    /**
     * @brief Determines the next index to probe in the hash table.
     *
     * Implements a linear probing mechanism by incrementing the current index by one
     * and wrapping around the table using modulo arithmetic with the table's capacity.
     * This simple probing strategy ensures that all indices are visited systematically.
     *
     * @param i The current index at which a collision occurred.
     * @return size_t The next index to probe.
     */
    size_t probe(size_t i);
    
    /**
     * @brief Doubles the size of the hash table to accommodate more entries.
     *
     * When the table grows, it allocates a new array of entries twice the size of the old one
     * using mmap. It initializes all new entries as available and then rehashes and inserts all
     * entries from the old table into the new one. This method ensures the hash table maintains
     * its performance characteristics by keeping the load factor in check and reduces the likelihood
     * of collisions. The old table's memory is released using munmap.
     */
    void grow();
    
public:
    /**
     * @brief Default constructor for the HashTable.
     *
     * Initializes an empty hash table with a default initial capacity. The table is ready to track memory allocations immediately after construction.
     */
    HashTable();
    
    /**
     * @brief Parameterized constructor for the HashTable.
     *
     * Initializes an empty hash table with the specified initial capacity. This constructor allows for optimization based on the expected number of elements.
     * @param initialCapacity The initial capacity of the hash table.
     */
    HashTable(size_t initialCapacity);
    
    /**
     * @brief Destructor for the HashTable.
     *
     * Cleans up the hash table's resources, ensuring no memory leaks occur. It does not explicitly deallocate memory blocks tracked by the table, as it assumes these are managed externally.
     */
    ~HashTable();
    
    /**
     * @brief Gets the current capacity of the hash table.
     *
     * @return The capacity of the hash table.
     */
    size_t getCapacity();
    
    /**
     * @brief Gets the current number of elements in the hash table.
     *
     * @return The number of elements currently stored in the hash table.
     */
    size_t getCount();
    
    /**
     * @brief Retrieves the size of the memory block for a given address.
     *
     * If the address is not found, this function returns 0 or an indicative value. It's used to verify and manage the size of allocated blocks.
     * @param address The memory address to query.
     * @return The size of the memory block associated with the given address.
     */
    size_t getSize(void* address);
    
    /**
     * @brief Finds the index of the entry for a given memory address.
     *
     * This function returns the index of the entry within the hash table that matches the given memory address. If the address is not found, it returns an indicative value such as -1.
     * @param address The memory address to find.
     * @return The index of the entry, or an indicative value if not found.
     */
    size_t find(void* address);
    
    /**
     * @brief Inserts a new memory block into the hash table.
     *
     * This method adds a new entry for the given memory address and size. It handles collisions and may trigger a resize of the hash table if necessary.
     * @param address The memory address of the block to insert.
     * @param size The size of the memory block.
     * @return True if the insert was successful, false otherwise.
     */
    bool insert(void* address, size_t size);
    
    /**
     * @brief Removes a memory block from the hash table.
     *
     * This method removes the entry corresponding to the given memory address. It uses lazy deletion, marking the entry as available rather than erasing it.
     * @param address The memory address of the block to remove.
     * @return True if the removal was successful, false if the address was not found.
     */
    bool remove(void* address);
};
