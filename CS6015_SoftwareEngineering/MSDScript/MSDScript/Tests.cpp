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
        CHECK( (new NumExpr(1))->equals(new NumExpr(1))==true );
        CHECK( (new NumExpr(1))->equals(new NumExpr(2))==false );
        CHECK( (new NumExpr(INT_MAX))->equals(new NumExpr(INT_MAX))==true );
        CHECK( (new NumExpr(INT_MIN))->equals(new NumExpr(INT_MIN))==true );
        CHECK( (new NumExpr(0))->equals(new NumExpr(0))==true);
        CHECK( (new NumExpr(-1))->equals(new NumExpr(-1))==true);
        CHECK( (new NumExpr(-1))->equals(new NumExpr(-3))==false);
    }
    
    SECTION("Testing interp()"){
        CHECK( (new NumExpr(-1))->interp()->equals(new NumVal(-1))); //check this one
        CHECK( (new NumExpr(0))->interp()->equals(new NumVal(0)) );
        CHECK( (new NumExpr(INT_MAX))->interp()->equals(new NumVal(INT_MAX)) );
    }
    
    SECTION("Testing has_variable()"){
        CHECK( (new NumExpr(-100))->has_variable()==false );
        CHECK( (new NumExpr(100))->has_variable()==false );
    }
    
    SECTION("Testing subst()"){
        //tested in other tests
    }
    
    SECTION("Testing to_string()"){
        CHECK( (new NumExpr(5))->to_string() == "5" );
        CHECK( (new NumExpr(-3))->to_string() == "-3" );
    }
}

TEST_CASE("ADD TESTS"){
    
    SECTION("Testing equals()"){
        CHECK( (new AddExpr(new NumExpr(2),new NumExpr(3)))->equals(new AddExpr(new NumExpr(2),new NumExpr(3)))==true );
        CHECK( ((new AddExpr(new NumExpr(2),new NumExpr(3)))->equals(new AddExpr(new NumExpr(3),new NumExpr(2))))==false );
        CHECK( ((new AddExpr(new NumExpr(-1),new NumExpr(-2)))->equals(new AddExpr(new NumExpr(-1),new NumExpr(-2))))==true);
        CHECK( ((new AddExpr(new NumExpr(INT_MAX),new NumExpr(INT_MIN)))->equals(new AddExpr(new NumExpr(INT_MAX),new NumExpr(INT_MIN))))==true);
        CHECK( ((new AddExpr(new NumExpr(0),new NumExpr(0)))->equals(new AddExpr(new NumExpr(0),new NumExpr(0))))==true );
        CHECK( (new AddExpr(new NumExpr(NULL),new NumExpr(3)))->equals(new AddExpr(new NumExpr(2),new NumExpr(3)))==false );
    }
    
    SECTION("Testing interp()"){
        CHECK( (new AddExpr(new AddExpr(new NumExpr(10), new NumExpr(15)),new AddExpr(new NumExpr(20),new NumExpr(20))))
              ->interp()->equals(new NumVal(65)));
        CHECK( (new AddExpr(new AddExpr(new NumExpr(-1), new NumExpr(-1)),new AddExpr(new NumExpr(-2),new NumExpr(-2))))
              ->interp()->equals(new NumVal(-6)));
        CHECK( (new AddExpr(new AddExpr(new NumExpr(0), new NumExpr(0)),new AddExpr(new NumExpr(0),new NumExpr(0))))
              ->interp()->equals(new NumVal(0)));
        CHECK( (new AddExpr(new AddExpr(new NumExpr(100), new NumExpr(100)),new AddExpr(new NumExpr(200),new NumExpr(200))))
              ->interp()->equals(new NumVal(600)));
    }
    
    SECTION("Testing has_variable()"){
        CHECK( (new AddExpr(new VarExpr("x"), new NumExpr(1)))->has_variable() == true );
        CHECK( (new AddExpr(new NumExpr(1), new VarExpr("y")))->has_variable() == true );
        CHECK( (new AddExpr(new VarExpr("x"), new VarExpr("y")))->has_variable() == true );
        CHECK( (new AddExpr(new NumExpr(0), new NumExpr(0)))->has_variable() == false );
    }
    
    SECTION("Testing subst()"){
        CHECK( (new AddExpr(new VarExpr("x"), new NumExpr(7)))
              ->subst("x", new VarExpr("y"))
              ->equals(new AddExpr(new VarExpr("y"), new NumExpr(7))) );
    }
    
    SECTION("Testing to_string()"){
        CHECK( (new AddExpr(new NumExpr(5), new NumExpr(3)))->to_string() == "(5+3)" );
        CHECK( (new AddExpr(new NumExpr(-2), new NumExpr(4)))->to_string() == "(-2+4)" );
    }
}

