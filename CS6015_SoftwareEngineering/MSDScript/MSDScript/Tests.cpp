//
//  Tests.cpp
//  ExpressionClasses
//
//  Created by Corinne Jones on 1/18/24.
//

#include <stdio.h>
#include "Expr.hpp"
#include "catch.h"


TEST_CASE("TESTING NUM"){
    
    //======================  EQUALS  ======================//
    
    CHECK( (new Num(1))->equals(new Num(1))==true );
    CHECK( (new Num(1))->equals(new Num(2))==false );
    CHECK( (new Num(INT_MAX))->equals(new Num(INT_MAX))==true );
    CHECK( (new Num(INT_MIN))->equals(new Num(INT_MIN))==true );
    CHECK( (new Num(0))->equals(new Num(0))==true);
    CHECK( (new Num(-1))->equals(new Num(-1))==true);
    CHECK( (new Num(-1))->equals(new Num(-3))==false);
    
    //======================  INTERP  ======================//
    CHECK( (new Num(-1))->interp()==-1 );
    CHECK( (new Num(0))->interp()==0 );
    CHECK( (new Num(INT_MAX))->interp()==INT_MAX );
    
    //======================  HAS_VARIABLE  ======================//
    
    
    //======================  SUBST  ======================//
    
}

TEST_CASE("TESTING ADD"){
    
    //======================  EQUALS  ======================//
    
    CHECK( (new Add(new Num(2),new Num(3)))->equals(new Add(new Num(2),new Num(3)))==true );
    CHECK( ((new Add(new Num(2),new Num(3)))->equals(new Add(new Num(3),new Num(2))))==false );
    CHECK( ((new Add(new Num(-1),new Num(-2)))->equals(new Add(new Num(-1),new Num(-2))))==true);
    CHECK( ((new Add(new Num(INT_MAX),new Num(INT_MIN)))->equals(new Add(new Num(INT_MAX),new Num(INT_MIN))))==true);
    CHECK( ((new Add(new Num(0),new Num(0)))->equals(new Add(new Num(0),new Num(0))))==true );
    
    //======================  INTERP  ======================//
    
    
    CHECK( (new Add(new Add(new Num(10), new Num(15)),new Add(new Num(20),new Num(20))))
            ->interp()==65);
    CHECK( (new Add(new Add(new Num(-1), new Num(-1)),new Add(new Num(-2),new Num(-2))))
            ->interp()==-6);
    CHECK( (new Add(new Add(new Num(0), new Num(0)),new Add(new Num(0),new Num(0))))
            ->interp()==0);
    CHECK( (new Add(new Add(new Num(100), new Num(100)),new Add(new Num(200),new Num(200))))
            ->interp()==600);
    
    //======================  HAS_VARIABLE  ======================//
    CHECK( (new Add(new Var("x"), new Num(1)))->has_variable() == true );
    
    
    //======================  SUBST  ======================//
    CHECK( (new Add(new Var("x"), new Num(7)))
           ->subst("x", new Var("y"))
           ->equals(new Add(new Var("y"), new Num(7))) );
}

TEST_CASE("TESTING MULT"){
    
    //======================  EQUALS  ======================//
    
    CHECK((new Mult(new Num(2),new Num(3)))->equals(new Mult(new Num(2),new Num(3)))==true );
    CHECK((new Mult(new Num(2),new Num(3)))->equals(new Mult(new Num(3),new Num(2)))==false );
    CHECK( ((new Mult(new Num(-1),new Num(-2)))->equals(new Mult(new Num(-1),new Num(-2))))==true);
    CHECK( ((new Mult(new Num(INT_MAX),new Num(INT_MIN)))->equals(new Mult(new Num(INT_MAX),new Num(INT_MIN))))==true);
    CHECK( ((new Mult(new Num(0),new Num(0)))->equals(new Mult(new Num(0),new Num(0))))==true );
    
    //======================  INTERP  ======================//
    
    CHECK( (new Mult(new Num(3), new Num(2)))
            ->interp()==6 );
    CHECK( (new Mult(new Mult(new Num(1), new Num(2)), new Mult(new Num(1),new Num(2))))
            ->interp()==4);
    
    CHECK( (new Mult(new Mult(new Num(0), new Num(0)), new Mult(new Num(0),new Num(0))))
            ->interp()==0);
    
    CHECK( (new Mult(new Mult(new Num(-1), new Num(-1)), new Mult(new Num(-1),new Num(-1))))
            ->interp()==1);
    CHECK( (new Mult(new Mult(new Num(-1), new Num(-1)), new Mult(new Num(-1),new Num(1))))
            ->interp()==-1);
    
    //======================  HAS_VARIABLE  ======================//
    
    CHECK( (new Mult(new Num(2), new Num(1)))->has_variable() == false );
    
    //======================  SUBST  ======================//
    
    CHECK( (new Var("x"))
           ->subst("x", new Mult(new Var("y"),new Num(7)))
          ->equals(new Mult(new Var("y"),new Num(7))) );
  
}

TEST_CASE("TESTING VAR"){
    
    //======================  EQUALS  ======================//
    
    CHECK( (new Var("x"))->equals(new Var("x")) == true );
    CHECK( (new Var("x"))->equals(new Var("y")) == false );
    CHECK( (new Num(1))->equals(new Var("x")) == false );
    
    //======================  INTERP  ======================//
    CHECK_THROWS_WITH( (new Var("x"))->interp(), "Var cannot be converted to a number" );
    
    //======================  HAS_VARIABLE  ======================//
    
    //======================  SUBST  ======================//
    
    CHECK( (new Var("x"))
           ->subst("x", new Add(new Var("y"),new Num(7)))
          ->equals(new Add(new Var("y"),new Num(7))) );
    
}

           
