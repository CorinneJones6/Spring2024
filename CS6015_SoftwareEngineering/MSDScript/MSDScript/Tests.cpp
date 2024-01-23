//
//  Tests.cpp
//  ExpressionClasses
//
//  Created by Corinne Jones on 1/18/24.
//

#include <stdio.h>
#include "Expr.hpp"
#include "catch.h"


TEST_CASE("testing Num equals"){
    CHECK( (new Num(1))->equals(new Num(1))==true );
    CHECK( (new Num(1))->equals(new Num(2))==false );
}

TEST_CASE("testing Add equals"){
    
    CHECK( (new Add(new Num(2),new Num(3)))->equals(new Add(new Num(2),new Num(3)))==true );
    CHECK( ((new Add(new Num(2),new Num(3)))->equals(new Add(new Num(3),new Num(2))))==false );

}

TEST_CASE("testing mult equals"){
    
    CHECK((new Mult(new Num(2),new Num(3)))->equals(new Mult(new Num(2),new Num(3)))==true );
    CHECK((new Mult(new Num(2),new Num(3)))->equals(new Mult(new Num(3),new Num(2)))==false );
}

TEST_CASE("testing Var equals"){
    
    CHECK( (new Var("x"))->equals(new Var("x")) == true );
    CHECK( (new Var("x"))->equals(new Var("y")) == false );
    CHECK( (new Num(1))->equals(new Var("x")) == false );
    
}

TEST_CASE("testing Expr Interp"){
    CHECK( (new Mult(new Num(3), new Num(2)))
            ->interp()==6 );
    CHECK( (new Add(new Add(new Num(10), new Num(15)),new Add(new Num(20),new Num(20))))
            ->interp()==65);
}

TEST_CASE("testing Var Throws Exception"){
    
    CHECK_THROWS_WITH( (new Var("x"))->interp(), "Var cannot be converted to a number" );
}
           
