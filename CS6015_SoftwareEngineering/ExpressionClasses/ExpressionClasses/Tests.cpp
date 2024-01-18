//
//  Tests.cpp
//  ExpressionClasses
//
//  Created by Corinne Jones on 1/18/24.
//

#include <stdio.h>
#include "Expr.hpp"
#include "catch.h"


TEST_CASE("Testing Num"){
    CHECK( (new Num(1))->equals(new Num(1))==true );
    CHECK( (new Num(1))->equals(new Num(2))==false );
}

TEST_CASE("Testing Add"){
    
    CHECK( (new Add(new Num(2),new Num(3)))->equals(new Add(new Num(2),new Num(3)))==true );
    CHECK( ((new Add(new Num(2),new Num(3)))->equals(new Add(new Num(3),new Num(2))))==false );

}

TEST_CASE("Testing Mult"){
    
    CHECK((new Mult(new Num(2),new Num(3)))->equals(new Mult(new Num(2),new Num(3)))==true );
    CHECK((new Mult(new Num(2),new Num(3)))->equals(new Mult(new Num(3),new Num(2)))==false );
}

TEST_CASE("Testing Var"){
    
    CHECK( (new Var("x"))->equals(new Var("x")) == true );
    CHECK( (new Var("x"))->equals(new Var("y")) == false );
    CHECK( (new Num(1))->equals(new Var("x")) == false );
    
}
           
