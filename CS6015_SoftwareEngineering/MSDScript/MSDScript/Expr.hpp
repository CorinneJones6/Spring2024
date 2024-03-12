/**
 * \file Expr.hpp
 * \brief Declaration of expression classes for arithmetic operations.
 *
 * Defines the abstract base class Expr and its derived classes such as Add, Mult, Num, and Var.
 * These classes are used to construct and evaluate arithmetic expressions involvingh basic operations
 * and variables. Includes functionality for equality checking, evaluation, variable substitution,
 * and pretty printing of expressions.
 *
 * \author Corinne Jones
 * \date 1/16/24
 */
#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <stdexcept>
#include <sstream>

using namespace std;
class Val;

typedef enum {
  prec_none,      // = 0
  prec_add,       // = 1
  prec_mult       // = 2
} precedence_t;

class Expr {
public:
    virtual bool equals (Expr *e)=0;
    virtual Val* interp()=0;
    virtual bool has_variable()=0;
    virtual Expr* subst(string str, Expr* e)=0;
    virtual void print(ostream &ostream)=0;
    
    string to_string();
    virtual void pretty_print_at(ostream &ostream, precedence_t prec, bool let_parent, streampos &strmpos);
    void pretty_print(ostream &ostream);
    string to_pretty_string();
    
};

//======================  ADD  ======================//

class AddExpr : public Expr {
    
public:
    Expr* lhs;
    Expr* rhs;
    
    AddExpr(Expr* lhs, Expr* rhs);

    virtual bool equals(Expr* e);
    
    virtual Val* interp();
    
    virtual bool has_variable();
    
    virtual Expr* subst(string str, Expr* e);
    
    virtual void print(ostream &ostream);
    
    void pretty_print_at(ostream &ostream, precedence_t prec, bool let_parent, streampos &strmpos);
    
};

//======================  MULT  ======================//

class MultExpr : public Expr {
public:
    Expr* lhs;
    Expr* rhs;

    MultExpr(Expr* lhs, Expr* rhs);

    virtual bool equals(Expr* e);
    
    virtual Val* interp();
    
    virtual bool has_variable();
    
    virtual Expr* subst(string str, Expr* e);
    
    virtual void print (ostream &ostream);
    
    void pretty_print_at(ostream &ostream, precedence_t prec, bool let_parent, streampos &strmpos);
    
};

//======================  NUM  ======================//

class NumExpr : public Expr {
public:
    int val;
    
    NumExpr(int rep);
    
    virtual bool equals(Expr* e);
    
    virtual Val* interp();
    
    virtual bool has_variable();
    
    virtual Expr* subst(string str, Expr* e);
    
    virtual void print (ostream &ostream);
    
};

class VarExpr : public Expr {
    
public:
    string val;
    
    VarExpr (string val);
    
    virtual bool equals(Expr* e);
    
    virtual Val* interp();
    
    virtual bool has_variable();
    
    virtual Expr* subst(string str, Expr* e);
    
    virtual void print (ostream &ostream);
    
};

//======================  LET  ======================//

class LetExpr : public Expr {
    
public:
    string lhs;
    Expr* rhs;
    Expr* body;
    
    LetExpr(string lhs, Expr* rhs, Expr* body);

    virtual bool equals(Expr* e);
    
    virtual Val* interp();
    
    virtual bool has_variable();
    
    virtual Expr* subst(string str, Expr* e);
    
    virtual void print(ostream &ostream);
    
    void pretty_print_at(ostream &ostream, precedence_t prec, bool let_parent, streampos &strmpos);
    
};



