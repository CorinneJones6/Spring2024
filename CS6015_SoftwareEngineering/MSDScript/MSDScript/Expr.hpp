//
//  Expr.hpp
//  ExpressionClasses
//
//  Created by Corinne Jones on 1/16/24.
//
#pragma once


#include <stdio.h>
#include <string>
#include <stdexcept>
using namespace std;


class Expr {
public:
    virtual bool equals (Expr *e)=0;
    virtual int interp()=0;
    virtual bool has_variable()=0;
    virtual Expr* subst(string s, Expr* e)=0;
};

//======================  ADD  ======================//

class Add : public Expr {
    
public:
    Expr* lhs;
    Expr* rhs;
    
    Add(Expr* lhs, Expr* rhs);

    virtual bool equals(Expr* e);
    
    virtual int interp();
    
    virtual bool has_variable();
    
    virtual Expr* subst(string s, Expr* e);
    
};

//======================  MULT  ======================//

class Mult : public Expr {
public:
    Expr* lhs;
    Expr* rhs;

    Mult(Expr* lhs, Expr* rhs);

    virtual bool equals(Expr* e);
    
    virtual int interp();
    
    virtual bool has_variable();
    
    virtual Expr* subst(string s, Expr* e);
};

//======================  NUM  ======================//

class Num : public Expr {
public:
    int val;
    
    Num(int val);
    
    virtual bool equals(Expr* e);
    
    virtual int interp();
    
    virtual bool has_variable();
    
    virtual Expr* subst(string s, Expr* e);
    
};

class Var : public Expr {
    
public:
    string val;
    
    Var (string val);
    
    virtual bool equals(Expr* e);
    
    virtual int interp();
    
    virtual bool has_variable();
    
    virtual Expr* subst(string s, Expr* e); 
    
};



