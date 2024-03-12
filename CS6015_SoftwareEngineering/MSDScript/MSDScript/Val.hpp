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

using namespace std;
class Expr;

class Val {
public:
    virtual bool equals (Val *v)=0;
    virtual Expr* to_expr()=0;
    virtual Val* add_to(Val* other_val)=0;
    virtual Val* mult_with(Val* other_val)=0;
    virtual void print(ostream &ostream)=0;
    
    string to_string();
};

class NumVal : public Val {
public:
    int val;
    
    NumVal(int i);
    
    virtual Expr* to_expr();
    virtual bool equals (Val *v);
    virtual Val* add_to(Val* other_val);
    virtual Val* mult_with(Val* other_val);
    virtual void print(ostream &ostream); 
    
};

class BoolVal : public Val {
    public:
    bool val;
    
    BoolVal(bool b);
    
    virtual Expr* to_expr();
    virtual bool equals (Val *v);
    virtual Val* add_to(Val* other_val);
    virtual Val* mult_with(Val* other_val);
    virtual void print(ostream &ostream);
    
};
