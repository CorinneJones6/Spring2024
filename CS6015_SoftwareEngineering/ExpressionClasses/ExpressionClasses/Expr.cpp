//
//  Expr.cpp
//  ExpressionClasses
//
//  Created by Corinne Jones on 1/16/24.
//

#include "Expr.hpp"

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