TEST_CASE("MULT TESTS"){
    
    SECTION("Testing equals()"){
        CHECK((new MultExpr(new NumExpr(2),new NumExpr(3)))->equals(new MultExpr(new NumExpr(2),new NumExpr(3)))==true );
        CHECK((new MultExpr(new NumExpr(2),new NumExpr(3)))->equals(new MultExpr(new NumExpr(3),new NumExpr(2)))==false );
        CHECK( ((new MultExpr(new NumExpr(-1),new NumExpr(-2)))->equals(new MultExpr(new NumExpr(-1),new NumExpr(-2))))==true);
        CHECK( ((new MultExpr(new NumExpr(INT_MAX),new NumExpr(INT_MIN)))->equals(new MultExpr(new NumExpr(INT_MAX),new NumExpr(INT_MIN))))==true);
        CHECK( ((new MultExpr(new NumExpr(0),new NumExpr(0)))->equals(new MultExpr(new NumExpr(0),new NumExpr(0))))==true );
        CHECK( (new MultExpr(new NumExpr(NULL),new NumExpr(3)))->equals(new AddExpr(new NumExpr(2),new NumExpr(3)))==false );
    }

    SECTION("Testing interp()"){
        CHECK( (new MultExpr(new NumExpr(3), new NumExpr(2)))
              ->interp()->equals(new NumVal(6)));
        CHECK( (new MultExpr(new MultExpr(new NumExpr(1), new NumExpr(2)), new MultExpr(new NumExpr(1),new NumExpr(2))))
              ->interp()->equals(new NumVal(4)));
        
        CHECK( (new MultExpr(new MultExpr(new NumExpr(0), new NumExpr(0)), new MultExpr(new NumExpr(0),new NumExpr(0))))
              ->interp()->equals(new NumVal(0)));
        
        CHECK( (new MultExpr(new MultExpr(new NumExpr(-1), new NumExpr(-1)), new MultExpr(new NumExpr(-1),new NumExpr(-1))))
              ->interp()->equals(new NumVal(1)));
        CHECK( (new MultExpr(new MultExpr(new NumExpr(-1), new NumExpr(-1)), new MultExpr(new NumExpr(-1),new NumExpr(1))))
              ->interp()->equals(new NumVal(-1)));
    }
    
    SECTION("Testing has_variable()"){
        CHECK( (new MultExpr(new NumExpr(2), new NumExpr(1)))->has_variable() == false );
    }
    
    SECTION("Testing subst()"){
        CHECK( (new MultExpr(new VarExpr("x"), new NumExpr(7)))
              ->subst("x", new VarExpr("y"))
              ->equals(new MultExpr(new VarExpr("y"), new NumExpr(7))) );
        CHECK( (new VarExpr("x"))
              ->subst("x", new MultExpr(new VarExpr("y"),new NumExpr(7)))
              ->equals(new MultExpr(new VarExpr("y"),new NumExpr(7))) );
    }
    
    SECTION("Testing to_string()"){
        CHECK( (new MultExpr(new NumExpr(6), new NumExpr(7)))->to_string() == "(6*7)" );
        CHECK( (new MultExpr(new NumExpr(-1), new NumExpr(3)))->to_string() == "(-1*3)" );
    }
}

TEST_CASE("VAR TESTS"){
    
    SECTION("Testing equals()"){
        CHECK( (new VarExpr("x"))->equals(new VarExpr("x")) == true );
        CHECK( (new VarExpr("x"))->equals(new VarExpr("y")) == false );
        CHECK( (new NumExpr(1))->equals(new VarExpr("x")) == false );
        VarExpr* var2 = nullptr; // Creating a nullptr
        CHECK(( new VarExpr("c"))->equals(var2)==false );
    }
    
    SECTION("Testing interp()"){
        CHECK_THROWS_WITH( (new VarExpr("x"))->interp(), "Var cannot be converted to a number" );
    }
    
    SECTION("Testing has_variable()"){
        CHECK( (new VarExpr("C"))->has_variable() == true );
        CHECK( (new VarExpr("xyz"))->has_variable() == true );
    }
    
    SECTION("Testing subst()"){
        CHECK( (new VarExpr("x"))
              ->subst("x", new AddExpr(new VarExpr("y"),new NumExpr(7)))
              ->equals(new AddExpr(new VarExpr("y"),new NumExpr(7))) );
        VarExpr* var = new VarExpr("x");
        CHECK( (var)->subst("y", new NumExpr(42)) == var );
    }
    
    SECTION("Testing to_string()"){
        CHECK( (new VarExpr("x"))->to_string() == "x" );
        CHECK( (new VarExpr("y"))->to_string() == "y" );
    }
}

TEST_CASE("Testing pretty_print()"){
    CHECK ( (new MultExpr(new NumExpr(1), new AddExpr(new NumExpr(2), new NumExpr(3))))->to_pretty_string() ==  "1 * (2 + 3)" );
    CHECK ( (new MultExpr(new MultExpr(new NumExpr(8), new NumExpr(1)), new VarExpr("y")))->to_pretty_string() ==  "(8 * 1) * y" );
    CHECK ( (new MultExpr(new AddExpr(new NumExpr(3), new NumExpr(5)), new MultExpr(new NumExpr(6), new NumExpr(1))))->to_pretty_string() ==  "(3 + 5) * 6 * 1" );
    CHECK ( (new MultExpr(new MultExpr(new NumExpr(7), new NumExpr(7)), new AddExpr(new NumExpr(9), new NumExpr(2))) )->to_pretty_string() ==  "(7 * 7) * (9 + 2)" );
}

