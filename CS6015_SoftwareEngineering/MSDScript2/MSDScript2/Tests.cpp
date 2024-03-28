/**
 * \file Tests.cpp
 * \brief Tests for the Expression Classes.
 *
 * Provides unit tests for evaluating the functionality of arithmetic expressions,
 * including basic operations like addition, multiplication, and variable handling
 * within an expression context. Utilizes the Catch testing framework.
 *
 * \author Corinne Jones
 * \date 1/18/24
 */

#include <stdio.h>
#include "Expr.hpp"
#include "Val.hpp"
#include "catch.h"
#include "parse.hpp"


TEST_CASE("NUM TESTS"){
    
    SECTION("Testing equals()") {
        CHECK( (NEW( NumExpr)(1))->equals(NEW( NumExpr)(1))==true );
        CHECK( (NEW( NumExpr)(1))->equals(NEW( NumExpr)(2))==false );
        CHECK( (NEW( NumExpr)(INT_MAX))->equals(NEW( NumExpr)(INT_MAX))==true );
        CHECK( (NEW( NumExpr)(INT_MIN))->equals(NEW( NumExpr)(INT_MIN))==true );
        CHECK( (NEW( NumExpr)(0))->equals(NEW( NumExpr)(0))==true);
        CHECK( (NEW( NumExpr)(-1))->equals(NEW( NumExpr)(-1))==true);
        CHECK( (NEW( NumExpr)(-1))->equals(NEW( NumExpr)(-3))==false);
    }
    
    SECTION("Testing interp()"){
        CHECK( (NEW( NumExpr)(-1))->interp()->equals(NEW(NumVal)(-1))); //check this one
        CHECK( (NEW( NumExpr)(0))->interp()->equals(NEW(NumVal)(0)) );
        CHECK( (NEW( NumExpr)(INT_MAX))->interp()->equals(NEW(NumVal)(INT_MAX)) );
    }
    
    SECTION("Testing subst()"){
        //tested in other tests
    }
    
    SECTION("Testing to_string()"){
        CHECK( (NEW( NumExpr)(5))->to_string() == "5" );
        CHECK( (NEW( NumExpr)(-3))->to_string() == "-3" );
    }
}

TEST_CASE("ADD TESTS"){
    
    SECTION("Testing equals()"){
        CHECK( (NEW(AddExpr)(NEW( NumExpr)(2),NEW( NumExpr)(3)))->equals(NEW(AddExpr)(NEW( NumExpr)(2),NEW( NumExpr)(3)))==true );
        CHECK( ((NEW(AddExpr)(NEW( NumExpr)(2),NEW( NumExpr)(3)))->equals(NEW(AddExpr)(NEW(NumExpr)(3),NEW(NumExpr)(2))))==false );
        CHECK( ((NEW(AddExpr)(NEW(NumExpr)(-1),NEW(NumExpr)(-2)))->equals(NEW(AddExpr)(NEW(NumExpr)(-1),NEW(NumExpr)(-2))))==true);
        CHECK( ((NEW(AddExpr)(NEW(NumExpr)(INT_MAX),NEW(NumExpr)(INT_MIN)))->equals(NEW(AddExpr)(NEW(NumExpr)(INT_MAX),NEW(NumExpr)(INT_MIN))))==true);
        CHECK( ((NEW(AddExpr)(NEW(NumExpr)(0),NEW(NumExpr)(0)))->equals(NEW(AddExpr)(NEW(NumExpr)(0),NEW(NumExpr)(0))))==true );
        CHECK( (NEW(AddExpr)(NEW(NumExpr)(NULL),NEW(NumExpr)(3)))->equals(NEW(AddExpr)(NEW(NumExpr)(2),NEW(NumExpr)(3)))==false );
    }
    
    SECTION("Testing interp()"){
        CHECK( (NEW(AddExpr)(NEW(AddExpr)(NEW(NumExpr)(10), NEW(NumExpr)(15)),NEW(AddExpr)(NEW(NumExpr)(20),NEW(NumExpr)(20))))
              ->interp()->equals(NEW(NumVal)(65)));
        CHECK( (NEW(AddExpr)(NEW(AddExpr)(NEW(NumExpr)(-1), NEW(NumExpr)(-1)),NEW(AddExpr)(NEW(NumExpr)(-2),NEW(NumExpr)(-2))))
              ->interp()->equals(NEW(NumVal)(-6)));
        CHECK( (NEW(AddExpr)(NEW(AddExpr)(NEW(NumExpr)(0), NEW(NumExpr)(0)),NEW(AddExpr)(NEW(NumExpr)(0),NEW(NumExpr)(0))))
              ->interp()->equals(NEW(NumVal)(0)));
        CHECK( (NEW(AddExpr)(NEW(AddExpr)(NEW(NumExpr)(100), NEW(NumExpr)(100)),NEW(AddExpr)(NEW(NumExpr)(200),NEW(NumExpr)(200))))
              ->interp()->equals(NEW(NumVal)(600)));
    }
    
    SECTION("Testing subst()"){
        CHECK( (NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(7)))
              ->subst("x", NEW(VarExpr)("y"))
              ->equals(NEW(AddExpr)(NEW(VarExpr)("y"), NEW(NumExpr)(7))) );
    }
    
    SECTION("Testing to_string()"){
        CHECK( (NEW(AddExpr)(NEW(NumExpr)(5), NEW(NumExpr)(3)))->to_string() == "(5+3)" );
        CHECK( (NEW(AddExpr)(NEW(NumExpr)(-2), NEW(NumExpr)(4)))->to_string() == "(-2+4)" );
    }
}

