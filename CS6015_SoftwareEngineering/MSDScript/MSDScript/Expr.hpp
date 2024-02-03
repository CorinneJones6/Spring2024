//
//  Expr.hpp
//  ExpressionClasses
//
//  Created by Corinne Jones on 1/16/24.
//
#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <stdexcept>
#include <sstream>

using namespace std;

typedef enum {
  prec_none,      // = 0
  prec_add,       // = 1
  prec_mult       // = 2
} precedence_t;

class Expr {
public:
    virtual bool equals (Expr *e)=0;
    virtual int interp()=0;
    virtual bool has_variable()=0;
    virtual Expr* subst(string str, Expr* e)=0;
    virtual void print(ostream &ostream)=0;
    
    string to_string();
    virtual void pretty_print_at(ostream &ostream, precedence_t prec);
    void pretty_print(ostream &ostream);
    string to_pretty_string();
    
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
    
    virtual Expr* subst(string str, Expr* e);
    
    virtual void print(ostream &ostream);
    
    void pretty_print_at(ostream &ostream, precedence_t prec); 
    
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
    
    virtual Expr* subst(string str, Expr* e);
    
    virtual void print (ostream &ostream);
    
    void pretty_print_at(std::ostream &ostream, precedence_t prec);
    
};

//======================  NUM  ======================//

class Num : public Expr {
public:
    int val;
    
    Num(int val);
    
    virtual bool equals(Expr* e);
    
    virtual int interp();
    
    virtual bool has_variable();
    
    virtual Expr* subst(string str, Expr* e);
    
    virtual void print (ostream &ostream);
    
};

class Var : public Expr {
    
public:
    string val;
    
    Var (string val);
    
    virtual bool equals(Expr* e);
    
    virtual int interp();
    
    virtual bool has_variable();
    
    virtual Expr* subst(string str, Expr* e);
    
    virtual void print (ostream &ostream);
    
};