TEST_CASE("LET TESTS"){
    
    SECTION("Testing equals()"){
        //equal let expressions
        Expr* letExpr1 = new LetExpr("x", new NumExpr(5), new AddExpr(new VarExpr("x"), new NumExpr(3)));
        Expr* letExpr2 = new LetExpr("x", new NumExpr(5), new AddExpr(new VarExpr("x"), new NumExpr(3)));
        CHECK( letExpr1->equals(letExpr2) == true );
        
        //different lhs
        Expr* letExpr3 = new LetExpr("x", new NumExpr(5), new AddExpr(new VarExpr("x"), new NumExpr(3)));
        Expr* letExpr4 = new LetExpr("y", new NumExpr(5), new AddExpr(new VarExpr("x"), new NumExpr(3)));
        CHECK( letExpr3->equals(letExpr4) == false );
        
        //different rhs
        Expr* letExpr5 = new LetExpr("x", new NumExpr(4), new AddExpr(new VarExpr("x"), new NumExpr(3)));
        Expr* letExpr6 = new LetExpr("x", new NumExpr(5), new AddExpr(new VarExpr("x"), new NumExpr(3)));
        CHECK( letExpr5->equals(letExpr6) == false );

        //different body
        Expr* letExpr7 = new LetExpr("x", new NumExpr(5), new AddExpr(new VarExpr("x"), new NumExpr(2)));
        Expr* letExpr8 = new LetExpr("x", new NumExpr(5), new AddExpr(new VarExpr("x"), new NumExpr(3)));
        CHECK( letExpr7->equals(letExpr8) == false );
    }
    
    SECTION("Testing interp()"){
        VarExpr* varX = new VarExpr("x");
        Expr* num5 = new NumExpr(5);
        Expr* addExpr = new AddExpr(varX, new NumExpr(2));
        CHECK( (new LetExpr("x", num5, addExpr))->interp()->equals(new NumVal(7)) );
    }
    
    SECTION("Testing has_variable()"){
        //rhs variable present
        CHECK( (new LetExpr("x", new AddExpr(new VarExpr("y"), new NumExpr(1)), new NumExpr(2)))->has_variable() == true );
        
        //body variable present
        CHECK( (new LetExpr("x", new NumExpr(1), new AddExpr(new VarExpr("y"), new NumExpr(2))))->has_variable() == true );
        
        //nested let variable present
        CHECK( (new AddExpr (new MultExpr(new NumExpr(4), new LetExpr("x", new NumExpr(5), new AddExpr(new VarExpr("x"), new NumExpr(1)))), new NumExpr(9)))->has_variable() == true );
        
        //nested let variable not present
        CHECK( (new MultExpr(new MultExpr(new NumExpr (2), new LetExpr("x", new NumExpr(5), new AddExpr(new NumExpr(2) , new NumExpr(1)) )), new NumExpr(3)))->has_variable() == false);
        
        //nested let variable not present
        CHECK( (new LetExpr("x", new NumExpr(1), new LetExpr("y", new NumExpr(1),new NumExpr(1))))->has_variable()==false);
        
        //no variable present
        CHECK( (new LetExpr("x", new NumExpr(1), new NumExpr(2)))->has_variable() == false );
    }
    
    SECTION("Testing subst()"){
        //Substitution in rhs, not shadowed by Let's lhs
        LetExpr* letExpr = new LetExpr("x", new AddExpr(new VarExpr("y"), new NumExpr(1)), new VarExpr("x"));
        Expr* substituted = letExpr->subst("y", new NumExpr(5));
        CHECK( substituted->equals(new LetExpr("x", new AddExpr(new NumExpr(5), new NumExpr(1)), new VarExpr("x"))) == true );
        
        //Substitution shadowed by Let's lhs
        LetExpr* letExpr1 = new LetExpr("x", new NumExpr(5), new AddExpr(new VarExpr("x"), new NumExpr(3)));
        Expr* substituted1 = letExpr1->subst("x", new NumExpr(10));
        CHECK( substituted1->equals(new LetExpr("x", new NumExpr(5), new AddExpr(new VarExpr("x"), new NumExpr(3)))) == true );
        
        //Substitution in body, not shadowed
        LetExpr* letExpr2 = new LetExpr("x", new NumExpr(5), new AddExpr(new VarExpr("y"), new NumExpr(3)));
        Expr* substituted2 = letExpr2->subst("y", new NumExpr(2));
        CHECK( substituted2->equals(new LetExpr("x", new NumExpr(5), new AddExpr(new NumExpr(2), new NumExpr(3)))) == true );
    }
    
    SECTION("Testing print()"){
        ostringstream out;
        LetExpr* letExpr = new LetExpr("x", new NumExpr(5), new AddExpr(new VarExpr("x"), new NumExpr(3)));
        letExpr->print(out);
        CHECK( out.str() == "(_let x=5 _in (x+3))" );
    }
    
    SECTION("Testing pretty_print()"){
        //Nabil's tests
        LetExpr* innerLet = new LetExpr("y", new NumExpr(3), new AddExpr(new VarExpr("y"), new NumExpr(2)));
        LetExpr* outerLet = new LetExpr("x", new NumExpr(5), new AddExpr(innerLet, new VarExpr("x")));
        string expected =
            "_let x = 5\n"
            "_in  (_let y = 3\n"
            "      _in  y + 2) + x";

        CHECK( outerLet->to_pretty_string()==expected );
            
        //Let nested as right argument of parenthesized multiplication expression
        CHECK ( (new MultExpr(new MultExpr(new NumExpr (2), new LetExpr("x", new NumExpr(5), new AddExpr(new VarExpr("x") , new NumExpr(1)) )), new NumExpr(3)))->to_pretty_string()
               ==
               "(2 * _let x = 5\n"
               "     _in  x + 1) * 3");
        
        CHECK((new MultExpr(new NumExpr(5), new AddExpr(new LetExpr("x", new NumExpr(5), new VarExpr("x")), new NumExpr(1))))->to_pretty_string()
              ==
              "5 * ((_let x = 5\n"
              "      _in  x) + 1)");
        //Let in lhs of add
        CHECK ( (new AddExpr(new LetExpr("x", new NumExpr(2), new AddExpr(new VarExpr("x"), new NumExpr(9))), new NumExpr(4)))->to_pretty_string()
               ==
               "(_let x = 2\n"
               " _in  x + 9) + 4");
        //Let in lhs of multiplication expression
        CHECK((new MultExpr(new LetExpr("x", new NumExpr(5), new AddExpr(new VarExpr("x"), new NumExpr(8))), new NumExpr(3)))->to_pretty_string()
              ==
              "(_let x = 5\n"
              " _in  x + 8) * 3");
        
        //Let nest as right argument of un-parenthesized multiplication expression
        CHECK((new AddExpr (new MultExpr(new NumExpr(4), new LetExpr("x", new NumExpr(5), new AddExpr(new VarExpr("x"), new NumExpr(1)))), new NumExpr(9)))->to_pretty_string()
              ==
              "4 * (_let x = 5\n"
              "     _in  x + 1) + 9");
      
        CHECK( (new LetExpr("x",new NumExpr(5), new AddExpr(new LetExpr("y", new NumExpr(3), new AddExpr(new VarExpr("y"), new NumExpr(2 ))), new VarExpr("x"))))->to_pretty_string()
              ==
              "_let x = 5\n"
              "_in  (_let y = 3\n"
              "      _in  y + 2) + x" );
    }
    
    SECTION("More pretty print"){
        
        //Let nested as right argument of parenthesized multiplication expression
        CHECK ( (new MultExpr(new MultExpr(new NumExpr(2), new LetExpr("x", new NumExpr(5), new AddExpr(new VarExpr("x") , new NumExpr(1)) )), new NumExpr(3)))->to_pretty_string()
               ==
               "(2 * _let x = 5\n"
               "     _in  x + 1) * 3");
        
        //Let nested to the left in add expression which is nested to the right within a multiplication expression
        CHECK((new MultExpr(new NumExpr(5), new AddExpr(new LetExpr("x", new NumExpr(5), new VarExpr("x")), new NumExpr(1))))->to_pretty_string()
              ==
              "5 * ((_let x = 5\n"
              "      _in  x) + 1)");
        
        //Let in lhs of add
        CHECK ( (new AddExpr(new LetExpr("x", new NumExpr(2), new AddExpr(new VarExpr("x"), new NumExpr(9))), new NumExpr(4)))->to_pretty_string()
               ==
               "(_let x = 2\n"
               " _in  x + 9) + 4");

        //Let in lhs of multiplication expression
        CHECK((new MultExpr(new LetExpr("x", new NumExpr(5), new AddExpr(new VarExpr("x"), new NumExpr(8))), new NumExpr(3)))->to_pretty_string()
              ==
              "(_let x = 5\n"
              " _in  x + 8) * 3");

        //Let nest as right argument of un-parenthesized multiplication expression
        CHECK((new AddExpr(new MultExpr(new NumExpr(4), new LetExpr("x", new NumExpr(5), new AddExpr(new VarExpr("x"), new NumExpr(1)))), new NumExpr(9)))->to_pretty_string()
              ==
              "4 * (_let x = 5\n"
              "     _in  x + 1) + 9");

        //Let nested to the left within let that is nested to the left within add
        CHECK ((new AddExpr(new LetExpr("x", new NumExpr(3), new LetExpr("y", new NumExpr(3), new AddExpr(new VarExpr("y"), new NumExpr(2))) ), new VarExpr("x")))->to_pretty_string()
               ==
               "(_let x = 3\n"
               " _in  _let y = 3\n"
               "      _in  y + 2) + x");
        
        //Let nested in lhs of Add expression nested within body of let expression
        CHECK((new LetExpr("x", new NumExpr(5), new AddExpr(new LetExpr("y" , new NumExpr(3), new AddExpr(new VarExpr("y"), new NumExpr(2))), new VarExpr("x"))))->to_pretty_string()
              ==
              "_let x = 5\n"
              "_in  (_let y = 3\n"
              "      _in  y + 2) + x");
        
        //Triple nested let
        CHECK( ( new LetExpr( "x", new NumExpr(5), new AddExpr( new LetExpr( "y", new NumExpr(3), new AddExpr( new VarExpr("y"), new LetExpr("z", new NumExpr(6),new AddExpr(new VarExpr("a"), new NumExpr(8))) ) ), new VarExpr("x") ) ) )->to_pretty_string()
              ==
              "_let x = 5\n"
              "_in  (_let y = 3\n"
              "      _in  y + _let z = 6\n"
              "               _in  a + 8) + x" );
    }
}

