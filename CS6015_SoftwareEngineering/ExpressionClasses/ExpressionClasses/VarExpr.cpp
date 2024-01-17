//
//  VarExpr.cpp
//  ExpressionClasses
//
//  Created by Corinne Jones on 1/17/24.
//

#include "VarExpr.hpp"

VarExpr::VarExpr (string val){
  this->val = val;
}

bool VarExpr::equals (Expr *e) {
  VarExpr* varPtr = dynamic_cast<VarExpr*>(e); // Check if 'e' is a 'VarExpr' object
  if (varPtr == nullptr) {
      return false; // 'e' is not a 'VarExpr' object
  }
  return this->val == varPtr->val;
}
