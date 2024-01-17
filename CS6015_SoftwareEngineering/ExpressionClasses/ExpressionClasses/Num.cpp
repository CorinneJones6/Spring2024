//
//  Num.cpp
//  ExpressionClasses
//
//  Created by Corinne Jones on 1/16/24.
//

#include "Num.hpp"

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