TEST_CASE("MULT TESTS"){
    
    SECTION("Testing equals()"){
        CHECK((NEW(MultExpr)(NEW(NumExpr)(2),NEW(NumExpr)(3)))->equals(NEW(MultExpr)(NEW(NumExpr)(2),NEW(NumExpr)(3)))==true );
        CHECK((NEW(MultExpr)(NEW(NumExpr)(2),NEW(NumExpr)(3)))->equals(NEW(MultExpr)(NEW(NumExpr)(3),NEW(NumExpr)(2)))==false );
        CHECK( ((NEW(MultExpr)(NEW(NumExpr)(-1),NEW(NumExpr)(-2)))->equals(NEW(MultExpr)(NEW(NumExpr)(-1),NEW(NumExpr)(-2))))==true);
        CHECK( ((NEW(MultExpr)(NEW(NumExpr)(INT_MAX),NEW(NumExpr)(INT_MIN)))->equals(NEW(MultExpr)(NEW(NumExpr)(INT_MAX),NEW(NumExpr)(INT_MIN))))==true);
        CHECK( ((NEW(MultExpr)(NEW(NumExpr)(0),NEW(NumExpr)(0)))->equals(NEW(MultExpr)(NEW(NumExpr)(0),NEW(NumExpr)(0))))==true );
        CHECK( (NEW(MultExpr)(NEW(NumExpr)(NULL),NEW(NumExpr)(3)))->equals(NEW(AddExpr)(NEW(NumExpr)(2),NEW(NumExpr)(3)))==false );
    }

    SECTION("Testing interp()"){
        CHECK( (NEW(MultExpr)(NEW(NumExpr)(3), NEW(NumExpr)(2)))
              ->interp()->equals(NEW(NumVal)(6)));
        CHECK( (NEW(MultExpr)(NEW(MultExpr)(NEW(NumExpr)(1), NEW(NumExpr)(2)), NEW(MultExpr)(NEW(NumExpr)(1),NEW(NumExpr)(2))))
              ->interp()->equals(NEW(NumVal)(4)));
        
        CHECK( (NEW(MultExpr)(NEW(MultExpr)(NEW(NumExpr)(0), NEW(NumExpr)(0)), NEW(MultExpr)(NEW(NumExpr)(0),NEW(NumExpr)(0))))
              ->interp()->equals(NEW(NumVal)(0)));
        
        CHECK( (NEW(MultExpr)(NEW(MultExpr)(NEW(NumExpr)(-1), NEW(NumExpr)(-1)), NEW(MultExpr)(NEW(NumExpr)(-1),NEW(NumExpr)(-1))))
              ->interp()->equals(NEW(NumVal)(1)));
        CHECK( (NEW(MultExpr)(NEW(MultExpr)(NEW(NumExpr)(-1), NEW(NumExpr)(-1)), NEW(MultExpr)(NEW(NumExpr)(-1),NEW(NumExpr)(1))))
              ->interp()->equals(NEW(NumVal)(-1)));
    }
    
    SECTION("Testing subst()"){
        CHECK( (NEW(MultExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(7)))
              ->subst("x", NEW(VarExpr)("y"))
              ->equals(NEW(MultExpr)(NEW(VarExpr)("y"), NEW(NumExpr)(7))) );
        CHECK( (NEW(VarExpr)("x"))
              ->subst("x", NEW(MultExpr)(NEW(VarExpr)("y"),NEW(NumExpr)(7)))
              ->equals(NEW(MultExpr)(NEW(VarExpr)("y"),NEW(NumExpr)(7))) );
    }
    
    SECTION("Testing to_string()"){
        CHECK( (NEW(MultExpr)(NEW(NumExpr)(6), NEW(NumExpr)(7)))->to_string() == "(6*7)" );
        CHECK( (NEW(MultExpr)(NEW(NumExpr)(-1), NEW(NumExpr)(3)))->to_string() == "(-1*3)" );
    }
}

TEST_CASE("VAR TESTS"){
    
    SECTION("Testing equals()"){
        CHECK( (NEW(VarExpr)("x"))->equals(NEW(VarExpr)("x")) == true );
        CHECK( (NEW(VarExpr)("x"))->equals(NEW(VarExpr)("y")) == false );
        CHECK( (NEW(NumExpr)(1))->equals(NEW(VarExpr)("x")) == false );
        PTR(VarExpr) var2 = nullptr; // Creating a nullptr
        CHECK(( NEW(VarExpr)("c"))->equals(var2)==false );
    }
    
    SECTION("Testing interp()"){
        CHECK_THROWS_WITH( (NEW(VarExpr)("x"))->interp(), "Var cannot be converted to a number" );
    }
    
    SECTION("Testing subst()"){
        CHECK( (NEW(VarExpr)("x"))
              ->subst("x", NEW(AddExpr)(NEW(VarExpr)("y"),NEW(NumExpr)(7)))
              ->equals(NEW(AddExpr)(NEW(VarExpr)("y"),NEW(NumExpr)(7))) );
        PTR(VarExpr) var = NEW(VarExpr)("x");
        CHECK( (var)->subst("y", NEW(NumExpr)(42)) == var );
    }
    
    SECTION("Testing to_string()"){
        CHECK( (NEW(VarExpr)("x"))->to_string() == "x" );
        CHECK( (NEW(VarExpr)("y"))->to_string() == "y" );
    }
}