TEST_CASE("parse") {
  CHECK_THROWS_WITH( parse_str("()"), "invalid input" );
  
  CHECK( parse_str("(1)")->equals(new NumExpr(1)) );
  CHECK( parse_str("(((1)))")->equals(new NumExpr(1)) );
  
  CHECK_THROWS_WITH( parse_str("(1"), "missing close parenthesis" );
  
  CHECK( parse_str("1")->equals(new NumExpr(1)) );
  CHECK( parse_str("10")->equals(new NumExpr(10)) );
  CHECK( parse_str("-3")->equals(new NumExpr(-3)) );
  CHECK( parse_str("  \n 5  ")->equals(new NumExpr(5)) );
  CHECK_THROWS_WITH( parse_str("-"), "not a num" );
  CHECK_THROWS_WITH( parse_str(" -   5  "), "not a num" );
  
  CHECK( parse_str("x")->equals(new VarExpr("x")) );
  CHECK( parse_str("xyz")->equals(new VarExpr("xyz")) );
  CHECK( parse_str("xYz")->equals(new VarExpr("xYz")) );
  CHECK_THROWS_WITH( parse_str("x_z"), "invalid input" );
  
  CHECK( parse_str("x + y")->equals(new AddExpr(new VarExpr("x"), new VarExpr("y"))) );

  CHECK( parse_str("x * y")->equals(new MultExpr(new VarExpr("x"), new VarExpr("y"))) );

  CHECK( parse_str("z * x + y")
        ->equals(new AddExpr(new MultExpr(new VarExpr("z"), new VarExpr("x")),
                         new VarExpr("y"))) );
  
  CHECK( parse_str("z * (x + y)")
        ->equals(new MultExpr(new VarExpr("z"),
                          new AddExpr(new VarExpr("x"), new VarExpr("y"))) ));
    
  CHECK( parse_str("_let x = 5 _in x + 3")
           ->equals(new LetExpr("x", new NumExpr(5), new AddExpr(new VarExpr("x"), new NumExpr(3)))) );
    
  CHECK( parse_str("_let x = 2 _in _let y = x + 3 _in y * x") ->equals(new LetExpr("x", new NumExpr(2),
        new LetExpr("y", new AddExpr(new VarExpr("x"), new NumExpr(3)), new MultExpr(new VarExpr("y"), new VarExpr("x"))))) );

}

