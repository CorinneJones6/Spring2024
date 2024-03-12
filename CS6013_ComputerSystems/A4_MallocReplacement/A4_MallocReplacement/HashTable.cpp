//
//  HashTable.cpp
//  A4_MallocReplacement
//
//  Created by Corinne Jones on 3/11/24.
//
#include "HashTable.hpp"

//private methods
size_t HashTable::hash(void *key) {
    //A reasonable hash function for pointers is x >> VirtualAddressOffsetSizeInBits
    int VirtualAddressOffsetSizeInBits = 0;
    
    for(int i=1; i < sizeof(void*); i*=2){
        VirtualAddressOffsetSizeInBits++;
    }
    
    return reinterpret_cast<size_t>(key) >> VirtualAddressOffsetSizeInBits;
}

size_t HashTable::probe(size_t hash, size_t i) {
    
    return (hash + i) / capacity;
}

size_t HashTable::find(void *key) {
    
    size_t i = hash(key);
    
    i %= capacity;
    
    int count = 0;
    
    while(table[i].key != key){
        i = (i+1) % capacity;
        count++;
        
        if(count >= capacity){
            return -1; //flag for index not found
        }
    }
    
    return i;
}

void HashTable::grow() {
    //store the old variables
    size_t oldCapacity = capacity;
    
    Entry* oldTable = table;
    
    // Update member variables
    capacity = oldCapacity * 2;
    
    table = (Entry*)mmap(NULL, capacity * sizeof(Entry), PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    
    for (size_t i = 0; i < capacity; i++) {
        table[i].isAvailable = true;
    }
    
    // Rehash entries from old table to new one
       for(size_t i = 0; i < capacity; i++) {
           if (table[i].isAvailable) {
               table[i]=oldTable[i];
           }
       }
       
       // Free old table???????
       munmap(oldTable, oldCapacity * sizeof(Entry));
}

//public methods
HashTable::HashTable(size_t initialCapacity) {
    capacity = initialCapacity;
    count = 0;
    
    //param 1: NULL - starting address of memory; I don't care where so I'll let the computer decide
    //param 2: length of memory to be allocated; capacity * sizeof(Entry) gives me the size of one entry * how many entries I can have
    //param 3: protection flags for memory area; I want to be able to read and write from the area, combine the flags with bitwise operator |
    //param 4: mapping flags; MAP_PRIVATE because I want the memory to be exclusive to this program, MAP_ANONYMOUS because the memory is not backed by a file
    //param 5 and 6: standards entries for when the memory is not backed by a file
    table = (Entry*)mmap(NULL, capacity * sizeof(Entry), PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    
    //initialize the table with empty values/not occupied
       for (size_t i = 0; i < capacity; i++) {
           table[i].isAvailable = true;
       }
}

HashTable::~HashTable() {
    munmap(table, capacity * sizeof(Entry));
}

bool HashTable::insert(void *key, size_t size) {
    
    size_t oldCount = count;

    if(count/capacity > .7){
        grow();
    }
    
    //Get the index and put it within the capacity
    size_t i = hash(key);
    
    i = i % capacity;
    
    while(!table[i].isAvailable){
        //handle if the key exists
        if(table[i].key == key) {
            return false;
        }
        i = (i+1) % capacity;
    }
    
    // if available, assigns new values
    table[i].key=key;
    table[i].size=size;
    table[i].isAvailable = false;
    count++;
    
    return count > oldCount;
}

bool HashTable::remove(void *key) {
    
    size_t oldCount = count;
    
    size_t i = hash(key);
    
    i = i % capacity;
    
    while(!table[i].isAvailable){
        if(table[i].key==key){
            table[i].isAvailable = true;
            count--;
            break;
        }
        i = (i+1) % capacity;
    }
    
    return count < oldCount;
}