TEST_CASE("Testing pretty_print()"){
    CHECK ( (NEW(MultExpr)(NEW(NumExpr)(1), NEW(AddExpr)(NEW(NumExpr)(2), NEW(NumExpr)(3))))->to_pretty_string() ==  "1 * (2 + 3)" );
    CHECK ( (NEW(MultExpr)(NEW(MultExpr)(NEW(NumExpr)(8), NEW(NumExpr)(1)), NEW(VarExpr)("y")))->to_pretty_string() ==  "(8 * 1) * y" );
    CHECK ( (NEW(MultExpr)(NEW(AddExpr)(NEW(NumExpr)(3), NEW(NumExpr)(5)), NEW(MultExpr)(NEW(NumExpr)(6), NEW(NumExpr)(1))))->to_pretty_string() ==  "(3 + 5) * 6 * 1" );
    CHECK ( (NEW(MultExpr)(NEW(MultExpr)(NEW(NumExpr)(7), NEW(NumExpr)(7)), NEW(AddExpr)(NEW(NumExpr)(9), NEW(NumExpr)(2))) )->to_pretty_string() ==  "(7 * 7) * (9 + 2)" );
}

TEST_CASE("LET TESTS"){
    
    SECTION("Testing equals()"){
        //equal let expressions
        PTR(Expr) letExpr1 = NEW(LetExpr)("x", NEW(NumExpr)(5), NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(3)));
        PTR(Expr) letExpr2 = NEW(LetExpr)("x", NEW(NumExpr)(5), NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(3)));
        CHECK( letExpr1->equals(letExpr2) == true );
        
        //different lhs
        PTR(Expr) letExpr3 = NEW(LetExpr)("x", NEW(NumExpr)(5), NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(3)));
        PTR(Expr) letExpr4 = NEW(LetExpr)("y", NEW(NumExpr)(5), NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(3)));
        CHECK( letExpr3->equals(letExpr4) == false );
        
        //different rhs
        PTR(Expr) letExpr5 = NEW(LetExpr)("x", NEW(NumExpr)(4), NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(3)));
        PTR(Expr) letExpr6 = NEW(LetExpr)("x", NEW(NumExpr)(5), NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(3)));
        CHECK( letExpr5->equals(letExpr6) == false );

        //different body
        PTR(Expr) letExpr7 = NEW(LetExpr)("x", NEW(NumExpr)(5), NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(2)));
        PTR(Expr) letExpr8 = NEW(LetExpr)("x", NEW(NumExpr)(5), NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(3)));
        CHECK( letExpr7->equals(letExpr8) == false );
    }
    
    SECTION("Testing interp()"){
        PTR(VarExpr) varX = NEW(VarExpr)("x");
        PTR(Expr) num5 = NEW(NumExpr)(5);
        PTR(Expr) addExpr = NEW(AddExpr)(varX, NEW(NumExpr)(2));
        CHECK( (NEW(LetExpr)("x", num5, addExpr))->interp()->equals(NEW(NumVal)(7)) );
    }
    
    SECTION("Testing subst()"){
        //Substitution in rhs, not shadowed by Let's lhs
        PTR(LetExpr) letExpr = NEW(LetExpr)("x", NEW(AddExpr)(NEW(VarExpr)("y"), NEW(NumExpr)(1)), NEW(VarExpr)("x"));
        PTR(Expr) substituted = letExpr->subst("y", NEW(NumExpr)(5));
        CHECK( substituted->equals(NEW(LetExpr)("x", NEW(AddExpr)(NEW(NumExpr)(5), NEW(NumExpr)(1)), NEW(VarExpr)("x"))) == true );
        
        //Substitution shadowed by Let's lhs
        PTR(LetExpr) letExpr1 = NEW(LetExpr)("x", NEW(NumExpr)(5), NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(3)));
        PTR(Expr) substituted1 = letExpr1->subst("x", NEW(NumExpr)(10));
        CHECK( substituted1->equals(NEW(LetExpr)("x", NEW(NumExpr)(5), NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(3)))) == true );
        
        //Substitution in body, not shadowed
        PTR(LetExpr) letExpr2 = NEW(LetExpr)("x", NEW(NumExpr)(5), NEW(AddExpr)(NEW(VarExpr)("y"), NEW(NumExpr)(3)));
        PTR(Expr) substituted2 = letExpr2->subst("y", NEW(NumExpr)(2));
        CHECK( substituted2->equals(NEW(LetExpr)("x", NEW(NumExpr)(5), NEW(AddExpr)(NEW(NumExpr)(2), NEW(NumExpr)(3)))) == true );
    }
    
    SECTION("Testing print()"){
        ostringstream out;
        PTR(LetExpr) letExpr = NEW(LetExpr)("x", NEW(NumExpr)(5), NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(3)));
        letExpr->print(out);
        CHECK( out.str() == "(_let x=5 _in (x+3))" );
    }
    
    SECTION("Testing pretty_print()"){
        //Nabil's tests
        PTR(LetExpr) innerLet = NEW(LetExpr)("y", NEW(NumExpr)(3), NEW(AddExpr)(NEW(VarExpr)("y"), NEW(NumExpr)(2)));
        PTR(LetExpr) outerLet = NEW(LetExpr)("x", NEW(NumExpr)(5), NEW(AddExpr)(innerLet, NEW(VarExpr)("x")));
        string expected =
            "_let x = 5\n"
            "_in  (_let y = 3\n"
            "      _in  y + 2) + x";

        CHECK( outerLet->to_pretty_string()==expected );
            
        //Let nested as right argument of parenthesized multiplication expression
        CHECK ( (NEW(MultExpr)(NEW(MultExpr)(NEW(NumExpr) (2), NEW(LetExpr)("x", NEW(NumExpr)(5), NEW(AddExpr)(NEW(VarExpr)("x") , NEW(NumExpr)(1)) )), NEW(NumExpr)(3)))->to_pretty_string()
               ==
               "(2 * _let x = 5\n"
               "     _in  x + 1) * 3");
        
        CHECK((NEW(MultExpr)(NEW(NumExpr)(5), NEW(AddExpr)(NEW(LetExpr)("x", NEW(NumExpr)(5), NEW(VarExpr)("x")), NEW(NumExpr)(1))))->to_pretty_string()
              ==
              "5 * ((_let x = 5\n"
              "      _in  x) + 1)");
        //Let in lhs of add
        CHECK ( (NEW(AddExpr)(NEW(LetExpr)("x", NEW(NumExpr)(2), NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(9))), NEW(NumExpr)(4)))->to_pretty_string()
               ==
               "(_let x = 2\n"
               " _in  x + 9) + 4");
        //Let in lhs of multiplication expression
        CHECK((NEW(MultExpr)(NEW(LetExpr)("x", NEW(NumExpr)(5), NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(8))), NEW(NumExpr)(3)))->to_pretty_string()
              ==
              "(_let x = 5\n"
              " _in  x + 8) * 3");
        
        //Let nest as right argument of un-parenthesized multiplication expression
        CHECK((NEW(AddExpr) (NEW(MultExpr)(NEW(NumExpr)(4), NEW(LetExpr)("x", NEW(NumExpr)(5), NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(1)))), NEW(NumExpr)(9)))->to_pretty_string()
              ==
              "4 * (_let x = 5\n"
              "     _in  x + 1) + 9");
      
        CHECK( (NEW(LetExpr)("x",NEW(NumExpr)(5), NEW(AddExpr)(NEW(LetExpr)("y", NEW(NumExpr)(3), NEW(AddExpr)(NEW(VarExpr)("y"), NEW(NumExpr)(2 ))), NEW(VarExpr)("x"))))->to_pretty_string()
              ==
              "_let x = 5\n"
              "_in  (_let y = 3\n"
              "      _in  y + 2) + x" );
    }
    
    SECTION("More pretty print"){
        
        //Let nested as right argument of parenthesized multiplication expression
        CHECK ( (NEW(MultExpr)(NEW(MultExpr)(NEW(NumExpr)(2), NEW(LetExpr)("x", NEW(NumExpr)(5), NEW(AddExpr)(NEW(VarExpr)("x") , NEW(NumExpr)(1)) )), NEW(NumExpr)(3)))->to_pretty_string()
               ==
               "(2 * _let x = 5\n"
               "     _in  x + 1) * 3");
        
        //Let nested to the left in add expression which is nested to the right within a multiplication expression
        CHECK((NEW(MultExpr)(NEW(NumExpr)(5), NEW(AddExpr)(NEW(LetExpr)("x", NEW(NumExpr)(5), NEW(VarExpr)("x")), NEW(NumExpr)(1))))->to_pretty_string()
              ==
              "5 * ((_let x = 5\n"
              "      _in  x) + 1)");
        
        //Let in lhs of add
        CHECK ( (NEW(AddExpr)(NEW(LetExpr)("x", NEW(NumExpr)(2), NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(9))), NEW(NumExpr)(4)))->to_pretty_string()
               ==
               "(_let x = 2\n"
               " _in  x + 9) + 4");

        //Let in lhs of multiplication expression
        CHECK((NEW(MultExpr)(NEW(LetExpr)("x", NEW(NumExpr)(5), NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(8))), NEW(NumExpr)(3)))->to_pretty_string()
              ==
              "(_let x = 5\n"
              " _in  x + 8) * 3");

        //Let nest as right argument of un-parenthesized multiplication expression
        CHECK((NEW(AddExpr)(NEW(MultExpr)(NEW(NumExpr)(4), NEW(LetExpr)("x", NEW(NumExpr)(5), NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(1)))), NEW(NumExpr)(9)))->to_pretty_string()
              ==
              "4 * (_let x = 5\n"
              "     _in  x + 1) + 9");

        //Let nested to the left within let that is nested to the left within add
        CHECK ((NEW(AddExpr)(NEW(LetExpr)("x", NEW(NumExpr)(3), NEW(LetExpr)("y", NEW(NumExpr)(3), NEW(AddExpr)(NEW(VarExpr)("y"), NEW(NumExpr)(2))) ), NEW(VarExpr)("x")))->to_pretty_string()
               ==
               "(_let x = 3\n"
               " _in  _let y = 3\n"
               "      _in  y + 2) + x");
        
        //Let nested in lhs of Add expression nested within body of let expression
        CHECK((NEW(LetExpr)("x", NEW(NumExpr)(5), NEW(AddExpr)(NEW(LetExpr)("y" , NEW(NumExpr)(3), NEW(AddExpr)(NEW(VarExpr)("y"), NEW(NumExpr)(2))), NEW(VarExpr)("x"))))->to_pretty_string()
              ==
              "_let x = 5\n"
              "_in  (_let y = 3\n"
              "      _in  y + 2) + x");
        
        //Triple nested let
        CHECK( ( NEW(LetExpr)( "x", NEW(NumExpr)(5), NEW(AddExpr)( NEW(LetExpr)( "y", NEW(NumExpr)(3), NEW(AddExpr)( NEW(VarExpr)("y"), NEW(LetExpr)("z", NEW(NumExpr)(6),NEW(AddExpr)(NEW(VarExpr)("a"), NEW(NumExpr)(8))) ) ), NEW(VarExpr)("x") ) ) )->to_pretty_string()
              ==
              "_let x = 5\n"
              "_in  (_let y = 3\n"
              "      _in  y + _let z = 6\n"
              "               _in  a + 8) + x" );
    }
}