TEST_CASE("Testing NumVal") {
    
    SECTION("to_expr"){
        CHECK( (new NumVal(1))->to_expr()->equals((new NumExpr(1)))==true);
        CHECK( (new NumVal(1))->to_expr()->equals((new NumExpr(2)))==false);
    }
    
    SECTION("equals"){
        CHECK( (new NumVal(1))->equals(new NumVal(1))==true );
        CHECK( (new NumVal(1))->equals(new NumVal(2))==false );
    }
    
    SECTION("add_to"){
        Val* result = (new NumVal(5))->add_to(new NumVal(10));
        CHECK(result->to_string()=="15");
        
        Val* result2 = (new NumVal(-1))->add_to(new NumVal(-1));
        CHECK(result2->to_string()=="-2");
        
        Val* result3 = (new NumVal(0))->add_to(new NumVal(0));
        CHECK(result3->to_string()=="0");
        
        NumVal a(5);
        Val* nonNumVal = reinterpret_cast<Val*>(new LetExpr("x", new NumExpr(1), new NumExpr(1)));
        REQUIRE_THROWS_AS(a.add_to(nonNumVal), runtime_error);

    }
    
    SECTION("mult_with"){
        Val* result = (new NumVal(5))->mult_with(new NumVal(10));
        CHECK(result->to_string()=="50");
        
        Val* result2 = (new NumVal(-1))->mult_with(new NumVal(1));
        CHECK(result2->to_string()=="-1");
        
        Val* result3 = (new NumVal(0))->mult_with(new NumVal(0));
        CHECK(result3->to_string()=="0");
        
        NumVal a(5);
        Val* nonNumVal = reinterpret_cast<Val*>(new LetExpr("x", new NumExpr(1), new NumExpr(1)));
        REQUIRE_THROWS_AS(a.mult_with(nonNumVal), runtime_error);
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
        
        CHECK(outputTrue.str() == "1");
        CHECK(outputFalse.str() == "0");
    }
    
    SECTION("to_expr"){
        BoolVal boolVal(true);
        Expr* expr = boolVal.to_expr();
        ostringstream output;
        expr->print(output);
        CHECK(output.str() == "_true");
        
        BoolVal boolVal2(false);
        Expr* expr2 = boolVal2.to_expr();
        ostringstream output2;
        expr2->print(output2);
        CHECK(output2.str() == "_false");
    }
    
    SECTION("equals") {
        CHECK( (new BoolVal(true))->equals(new BoolVal(true))==true );
        CHECK( (new BoolVal(true))->equals(new BoolVal(false))==false );
    }
    
    SECTION("add_to") {
            BoolVal boolVal(true);
            BoolVal anotherBoolVal(false);
            
            REQUIRE_THROWS_AS(boolVal.add_to(&anotherBoolVal), runtime_error);
    }
    
    SECTION("mult_with") {
          REQUIRE_THROWS_AS((new BoolVal(true))->mult_with(new BoolVal(false)), runtime_error);
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
        CHECK( (new BoolExpr(true))->equals(new BoolExpr(true)) );
        CHECK_FALSE( (new BoolExpr(true))->equals(new BoolExpr(false)) );
    }
    
    SECTION("interp") {
        CHECK( (new BoolExpr(true))->interp()->equals(new BoolVal(true)) );
        CHECK( (new BoolExpr(false))->interp()->equals(new BoolVal(false)) );
    }
}

