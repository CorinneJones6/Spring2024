//
//  env.cpp
//  MSDScript2
//
//  Created by Corinne Jones on 3/30/24.
//

#include "env.hpp"

PTR(Env) Env::empty = NEW(EmptyEnv)();

PTR(Val) EmptyEnv::lookup(string find_name) {
    throw runtime_error("free variable: " + find_name);
}

ExtendedEnv::ExtendedEnv(string name, PTR(Val) val, PTR(Env) rest){
    this->name = name;
    this->val = val;
    this->rest = rest;
}
PTR(Val) ExtendedEnv::lookup(string find_name){
    if(find_name == name){
        return val;
    }
    else {
        return rest->lookup(find_name);
    }
}