TEST_CASE("parse") {
  CHECK_THROWS_WITH( parse_str("()"), "invalid input" );
  
  CHECK( parse_str("(1)")->equals(NEW( NumExpr)(1)) );
  CHECK( parse_str("(((1)))")->equals(NEW( NumExpr)(1)) );
  
  CHECK_THROWS_WITH( parse_str("(1"), "missing close parenthesis" );
  
  CHECK( parse_str("1")->equals(NEW( NumExpr)(1)) );
  CHECK( parse_str("10")->equals(NEW( NumExpr)(10)) );
  CHECK( parse_str("-3")->equals(NEW( NumExpr)(-3)) );
  CHECK( parse_str("  \n 5  ")->equals(NEW( NumExpr)(5)) );
  CHECK_THROWS_WITH( parse_str("-"), "not a num" );
  CHECK_THROWS_WITH( parse_str(" -   5  "), "not a num" );
  
  CHECK( parse_str("x")->equals(NEW( VarExpr)("x")) );
  CHECK( parse_str("xyz")->equals(NEW( VarExpr)("xyz")) );
  CHECK( parse_str("xYz")->equals(NEW( VarExpr)("xYz")) );
  CHECK_THROWS_WITH( parse_str("x_z"), "invalid input" );
  
  CHECK( parse_str("x + y")->equals(NEW( AddExpr)(NEW( VarExpr)("x"), NEW( VarExpr)("y"))) );

  CHECK( parse_str("x * y")->equals(NEW( MultExpr)(NEW( VarExpr)("x"), NEW( VarExpr)("y"))) );

  CHECK( parse_str("z * x + y")
        ->equals(NEW( AddExpr)(NEW( MultExpr)(NEW( VarExpr)("z"), NEW( VarExpr)("x")),
                             NEW( VarExpr)("y"))) );
  
  CHECK( parse_str("z * (x + y)")
        ->equals(NEW( MultExpr)(NEW( VarExpr)("z"),
                          NEW( AddExpr)(NEW( VarExpr)("x"), NEW( VarExpr)("y"))) ));
    
  CHECK( parse_str("_let x = 5 _in x + 3")
           ->equals(NEW( LetExpr)("x", NEW( NumExpr)(5), NEW( AddExpr)(NEW( VarExpr)("x"), NEW( NumExpr)(3)))) );
    
  CHECK( parse_str("_let x = 2 _in _let y = x + 3 _in y * x") ->equals(NEW(LetExpr)("x", NEW(NumExpr)(2),
        NEW(LetExpr)("y", NEW( AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(3)), NEW( MultExpr)(NEW(VarExpr)("y"), NEW(VarExpr)("x"))))) );
    
    SECTION("Parse LetExpr") {
        CHECK(parse_str(("_let x=1 _in (x+2)"))->equals(NEW(LetExpr)("x", NEW(NumExpr)(1), NEW( AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(2)))));
        
        CHECK(parse_str(("_let x=1 _in (x+2)"))->equals(NEW(LetExpr)("x", NEW(NumExpr)(1), NEW( AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(2)))));
        
        CHECK(parse_str(("_let x=1 _in (y+1)"))->equals(NEW(LetExpr)("x", NEW(NumExpr)(1), NEW( AddExpr)(NEW(VarExpr)("y"), NEW(NumExpr)(1)))));
        
        CHECK(parse_str(("(_let x=3 _in ((_let y=5 _in (y+2))+x))"))->equals(NEW(LetExpr)("x", NEW(NumExpr)(3), NEW(AddExpr)(NEW(LetExpr)("y", NEW(NumExpr)(5), NEW(AddExpr)(NEW(VarExpr)("y"), NEW(NumExpr)(2))), NEW(VarExpr)("x")))));
        
        CHECK(parse_str(("(_let x=3 _in (x+5))"))->equals((NEW(LetExpr)("x", NEW(NumExpr)(3), NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(5))))));
    }
    
    SECTION("Parsing BoolExpr") {
        CHECK(parse_str(("_true"))->equals(NEW(BoolExpr)(true)));
        CHECK(parse_str(("_false"))->equals(NEW(BoolExpr)(false)));
    }

    SECTION("Parsing IfExpr") {
        CHECK(parse_str(("_if _true _then 4 _else 5"))->equals(
                NEW( IfExpr)(NEW( BoolExpr)(true), NEW( NumExpr)(4), NEW( NumExpr)(5))));
        CHECK(parse_str(("_if _false _then 4 _else 5"))->equals(
                                                                NEW( IfExpr)(NEW( BoolExpr)(false), NEW( NumExpr)(4), NEW( NumExpr)(5))));
        CHECK(parse_str(("_if 2 == 3 _then 1 _else 2"))->equals(
                                                                NEW( IfExpr)(NEW( EqExpr)(NEW( NumExpr)(2), NEW( NumExpr)(3)), NEW( NumExpr)(1), NEW( NumExpr)(2))));
    }
    
    SECTION("Parsing EqExpr") {
        CHECK( parse_str("1 == 2")->interp()->equals(NEW(BoolVal)(false)) );
        CHECK( parse_str("2 == 2")->interp()->equals(NEW(BoolVal)(true)) );
        CHECK((((parse_str("_if 1 == 2 _then 3 _else 4"))->interp())->to_string()) == "4");
        CHECK((parse_str("1 + 2 == 3 + 0"))->interp()->to_string() == "_true");
    }

    SECTION("Parsing FunExpr"){
        CHECK((parse_str("_let f = _fun (x) x+1"
                         "_in f(10)"))->interp()->to_string() == "11");

        CHECK((parse_str("_let f = _fun (x) x*x"
                         "_in f(2)"))->interp()->to_string() == "4");

        CHECK((parse_str("_let y = 8"
                         "_in  _let f = _fun (x) x*y"
                              "_in f(2)"))->interp()->to_string() == "16");

        CHECK((parse_str("_let x = 8"
                         "_in  _let f = _fun (x) x*x"
                              "_in f(2)"))->interp()->to_string() == "4");

        CHECK((parse_str("_let factrl = _fun (factrl)"
                                        "_fun (x)"
                                            "_if x ==1"
                                            "_then 1"
                                            "_else x * factrl(factrl)(x + -1)"
                         "_in  factrl(factrl)(10)"))->interp()->to_string() == "3628800");
    }

    SECTION("Parsing CallExpr"){
        CHECK((parse_str("(_fun (x) x+1 (10))"))->interp()->to_string() == "11");
        CHECK((parse_str("(_fun (x) x+x (1))"))->interp()->to_string() == "2");
    }
}

