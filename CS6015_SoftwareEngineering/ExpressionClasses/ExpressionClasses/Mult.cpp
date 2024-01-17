//
//  Mult.cpp
//  ExpressionClasses
//
//  Created by Corinne Jones on 1/16/24.
//

#include "Mult.hpp"

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
  
