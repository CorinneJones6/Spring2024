//
//  Add.hpp
//  ExpressionClasses
//
//  Created by Corinne Jones on 1/16/24.
//

#pragma once

#include "Expr.hpp"

class Add : public Expr {
public:
    Expr* lhs;
    Expr* rhs;
    
    Add(Expr* lhs, Expr* rhs);

    virtual bool equals(Expr* e);
    
};