TEST_CASE("Testing NumVal") {

    SECTION("to_expr"){
        CHECK( (NEW(NumVal)(1))->to_expr()->equals((NEW(NumExpr)(1)))==true);
        CHECK( (NEW(NumVal)(1))->to_expr()->equals((NEW(NumExpr)(2)))==false);
    }

    SECTION("equals"){
        CHECK( (NEW(NumVal)(1))->equals(NEW(NumVal)(1))==true );
        CHECK( (NEW(NumVal)(1))->equals(NEW(NumVal)(2))==false );

        //(1 + 2) evaluated to 3
        CHECK( (NEW(AddExpr)(NEW(NumExpr)(1), NEW(NumExpr)(2)))->interp()->equals(NEW(NumVal)(3)) );

        //_let x = 2+3 _in x*x evaluated to 25
        CHECK( (NEW(LetExpr)("x", NEW(AddExpr)(NEW(NumExpr)(2), NEW(NumExpr)(3)), NEW(MultExpr)(NEW(VarExpr)("x"), NEW(VarExpr)("x"))))->interp()->equals(NEW(NumVal)(25)) );
    }

    SECTION("add_to"){
        PTR(Val) result = (NEW(NumVal)(5))->add_to(NEW(NumVal)(10));
        CHECK(result->to_string()=="15");

        PTR(Val) result2 = (NEW(NumVal)(-1))->add_to(NEW(NumVal)(-1));
        CHECK(result2->to_string()=="-2");

        PTR(Val) result3 = (NEW(NumVal)(0))->add_to(NEW(NumVal)(0));
        CHECK(result3->to_string()=="0");

        PTR(Val) nonNumVal = CAST(Val)(NEW(LetExpr)("x", NEW(NumExpr)(1), NEW(NumExpr)(1)));
        REQUIRE_THROWS_AS((NEW(NumVal)(5))->add_to(nonNumVal), runtime_error);
    }

    SECTION("mult_with"){
        PTR(Val) result = (NEW(NumVal)(5))->mult_with(NEW(NumVal)(10));
        CHECK(result->to_string()=="50");

        PTR(Val) result2 = (NEW(NumVal)(-1))->mult_with(NEW(NumVal)(1));
        CHECK(result2->to_string()=="-1");

        PTR(Val) result3 = (NEW(NumVal)(0))->mult_with(NEW(NumVal)(0));
        CHECK(result3->to_string()=="0");

        PTR(Val) nonNumVal = CAST(Val)(NEW(LetExpr)("x", NEW(NumExpr)(1), NEW(NumExpr)(1)));
        REQUIRE_THROWS_AS((NEW(NumVal)(5))->mult_with(nonNumVal), runtime_error);
    }

    SECTION("print"){
        NumVal numVal(123);
        NumVal numVal2(1);
        ostringstream output;
        numVal.print(output);
        CHECK(output.str() == "123");

        ostringstream output2;
        numVal2.print(output2);
        CHECK(output2.str() == "1");
    }
}

