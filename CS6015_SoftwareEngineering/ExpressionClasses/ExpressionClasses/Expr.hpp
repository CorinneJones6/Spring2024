//
//  Expr.hpp
//  ExpressionClasses
//
//  Created by Corinne Jones on 1/16/24.
//
#pragma once


#include <stdio.h>
#include <string>
using namespace std;


class Expr {
public:
  virtual bool equals (Expr *e)=0;
};


class Add : public Expr {
    
public:
    Expr* lhs;
    Expr* rhs;
    
    Add(Expr* lhs, Expr* rhs);

    virtual bool equals(Expr* e);
    
};

class Mult : public Expr {
public:
    Expr* lhs;
    Expr* rhs;

    Mult(Expr* lhs, Expr* rhs);

    virtual bool equals(Expr* e);
};

class Num : public Expr {
public:
    int val;
    
    Num(int val);
    
    virtual bool equals(Expr* e);
    
};

class Var : public Expr {
    
public:
    string val;
    
    Var (string val);
    
    virtual bool equals(Expr* e);
    
};



