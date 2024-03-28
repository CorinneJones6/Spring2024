//
//  Val.hpp
//  MSDScript
//
//  Created by Corinne Jones on 3/5/24.
//

#pragma once

#include <stdio.h>
#include <string>
#include "Expr.hpp"
#include "pointer.h"

using namespace std;
class Expr;

//======================  Val  ======================//

CLASS( Val ){
public:
    virtual bool equals (PTR(Val) v)=0;
    virtual PTR(Expr) to_expr()=0;
    virtual PTR(Val) add_to(PTR(Val) other_val)=0;
    virtual PTR(Val) mult_with(PTR(Val) other_val)=0;
    virtual void print(ostream &ostream)=0;
    virtual bool is_true()=0;
    
    string to_string();
    
    virtual PTR(Val) call(PTR(Val) actual_arg) = 0;
    
    virtual ~Val() {};
};

//======================  NumVal  ======================//

class NumVal : public Val {
public:
    int val;
    NumVal(int i);
    
    virtual PTR(Expr) to_expr();
    virtual bool equals (PTR(Val) v);
    virtual PTR(Val) add_to(PTR(Val) other_val);
    virtual PTR(Val) mult_with(PTR(Val) other_val);
    virtual void print(ostream &ostream);
    virtual bool is_true();
    
    virtual PTR(Val) call(PTR(Val) actual_arg);
};

//======================  BoolVal  ======================//

class BoolVal : public Val {
public:
    bool val;
    BoolVal(bool b);
    
    virtual PTR(Expr) to_expr();
    virtual bool equals (PTR(Val) v);
    virtual PTR(Val) add_to(PTR(Val) other_val);
    virtual PTR(Val) mult_with(PTR(Val) other_val);
    virtual void print(ostream &ostream);
    virtual bool is_true();
    
    virtual PTR(Val) call(PTR(Val) actual_arg);
};

//======================  FunVal  ======================//

class FunVal : public Val {
public: 
    string formal_arg;
    PTR(Expr) body;
    
    FunVal(string formal_arg, PTR(Expr) body);
    
    virtual PTR(Expr) to_expr();
    virtual bool equals (PTR(Val) v);
    virtual PTR(Val) add_to(PTR(Val) other_val);
    virtual PTR(Val) mult_with(PTR(Val) other_val);
    virtual void print(ostream &ostream);
    virtual bool is_true();
    
    virtual PTR(Val) call(PTR(Val) actual_arg);
};
