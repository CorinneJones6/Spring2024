//
//  env.hpp
//  MSDScript2
//
//  Created by Corinne Jones on 3/30/24.
//

#pragma once
#include <stdio.h>
#include "pointer.h"
#include <string>

using namespace std;
class Val;

CLASS(Env) {
public:
    static PTR(Env) empty;
    virtual PTR(Val) lookup (string find_name) = 0;
    virtual ~Env() {};
};

class EmptyEnv : public Env {
public:
    EmptyEnv() = default;
    
    virtual PTR(Val) lookup(string find_name);
};

class ExtendedEnv : public Env {
public:
    string name;
    PTR(Val) val;
    PTR(Env) rest;
    
    ExtendedEnv(string name, PTR(Val) val, PTR(Env) rest);
    
    virtual PTR(Val) lookup(string find_name);
};
