//
//  MyMalloc.cpp
//  A4_MallocReplacement
//
//  Created by Corinne Jones on 3/11/24.
//

#include "MyMalloc.hpp"

MyMalloc::MyMalloc(){
    hashTable = HashTable();
}

MyMalloc::~MyMalloc(){

}

void* MyMalloc::allocate( size_t bytesToAllocate ){
    size_t pageSize = getpagesize();
    size_t alignedSize = ((bytesToAllocate + pageSize - 1) / pageSize) * pageSize;
    
    void* ptr = mmap(NULL, alignedSize, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    
    if (ptr == MAP_FAILED) {
        throw runtime_error("Failed to allocate memory for MyMalloc using mmap.");
    }
    
    bool didInsert = hashTable.insert(ptr, alignedSize);
    
    if(!didInsert){
        cerr << "MyMalloc::allocate() did not insert the ptr" << endl;
    }
    
    return ptr;
}

void MyMalloc::deallocate( void* ptr ){
    size_t index = hashTable.find(ptr);
    
    if(index==-1){
        cerr << "Index not found in the hashtable" << endl;
        throw runtime_error("Index not found in hashtable.");
    }
    
    size_t size = hashTable.getSize(ptr);
    
    int rc = munmap(ptr, size);
    
    if(rc==-1){
        throw runtime_error("MyMalloc::deallocate() munmap() failed.");
    }
    
    bool didRemove = hashTable.remove(ptr);
    
    if(!didRemove){
        cerr << "MyMalloc::deallocate() did not remove the ptr" << endl;
    }
    
}