TEST_CASE("Testing BoolVal") {

    SECTION("constructor/print") {
        BoolVal trueVal(true);
        BoolVal falseVal(false);
        ostringstream outputTrue, outputFalse;

        trueVal.print(outputTrue);
        falseVal.print(outputFalse);

        CHECK(outputTrue.str() == "_true");
        CHECK(outputFalse.str() == "_false");
    }

    SECTION("to_expr"){
        BoolVal boolVal(true);
        PTR(Expr) expr = boolVal.to_expr();
        ostringstream output;
        expr->print(output);
        CHECK(output.str() == "_true");

        BoolVal boolVal2(false);
        PTR(Expr) expr2 = boolVal2.to_expr();
        ostringstream output2;
        expr2->print(output2);
        CHECK(output2.str() == "_false");
    }

    SECTION("equals") {
        CHECK( (NEW(BoolVal)(true))->equals(NEW(BoolVal)(true))==true );
        CHECK( (NEW(BoolVal)(true))->equals(NEW(BoolVal)(false))==false );
    }

    SECTION("add_to") {
            PTR(BoolVal) boolVal = NEW(BoolVal)(true);
            PTR(BoolVal) anotherBoolVal = NEW(BoolVal)(false);

            REQUIRE_THROWS_AS(boolVal->add_to(anotherBoolVal), runtime_error);
    }

    SECTION("mult_with") {
          REQUIRE_THROWS_AS((NEW(BoolVal)(true))->mult_with(NEW(BoolVal)(false)), runtime_error);
    }
}

TEST_CASE("Testing BoolExpr") {

    SECTION("constructor + print"){
        BoolExpr trueExpr(true);
        BoolExpr falseExpr(false);
        ostringstream outputTrue, outputFalse;

        trueExpr.print(outputTrue);
        falseExpr.print(outputFalse);

        CHECK( outputTrue.str() == "_true" );
        CHECK( outputFalse.str() == "_false" );
    }

    SECTION("equals") {
        CHECK( (NEW(BoolExpr)(true))->equals(NEW(BoolExpr)(true)) );
        CHECK_FALSE( (NEW(BoolExpr)(true))->equals(NEW(BoolExpr)(false)) );
    }

    SECTION("interp") {
        CHECK( (NEW(BoolExpr)(true))->interp()->equals(NEW(BoolVal)(true)) );
        CHECK( (NEW(BoolExpr)(false))->interp()->equals(NEW(BoolVal)(false)) );
    }
}

