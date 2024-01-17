//
//  VarExpr.hpp
//  ExpressionClasses
//
//  Created by Corinne Jones on 1/17/24.
//

#pragma once
#include <stdio.h>
#include "Expr.hpp"
#include <string>
using namespace std;

class VarExpr : public Expr {
public:
    string val;
    
    VarExpr (string val);
    
    virtual bool equals(Expr* e);
    
};
