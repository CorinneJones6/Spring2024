//
//  Mult.hpp
//  ExpressionClasses
//
//  Created by Corinne Jones on 1/16/24.
//

#pragma once
#include <stdio.h>
#include "Expr.hpp"

class Mult : public Expr {
public:
    Expr* lhs;
    Expr* rhs;

    Mult(Expr* lhs, Expr* rhs);

    virtual bool equals(Expr* e);
};

