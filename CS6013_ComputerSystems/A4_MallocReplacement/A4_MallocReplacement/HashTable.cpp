/**
 * @file HashTable.hpp
 * @brief Assignment 4: Malloc Replacement - Defines a HashTable class for managing memory allocations.
 *
 * Created by Corinne Jones on 3/11/24.
 */
#include "HashTable.hpp"

//==================== PRIVATE METHODS ====================//

size_t HashTable::hash(void *address) {
    //A reasonable hash function for pointers is x >> VirtualAddressOffsetSizeInBits
    const int VirtualAddressOffsetSizeInBits = 14;
    
    return reinterpret_cast<size_t>(address) >> VirtualAddressOffsetSizeInBits;
}

size_t HashTable::probe(size_t i) {
    
    return (i+1) % capacity;
}

size_t HashTable::getCapacity(){
    return capacity;
}

size_t HashTable::getCount(){
    return count;
}

size_t HashTable::getSize(void* address){
    size_t index = find(address);
    
    if(index==-1){
        cerr << "Address not found." << endl;
        return -1;
    }
    
    return table[index].memSize;
}

size_t HashTable::find(void *address) {
    
    size_t i = hash(address);
    
    i %= capacity;
    
    int increment = 0;
    
    while(table[i].address != address){
        i = (i+1) % capacity;
        increment++;
        
        if(increment > capacity){
            cerr << "Address not found." << endl;
            return -1; //flag for index not found
        }
    }
    
    return i;
}

void HashTable::grow() {
    //Store old variables
    size_t oldCapacity = capacity;
    
    Entry* oldTable = table;
    
    //Update to new variables
    capacity *=2;
    table = (Entry*)mmap(NULL, capacity * sizeof(Entry), PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    
    if(table == MAP_FAILED){
        throw runtime_error("Failed to allocate memory for the hashtable using mmap.");
    }
    
    for (size_t i = 0; i < capacity; i++) {
            table[i].isAvailable = true;
    }
    
    count = 0;
    // Rehash entries from old table to new one
    for(size_t i = 0; i < oldCapacity; i++) {
        if (!oldTable[i].isAvailable) {
               insert(oldTable[i].address, oldTable[i].memSize);
        }
    }
       
    // Free memory from old table
    munmap(oldTable, oldCapacity * sizeof(Entry));
}

//==================== PUBLIC METHODS ====================//

HashTable::HashTable(){
    capacity = 10;
    count = 0;
    
    table = (Entry*)mmap(NULL, capacity * sizeof(Entry), PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    
    if(table == MAP_FAILED){
        throw runtime_error("Failed to allocate memory for the hashtable using mmap.");
    }
    
    for(size_t i = 0; i < capacity; i++){
        table[i].isAvailable = true;
    }
}

HashTable::HashTable(size_t capacity) {
    this->capacity = capacity;
    count = 0;
    
    table = (Entry*)mmap(NULL, capacity * sizeof(Entry), PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

    if(table == MAP_FAILED){
        throw runtime_error("Failed to allocate memory for HashTable using mmap.");
    }
    
    for (size_t i = 0; i < capacity; i++) {
        table[i].isAvailable = true;
    }
}

HashTable::~HashTable() {
    int rc = munmap(table, capacity * sizeof(Entry));
    
    if(rc==-1){
        cerr << "Hashtable munmap() failed." << endl;
    }
}

bool HashTable::insert(void *address, size_t size) {
    
    size_t oldCount = count;

    if((count/capacity) > .7){
        grow();
    }
    
    //Get the index and put it within the capacity
    size_t i = hash(address);
    
    i = i % capacity;
    
    int loopProtection = 0;
    
    while(!table[i].isAvailable){
        //handle if the key exists
        if(table[i].address == address) {
            return false;
        }
        i = probe(i);
        
        // used in debugging
        if(++loopProtection > capacity) {
            cerr << "Error: Stuck in insert loop." << endl;
            break;
        }
    }
    
    // if available, assigns new values
    table[i].address=address;
    table[i].memSize=size;
    table[i].isAvailable = false;
    
    count++;
    
    return count > oldCount;
}

bool HashTable::remove(void *address) {
    
    size_t oldCount = count;
    
    size_t i = hash(address);
    
    i = i % capacity;
    
    int loopProtection = 0;
    
    while(true){
        if(table[i].address==address){
            table[i].isAvailable = true;
            count--;
            break;
        }
        
        i = probe(i); 
        
        if(++loopProtection > capacity){
            cerr << "Error: Stuck in remove loop." << endl;
            break;
        }
    }
    
    return count < oldCount;
}