TEST_CASE("Testing IfExpr") {

    SECTION("Testing constructor") {
        PTR(Expr) condition = NEW(BoolExpr)(true);
        PTR(Expr) thenBranch = NEW(NumExpr)(1);
        PTR(Expr) elseBranch = NEW(NumExpr)(0);
        IfExpr ifExpr(condition, thenBranch, elseBranch);

        CHECK(ifExpr.if_ == condition);
        CHECK(ifExpr.then_ == thenBranch);
        CHECK(ifExpr.else_ == elseBranch);
    }

    SECTION("Testing equals()") {
        PTR(Expr) ifExpr1 = (NEW(IfExpr)(NEW(BoolExpr)(true), NEW(NumExpr)(1), NEW(NumExpr)(0)));
        PTR(Expr) ifExpr2 = NEW(IfExpr)(NEW(BoolExpr)(true), NEW(NumExpr)(1), NEW(NumExpr)(0));
        PTR(Expr) ifExpr3 = NEW(IfExpr)(NEW(BoolExpr)(false), NEW(NumExpr)(2), NEW(NumExpr)(3));

        CHECK(ifExpr1->equals(ifExpr2) == true);
        CHECK(ifExpr1->equals(ifExpr3) == false);
    }

    SECTION("Testing interp()") {
        PTR(Expr) conditionTrue = NEW(BoolExpr)(true);
        PTR(Expr) conditionFalse = NEW(BoolExpr)(false);
        PTR(Expr) thenBranch = NEW(NumExpr)(1);
        PTR(Expr) elseBranch = NEW(NumExpr)(2);

        CHECK( (NEW(IfExpr)(conditionTrue, thenBranch, elseBranch))->interp()->equals(NEW(NumVal)(1)) );
        CHECK( (NEW(IfExpr)(conditionFalse, thenBranch, elseBranch))->interp()->equals(NEW(NumVal)(2)) );

        //_if _true _then 1 _else 2 evaluated to 1
        CHECK( (NEW(IfExpr)(NEW(BoolExpr)(true), NEW(NumExpr)(1), NEW(NumExpr)(2)))->interp()->equals(NEW(NumVal)(1)) );
    }

    SECTION("Testing subst()") {
        PTR(Expr) condition = NEW(VarExpr)("x");
        PTR(Expr) thenBranch = NEW(NumExpr)(1);
        PTR(Expr) elseBranch = NEW(NumExpr)(2);
        PTR(IfExpr) ifExpr = NEW(IfExpr)(condition, thenBranch, elseBranch);

        PTR(Expr) substituted = ifExpr->subst("x", NEW(BoolExpr)(true));

        CHECK(CAST(IfExpr)(substituted) != nullptr);
        PTR(IfExpr) substIfExpr = CAST(IfExpr)(substituted);
        CHECK(substIfExpr->if_ != condition); // Ensure the condition has been substituted
    }


    SECTION("Testing print()") {
        ostringstream output;
        IfExpr ifExpr(NEW(BoolExpr)(true), NEW(NumExpr)(1), NEW(NumExpr)(0));
        ifExpr.print(output);

        CHECK(output.str() == "(_if_true_then1_else0)");
    }

    SECTION("Testing pretty_print_at()") {
        string expected =
              "_if _true\n"
              "_then 1\n"
              "_else 0\n";

        CHECK( (NEW(IfExpr)(NEW(BoolExpr)(true), NEW(NumExpr)(1), NEW(NumExpr)(0)))->to_pretty_string() == expected);
    }

    SECTION("pretty_print"){

        CHECK( (NEW(IfExpr)(NEW(EqExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(1)), NEW(NumExpr)(1), NEW(NumExpr)(2)))->to_pretty_string()
              ==
              "_if x==1\n"
              "_then 1\n"
              "_else 2\n" );
    }
}

