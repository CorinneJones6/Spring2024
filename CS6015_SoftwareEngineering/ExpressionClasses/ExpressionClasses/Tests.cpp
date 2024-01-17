//
//  Tests.cpp
//  ExpressionClasses
//
//  Created by Corinne Jones on 1/16/24.
//

#include "Tests.hpp"

void testNum(){
    assert((new Num(1))->equals(new Num(1)));
    assert(!(new Num(1))->equals(new Num(2)));
}

void testAdd(){
    assert((new Add(new Num(2),new Num(3)))->equals(new Add(new Num(2),new Num(3))));
    assert(!(new Add(new Num(2),new Num(3)))->equals(new Add(new Num(3),new Num(2))));
}

void testMult(){
    assert((new Mult(new Num(2), new Num(3)))->equals(new Mult(new Num(2), new Num(3))));
    assert(!(new Mult(new Num(2), new Num(3)))->equals(new Mult(new Num(3), new Num(2))));
}

void testVarExpr(){
    assert((new VarExpr("x"))->equals(new VarExpr("x")));
    assert(!(new VarExpr("x"))->equals(new VarExpr("y")));
    assert(!(new Num(1))->equals(new VarExpr("x")));
}
           
void Tests::tests(){
    testNum();
    testAdd();
    testMult();
    testVarExpr();
}
