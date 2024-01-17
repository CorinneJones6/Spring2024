//
//  Num.hpp
//  ExpressionClasses
//
//  Created by Corinne Jones on 1/16/24.
//

#pragma once

#include <stdio.h>
#include "Expr.hpp"

class Num : public Expr {
public: 
    int val;
    
    Num(int val);
    
    virtual bool equals(Expr* e);
    
};