TEST_CASE("Testing IfExpr") {
    
    SECTION("Testing constructor") {
        Expr* condition = new BoolExpr(true);
        Expr* thenBranch = new NumExpr(1);
        Expr* elseBranch = new NumExpr(0);
        IfExpr ifExpr(condition, thenBranch, elseBranch);

        CHECK(ifExpr.if_ == condition);
        CHECK(ifExpr.then_ == thenBranch);
        CHECK(ifExpr.else_ == elseBranch);
    }
    
    SECTION("Testing equals()") {
        Expr* ifExpr1 = (new IfExpr(new BoolExpr(true), new NumExpr(1), new NumExpr(0)));
        Expr* ifExpr2 = new IfExpr(new BoolExpr(true), new NumExpr(1), new NumExpr(0));
        Expr* ifExpr3 = new IfExpr(new BoolExpr(false), new NumExpr(2), new NumExpr(3));

        CHECK(ifExpr1->equals(ifExpr2) == true);
        CHECK(ifExpr1->equals(ifExpr3) == false);
    }
    
    SECTION("Testing interp()") {
        Expr* conditionTrue = new BoolExpr(true);
        Expr* conditionFalse = new BoolExpr(false);
        Expr* thenBranch = new NumExpr(1);
        Expr* elseBranch = new NumExpr(0);

        CHECK( (new IfExpr(conditionTrue, thenBranch, elseBranch))->interp()->equals(new NumVal(1)) );
//        CHECK( (new IfExpr(conditionFalse, thenBranch, elseBranch))->interp()->equals(new NumVal(0)) );
    }
    
    SECTION("Testing has_variable()") {
        CHECK((new IfExpr(new VarExpr("x"), new NumExpr(1), new NumExpr(2)))->has_variable() == true);
        CHECK((new IfExpr(new BoolExpr(true), new NumExpr(1), new NumExpr(2)))->has_variable() == false);
    }
    
    SECTION("Testing subst()") {
        Expr* condition = new VarExpr("x");
        Expr* thenBranch = new NumExpr(1);
        Expr* elseBranch = new NumExpr(2);
        IfExpr* ifExpr = new IfExpr(condition, thenBranch, elseBranch);

        Expr* substituted = ifExpr->subst("x", new BoolExpr(true));

        CHECK(dynamic_cast<IfExpr*>(substituted) != nullptr);
        IfExpr* substIfExpr = dynamic_cast<IfExpr*>(substituted);
        CHECK(substIfExpr->if_ != condition); // Ensure the condition has been substituted
    }
    
    SECTION("Testing print()") {
        ostringstream output;
        IfExpr ifExpr(new BoolExpr(true), new NumExpr(1), new NumExpr(0));
        ifExpr.print(output);

        CHECK(output.str() == "(_if_true_then1_else0)");
    }
    
    SECTION("Testing pretty_print_at()") {
        string expected =
              "_if _true\n"
              "_then 1\n"
              "_else 0\n";
        
        CHECK( (new IfExpr(new BoolExpr(true), new NumExpr(1), new NumExpr(0)))->to_pretty_string() == expected);
    }
    
    SECTION("pretty_print"){
        
        CHECK( (new IfExpr(new EqExpr(new VarExpr("x"), new NumExpr(1)), new NumExpr(1), new NumExpr(2)))->to_pretty_string()
              ==
              "_if x==1\n"
              "_then 1\n"
              "_else 2\n" );
    }
}

