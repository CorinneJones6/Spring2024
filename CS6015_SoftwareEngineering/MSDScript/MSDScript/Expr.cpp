//
//  Expr.cpp
//  ExpressionClasses
//
//  Created by Corinne Jones on 1/16/24.
//

#include "Expr.hpp"

//====================== EXPR ======================//

string Expr::to_string(){
    stringstream st("");
    this->print(st);
    return st.str();
}

void Expr::pretty_print_at(ostream &ostream, precedence_t prec){
    print(ostream);
}

void Expr::pretty_print(ostream &ostream){
    pretty_print_at(ostream, prec_none);
}

string Expr::to_pretty_string(){
    stringstream st("");
    this->pretty_print(st);
    return st.str();
}

//======================  ADD  ======================//

Add::Add(Expr* lhs, Expr* rhs) {
    this->lhs = lhs;
    this->rhs = rhs;
}

bool Add::equals(Expr* e) {
    Add* addPtr = dynamic_cast<Add*>(e);
    if (addPtr == nullptr) {
        return false;
    }
    return this->lhs->equals(addPtr->lhs) && this->rhs->equals(addPtr->rhs);
}

int Add:: interp(){
    return this->lhs->interp() + this->rhs->interp();
}

bool Add::has_variable(){
    return this->lhs->has_variable()||this->rhs->has_variable();
}

Expr* Add::subst(string str, Expr* e){
    return new Add (this->lhs->subst(str, e), this->rhs->subst(str, e));
}

void Add::print(ostream &ostream){
    ostream << "(";
    lhs->print(ostream);
    ostream << "+";
    rhs->print(ostream);
    ostream << ")";
}

void Add::pretty_print_at(ostream &ostream, precedence_t prec) {
    if(prec >= prec_add){
        ostream << "(";
    }
    lhs->pretty_print_at(ostream, prec_add);
    
    ostream << " + ";
    
    rhs->pretty_print_at(ostream, prec_add);
    
    if(prec >= prec_add){
        ostream << ")";
    }
}

//======================  MULT  ======================//

Mult::Mult(Expr *lhs, Expr *rhs){
  this->lhs = lhs;
  this->rhs = rhs;
}

bool Mult:: equals (Expr *e) {
Mult* multPtr = dynamic_cast<Mult*>(e);
if(multPtr==nullptr){
  return false;
}
  return this->lhs->equals(multPtr->lhs) && this->rhs->equals(multPtr->rhs);
}

int Mult:: interp() {
    return this->lhs->interp() * this->rhs->interp();
}

bool Mult::has_variable() {
    return this->lhs->has_variable()||this->rhs->has_variable();
}

Expr* Mult::subst(string str, Expr* e){
    return new Mult (this->lhs->subst(str, e), this->rhs->subst(str, e));
}

void Mult::print (ostream &ostream){
    ostream << "(";
    this->lhs->print(ostream);
    ostream << "*";
    this->rhs->print(ostream);
    ostream << ")";
}

void Mult::pretty_print_at(ostream &ostream, precedence_t prec) {
    if (prec >= prec_mult) {
        ostream << "(";
    }
    
    this->lhs->pretty_print_at(ostream, prec_mult);
    
    ostream << " * ";
    
    this->rhs->pretty_print_at(ostream, prec_mult);
    
    if (prec >= prec_mult) {
        ostream << ")";
    }
}

//======================  NUM  ======================//

Num::Num (int val){
  this->val = val;
}

bool Num::equals (Expr *e) {
  Num* numPtr = dynamic_cast<Num*>(e); // Check if 'e' is a 'Num' object
  if (numPtr == nullptr) {
      return false; // 'e' is not a 'Num' object
  }
  return this->val == numPtr->val;
}

int Num:: interp(){
    return this->val;
}

bool Num::has_variable() {
    return false;
}

Expr* Num::subst(string str, Expr* e){
    return this;
}

void Num::print (ostream &ostream){
    ostream<<::to_string(val);
}

//======================  VAR  ======================//

Var::Var (string val){
  this->val = val;
}

bool Var::equals (Expr *e) {
  Var* varPtr = dynamic_cast<Var*>(e); // Check if 'e' is a 'VarExpr' object
  if (varPtr == nullptr) {
      return false; // 'e' is not a 'VarExpr' object
  }
  return this->val == varPtr->val;
}

int Var::interp(){
    throw std::runtime_error("Var cannot be converted to a number");
    
    return 1;
}

bool Var::has_variable() {
    return true;
}

Expr* Var::subst(string str, Expr* e){
    if(val==str){
        return e;
    }
    else {
        return this;
    }
}

void Var::print (ostream &ostream){
    ostream << val;
}
