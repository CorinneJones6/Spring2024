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
#include "pointer.h"
#include "env.hpp"

using namespace std;
class Val;

typedef enum {
  prec_none,      // = 0
  prec_add,       // = 1
  prec_mult       // = 2
} precedence_t;

CLASS(Expr) {
public:
    virtual bool equals (PTR(Expr) e)=0;
    virtual PTR(Val) interp(PTR(Env) env = nullptr)=0;
//    virtual PTR(Expr) subst(string str, PTR(Expr) e)=0;
    virtual void print(ostream &ostream)=0;
    string to_string();
    
    virtual void pretty_print_at(ostream &ostream, precedence_t prec, bool let_parent, streampos &strmpos);
    void pretty_print(ostream &ostream);
    string to_pretty_string();
    virtual ~Expr() {}; 
};

//======================  ADD  ======================//

class AddExpr : public Expr {
    
public:
    PTR(Expr) lhs;
    PTR(Expr) rhs;
    
    AddExpr(PTR(Expr) lhs, PTR(Expr) rhs);
    virtual bool equals(PTR(Expr) e);
    virtual PTR(Val) interp(PTR(Env) env = nullptr);
//    virtual PTR(Expr) subst(string str, PTR(Expr) e);
    virtual void print(ostream &ostream);
    void pretty_print_at(ostream &ostream, precedence_t prec, bool let_parent, streampos &strmpos);
};

//======================  MULT  ======================//

class MultExpr : public Expr {
public:
    PTR(Expr) lhs;
    PTR(Expr) rhs;

    MultExpr(PTR(Expr) lhs, PTR(Expr) rhs);

    virtual bool equals(PTR(Expr) e);
    virtual PTR(Val) interp(PTR(Env) env = nullptr);
//    virtual PTR(Expr) subst(string str, PTR(Expr) e);
    virtual void print (ostream &ostream);
    void pretty_print_at(ostream &ostream, precedence_t prec, bool let_parent, streampos &strmpos);
};

//======================  NUM  ======================//

class NumExpr : public Expr {
public:
    int val;
    
    NumExpr(int rep);
    virtual bool equals(PTR(Expr) e);
    virtual PTR(Val) interp(PTR(Env) env = nullptr);
//    virtual PTR(Expr) subst(string str, PTR(Expr) e);
    virtual void print (ostream &ostream);
};

//======================  VarExpr  ======================//

class VarExpr : public Expr {
    
public:
    string val;
    
    VarExpr (string val);
    virtual bool equals(PTR(Expr) e);
    virtual PTR(Val) interp(PTR(Env) env = nullptr);
//    virtual PTR(Expr) subst(string str, PTR(Expr) e);
    virtual void print (ostream &ostream);
};

//======================  LetExpr  ======================//

class LetExpr : public Expr {
    
public:
    string lhs;
    PTR(Expr) rhs;
    PTR(Expr) body;
    
    LetExpr(string lhs, PTR(Expr) rhs, PTR(Expr) body);
    virtual bool equals(PTR(Expr) e);
    virtual PTR(Val) interp(PTR(Env) env = nullptr);
//    virtual PTR(Expr) subst(string str, PTR(Expr) e);
    virtual void print(ostream &ostream);
    void pretty_print_at(ostream &ostream, precedence_t prec, bool let_parent, streampos &strmpos);
};

//======================  BoolExpr  ======================//

class BoolExpr : public Expr {
public:
    bool val;
    
    BoolExpr(bool b);
    virtual bool equals (PTR(Expr) e);
    virtual PTR(Val) interp(PTR(Env) env = nullptr);
//    virtual PTR(Expr) subst(string str, PTR(Expr) e);
    virtual void print(ostream &ostream);
    
    virtual void pretty_print_at(ostream &ostream, precedence_t prec, bool let_parent, streampos &strmpos);
};

//======================  IfExpr  ======================//

class IfExpr : public Expr {
public:
    PTR(Expr) if_;
    PTR(Expr) then_;
    PTR(Expr) else_ ;
    
    IfExpr(PTR(Expr) if_, PTR(Expr) then_, PTR(Expr) else_);
    virtual bool equals (PTR(Expr)e);
    virtual PTR(Val) interp(PTR(Env) env = nullptr);
//    virtual PTR(Expr) subst(string str, PTR(Expr) e);
    virtual void print(ostream &ostream);
    
    virtual void pretty_print_at(ostream &ostream, precedence_t prec, bool let_parent, streampos &strmpos);
};

//======================  EqExpr  ======================//

class EqExpr : public Expr {
public:
    PTR(Expr) rhs;
    PTR(Expr) lhs;
    
    EqExpr(PTR(Expr) rhs, PTR(Expr) lhs);
    virtual bool equals (PTR(Expr) e);
    virtual PTR(Val) interp(PTR(Env) env = nullptr);
//    virtual PTR(Expr) subst(string str, PTR(Expr) e);
    virtual void print(ostream &ostream);
    
    virtual void pretty_print_at(ostream &ostream, precedence_t prec, bool let_parent, streampos &strmpos);
};

//======================  FunExpr  ======================//

class FunExpr : public Expr {
public:
    string formal_arg;
    PTR(Expr)body;
    
    FunExpr(string formal_arg, PTR(Expr)body);
    virtual bool equals (PTR(Expr)e);
    virtual PTR(Val) interp(PTR(Env) env = nullptr);
//    virtual PTR(Expr) subst(string str, PTR(Expr) e);
    virtual void print(ostream &ostream);
    
    virtual void pretty_print_at(ostream &ostream, precedence_t prec, bool let_parent, streampos &strmpos);
};

//======================  CallExpr  ======================//

class CallExpr : public Expr {
public: 
    PTR(Expr) to_be_called;
    PTR(Expr) actual_arg;
    
    CallExpr(PTR(Expr) to_be_called, PTR(Expr) actual_arg);
    virtual bool equals (PTR(Expr) e);
    virtual PTR(Val) interp(PTR(Env) env = nullptr);
//    virtual PTR(Expr) subst(string str, PTR(Expr) e);
    virtual void print(ostream &ostream);
    
    virtual void pretty_print_at(ostream &ostream, precedence_t prec, bool let_parent, streampos &strmpos);
};