TEST_CASE("Testing EqExpr") {
    
    SECTION("Testing constructor") {
        Expr* lhs = new NumExpr(1);
        Expr* rhs = new NumExpr(1);
        EqExpr eqExpr(lhs, rhs);
        
        CHECK(eqExpr.lhs == lhs);
        CHECK(eqExpr.rhs == rhs);
    }
    
    SECTION("Testing equals()") {
        Expr* eqExpr1 = new EqExpr(new NumExpr(1), new NumExpr(1));
        Expr* eqExpr2 = new EqExpr(new NumExpr(1), new NumExpr(1));
        Expr* eqExpr3 = new EqExpr(new NumExpr(1), new NumExpr(2));
        
        CHECK(eqExpr1->equals(eqExpr2) == true);
        CHECK(eqExpr1->equals(eqExpr3) == false);
    }
    
    SECTION("Testing interp()") {
        Expr* lhs = new NumExpr(1);
        Expr* rhs = new NumExpr(1);
        Expr* eqExprTrue = new EqExpr(lhs, rhs);
        
        Expr* rhsFalse = new NumExpr(2);
        Expr* eqExprFalse = new EqExpr(lhs, rhsFalse);
        
        CHECK(dynamic_cast<BoolVal*>(eqExprTrue->interp())->equals(new BoolVal(true)));
        CHECK(dynamic_cast<BoolVal*>(eqExprFalse->interp())->equals(new BoolVal(false)));
    }
    
    SECTION("Testing has_variable()") {
        CHECK((new EqExpr(new VarExpr("x"), new NumExpr(1)))->has_variable() == true);
        CHECK((new EqExpr(new NumExpr(1), new NumExpr(2)))->has_variable() == false);
    }
    
    SECTION("Testing subst()") {
        Expr* lhs = new VarExpr("x");
        Expr* rhs = new NumExpr(1);
        EqExpr* eqExpr = new EqExpr(lhs, rhs);
        
        Expr* substituted = eqExpr->subst("x", new NumExpr(2));
        
        CHECK(dynamic_cast<EqExpr*>(substituted) != nullptr);
        EqExpr* substEqExpr = dynamic_cast<EqExpr*>(substituted);
        CHECK(substEqExpr->lhs != lhs); // Ensure the lhs has been substituted
    }
    
    SECTION("Testing print()") {
        ostringstream output;
        EqExpr eqExpr(new NumExpr(1), new NumExpr(1));
        eqExpr.print(output);
        
        CHECK(output.str() == "(1==1)");
    }
    
    SECTION("Testing pretty_print_at()") {
        string expected = "1==1";
        CHECK( (new EqExpr(new NumExpr(1), new NumExpr(1)))->to_pretty_string() == expected);
    }
    
    SECTION("HW9 Assignment Example") {
        //_let same = 1 == 2
        //_in  _if 1 == 2
        //     _then _false + 5
        //     _else 88

        // Constructing the inner equality expression 1 == 2
        Expr* equality = new EqExpr(new NumExpr(1), new NumExpr(2));

        // Constructing the if-expression part
        Expr* ifExpr = new IfExpr(
                new EqExpr(new NumExpr(1), new NumExpr(2)), // condition: 1 == 2
                new AddExpr(new BoolExpr(false), new NumExpr(5)), // then: _false + 5 (This is expected not to be evaluated)
                new NumExpr(88) // else: 88
        );

        // Constructing the let-expression
        Expr* letExpr = new LetExpr(
                "same", // variable name
                equality, // binding expression: 1 == 2
                ifExpr // body of the let-expression
        );

        // Interpreting the let-expression
        Val* result = letExpr->interp();

        // Convert the result to a NumVal to extract its value
        NumVal* numResult = dynamic_cast<NumVal*>(result);

        CHECK(numResult->to_string() == "88"); // Verify the result is 88
    }
}