TEST_CASE("Testing EqExpr") {

    SECTION("Testing constructor") {
        PTR(Expr) lhs = NEW(NumExpr)(1);
        PTR(Expr) rhs = NEW(NumExpr)(1);
        EqExpr eqExpr(lhs, rhs);

        CHECK(eqExpr.lhs == lhs);
        CHECK(eqExpr.rhs == rhs);
    }

    SECTION("Testing equals()") {
        PTR(Expr) eqExpr1 = NEW(EqExpr)(NEW(NumExpr)(1), NEW(NumExpr)(1));
        PTR(Expr) eqExpr2 = NEW(EqExpr)(NEW(NumExpr)(1), NEW(NumExpr)(1));
        PTR(Expr) eqExpr3 = NEW(EqExpr)(NEW(NumExpr)(1), NEW(NumExpr)(2));

        CHECK(eqExpr1->equals(eqExpr2) == true);
        CHECK(eqExpr1->equals(eqExpr3) == false);
    }

    SECTION("Testing interp()") {
        PTR(Expr) lhs = NEW(NumExpr)(1);
        PTR(Expr) rhs = NEW(NumExpr)(1);
        PTR(Expr) eqExprTrue = NEW(EqExpr)(lhs, rhs);

        PTR(Expr) rhsFalse = NEW(NumExpr)(2);
        PTR(Expr) eqExprFalse = NEW(EqExpr)(lhs, rhsFalse);

        CHECK(CAST(BoolVal)(eqExprTrue->interp())->equals(NEW(BoolVal)(true)));
        CHECK(CAST(BoolVal)(eqExprFalse->interp())->equals(NEW(BoolVal)(false)));

        //(1 == 2) + 3 throws an exception
        PTR(Expr) testExpr = NEW(AddExpr)(NEW(EqExpr)(NEW(NumExpr)(1), NEW(NumExpr)(2)), NEW(NumExpr)(3));
        CHECK_THROWS_WITH(testExpr->interp(), "Bool cannot be added");

        //1 == 2 + 3 evaluates to _false"
        PTR(Expr) testExpr2 = NEW(EqExpr)(NEW(NumExpr)(1), NEW(AddExpr)(NEW(NumExpr)(2), NEW(NumExpr)(3)));
        CHECK((testExpr2->interp())->to_string() == "_false");

        //"1 + 1 == 2 + 0 evaluates to _true"
        PTR(Expr) testExpr3 = NEW(EqExpr)(NEW(AddExpr)(NEW(NumExpr)(1), NEW(NumExpr)(1)), NEW(AddExpr)(NEW(NumExpr)(2), NEW(NumExpr)(0)));
        CHECK((testExpr3->interp())->to_string() == "_true");
    }

    SECTION("Testing subst()") {
        PTR(Expr) lhs = NEW(VarExpr)("x");
        PTR(Expr) rhs = NEW(NumExpr)(1);
        PTR(EqExpr) eqExpr = NEW(EqExpr)(lhs, rhs);

        PTR(Expr) substituted = eqExpr->subst("x", NEW(NumExpr)(2));

        CHECK(CAST(EqExpr)(substituted) != nullptr);
        PTR(EqExpr) substEqExpr = CAST(EqExpr)(substituted);
        CHECK(substEqExpr->lhs != lhs); // Ensure the lhs has been substituted
    }

    SECTION("Testing print()") {
        ostringstream output;
        EqExpr eqExpr(NEW(NumExpr)(1), NEW(NumExpr)(1));
        eqExpr.print(output);

        CHECK(output.str() == "(1==1)");
    }

    SECTION("Testing pretty_print_at()") {
        string expected = "1==1";
        CHECK( (NEW(EqExpr)(NEW(NumExpr)(1), NEW(NumExpr)(1)))->to_pretty_string() == expected);
    }

    SECTION("Testing embedded let/if statement") {
        //_let same = 1 == 2
        //_in  _if 1 == 2
        //     _then _false + 5
        //     _else 88

        //should not throw an exception, should evaluate to 88
        PTR(Expr) equals = NEW(EqExpr)(NEW(NumExpr)(1), NEW(NumExpr)(2));

        PTR(Expr) ifExpr = NEW(IfExpr)(NEW(EqExpr)(NEW(NumExpr)(1), NEW(NumExpr)(2)), NEW(AddExpr)(NEW(BoolExpr)(false), NEW(NumExpr)(5)), NEW(NumExpr)(88));

        PTR(Expr) letExpr = NEW(LetExpr)("same", equals, ifExpr);

        CHECK(letExpr->interp()->to_string() == "88");
    }
}

TEST_CASE("Testing FunExpr") {
    
    SECTION("Testing equals()") {
        PTR(Expr) body1 = NEW( NumExpr)(1);
        PTR(Expr) body2 = NEW( NumExpr)(1);
        PTR(Expr) bodyDifferent = NEW( NumExpr)(2);

        CHECK((NEW( FunExpr) ("x", body1))->equals(NEW( FunExpr) ("x", body2)));
        CHECK(!(NEW( FunExpr) ("x", body1))->equals(NEW( FunExpr) ("y", body1)));
        CHECK(!(NEW( FunExpr) ("x", body1))->equals(NEW( FunExpr) ("x", bodyDifferent)));
    }
    
  
    SECTION("Testing print()") {
        stringstream ss;
        PTR(Expr) body = NEW( NumExpr)(3);
        PTR(FunExpr) funExpr = NEW(FunExpr)("x", body);

        funExpr->print(ss);
        assert(ss.str() == "_fun (x) 3");
    }
}

TEST_CASE("Testing CallExpr") {

    SECTION("Testing equals()") {
        PTR(Expr) toBeCalled1 = NEW(VarExpr)("f");
        PTR(Expr) actualArg1 = NEW(NumExpr)(1);
        PTR(CallExpr) callExpr1 = NEW(CallExpr)(toBeCalled1, actualArg1);

        PTR(Expr) toBeCalled2 = NEW(VarExpr)("f");
        PTR(Expr) actualArg2 = NEW(NumExpr)(1);
        PTR(CallExpr) callExpr2 = NEW(CallExpr)(toBeCalled2, actualArg2);

        PTR(Expr) differentToBeCalled = NEW(VarExpr)("g");
        PTR(CallExpr) callExprDifferentFunc = NEW(CallExpr)(differentToBeCalled, actualArg1);

        PTR(Expr) differentActualArg = NEW(NumExpr)(2);
        PTR(CallExpr) callExprDifferentArg = NEW(CallExpr)(toBeCalled1, differentActualArg);

        CHECK(callExpr1->equals(callExpr2)); // True, same structure
        CHECK(!callExpr1->equals(callExprDifferentFunc)); // False, different function
        CHECK(!callExpr1->equals(callExprDifferentArg)); // False, different argument
    }

    SECTION("Testing print()") {
        stringstream ss;
        PTR(Expr) toBeCalled = NEW(VarExpr)("f");
        PTR(Expr) actualArg = NEW(NumExpr)(3);
        PTR(CallExpr) callExpr = NEW(CallExpr)(toBeCalled, actualArg);

        callExpr->print(ss);
        CHECK(ss.str() == "(f) (3)");
    }
}



