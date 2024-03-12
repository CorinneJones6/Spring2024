//
//  MyMalloc.cpp
//  A4_MallocReplacement
//
//  Created by Corinne Jones on 3/11/24.
//

#include "MyMalloc.hpp"


MyMalloc::~MyMalloc(){
    
}

void* MyMalloc::allocate( size_t bytesToAllocate ){
    
}

void MyMalloc::deallocate( void* ptr ){
    hashTable.find(ptr);
}