TEST_CASE("HW8: Refactor interp + Val class") {

    SECTION("testing_let_parse") {
        CHECK(parse_str(("_let x=5 _in (x+7)"))->equals(
                new LetExpr("x", new NumExpr(5), new AddExpr(new VarExpr("x"), new NumExpr(7)))));

        CHECK(parse_str(("_let x=5 _in (x+7)"))->equals(
                new LetExpr("x", new NumExpr(5), new AddExpr(new VarExpr("x"), new NumExpr(7)))));
        CHECK(parse_str(("_let x=10 _in (y+10)"))->equals(
                new LetExpr("x", new NumExpr(10), new AddExpr(new VarExpr("y"), new NumExpr(10)))));
        CHECK(parse_str(("(_let x=5 _in ((_let y=3 _in (y+2))+x))"))
                      ->equals(new LetExpr("x", new NumExpr(5),
                                        new AddExpr(new LetExpr("y", new NumExpr(3), new AddExpr(new VarExpr("y"), new NumExpr(2))),
                                                new VarExpr("x")))));
        CHECK(parse_str(("(_let x=5 _in (x+7))"))->equals(
                (new LetExpr("x", new NumExpr(5), new AddExpr(new VarExpr("x"), new NumExpr(7))))));
    }

    SECTION("Num interprets to NumVal", "[NumVal]") {
        Expr *num = new NumExpr(5);
        Val *expected = new NumVal(5);
        REQUIRE(num->interp()->equals(expected));
    }

    SECTION("Add interprets correctly", "[Add]") {
        Expr *num1 = new NumExpr(5);
        Expr *num2 = new NumExpr(3);
        Expr *addExpr = new AddExpr(num1, num2);
        Val *expected = new NumVal(8);
        REQUIRE(addExpr->interp()->equals(expected));
    }
    SECTION("Mult interprets correctly", "[Mult]") {
        Expr *num1 = new NumExpr(4);
        Expr *num2 = new NumExpr(2);
        Expr *multExpr = new MultExpr(num1, num2);
        Val *expected = new NumVal(8);
        REQUIRE(multExpr->interp()->equals(expected));
    }
    SECTION("VarExpr throws exception on interp", "[VarExpr]") {
        Expr *varExpr = new VarExpr("x");
        REQUIRE_THROWS_AS(varExpr->interp(), std::runtime_error);
    }
    SECTION("NumVal equals works correctly", "[NumVal]") {
        Val *val1 = new NumVal(10);
        Val *val2 = new NumVal(10);
        Val *val3 = new NumVal(5);
        REQUIRE(val1->equals(val2));
        REQUIRE_FALSE(val1->equals(val3));
    }
}

TEST_CASE("HW9 Conditionals"){
    SECTION("(1 + 2) evaluated to 3", "[NumVal]") {
        //1 + 2 -> 3
        CHECK( (new AddExpr(new NumExpr(1), new NumExpr(2)))->interp()->equals(new NumVal(3)) );
    }
    SECTION("_let x = 2+3 _in x*x evaluated to 25", "[NumVal] and [_let]"){
        //_let x = 2+3
        //_in x*x
        CHECK( (new LetExpr("x",
                            new AddExpr(new NumExpr(2), new NumExpr(3)),
                            new MultExpr(new VarExpr("x"), new VarExpr("x"))))
                       ->interp()
                       ->equals(new NumVal(25)) );
    }
    SECTION("_if _true _then 1 _else 2 evaluated to 1", "[IfExpr] and [BoolExpr]"){
        //_if _true
        //_then 1
        //_else 2
        CHECK( (new IfExpr(new BoolExpr(true),
                           new NumExpr(1),
                           new NumExpr(2)))->interp()
                       ->equals(new NumVal(1)) );
    }
    SECTION("(1 == 2) + 3 throws an exception", "[Add]") {
        Expr* testExpr = new AddExpr(new EqExpr(new NumExpr(1), new NumExpr(2)), new NumExpr(3));
        CHECK_THROWS_WITH(testExpr->interp(), "Bool cannot be added");
    }

    SECTION("1 == 2 + 3 evaluates to _false", "[EqExpr]") {
        Expr* testExpr = new EqExpr(new NumExpr(1), new AddExpr(new NumExpr(2), new NumExpr(3)));
        Val* result = testExpr->interp();
        CHECK((result)->to_string() == "1");

    }

    SECTION("1 + 1 == 2 + 0 evaluates to _true", "[EqExpr]") {
        Expr* testExpr = new EqExpr(new AddExpr(new NumExpr(1), new NumExpr(1)), new AddExpr(new NumExpr(2), new NumExpr(0)));
        Val* result = testExpr->interp();
        CHECK((result)->to_string() == "1");
    }
}

//TEST_CASE("HW9-Parsing") {
//    SECTION("Parsing BoolExpr") {
//        CHECK(parse_str(("_true"))->equals(new BoolExpr(true)));
//        CHECK(parse_str(("_false"))->equals(new BoolExpr(false)));
//    }
//
//    SECTION("Parsing _if") {
//        CHECK(parse_str(("_if _true _then 1 _else 2"))->equals(
//                new IfExpr(new BoolExpr(true), new Num(1), new Num(2))));
//        CHECK(parse_str(("_if _false _then 1 _else 2"))->equals(
//                new IfExpr(new BoolExpr(false), new Num(1), new Num(2))));
//        CHECK(parse_str(("_if 1 == 2 _then 1 _else 2"))->equals(
//                new IfExpr(new EqExpr(new Num(1), new Num(2)), new Num(1), new Num(2))));
//    }
//    SECTION("Equality parsing") {
//        CHECK( parse_str("1 == 2")->interp()->equals(new BoolVal(false)) );
//        CHECK((((parse_str("_if 1 == 2 _then 5 _else 6"))->interp())->to_string()) == "6");
//        CHECK((parse_str("1 + 2 == 3 + 0"))->interp()->to_string() == "_true");
//
//    }
//}


