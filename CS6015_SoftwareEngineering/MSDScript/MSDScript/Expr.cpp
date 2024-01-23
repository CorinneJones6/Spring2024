//
//  Expr.cpp
//  ExpressionClasses
//
//  Created by Corinne Jones on 1/16/24.
//

#include "Expr.hpp"

//----------------------ADD----------------------//

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

int Add:: interp() {

    return this->lhs->interp() + this->rhs->interp();
}

//----------------------MULT----------------------//

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

//----------------------NUM----------------------//

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

//----------------------VAR----------------------//

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
    
    return 0;
}
