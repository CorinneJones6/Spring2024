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
    
//    SECTION("More pretty print"){
//        //Let nested as right argument of parenthesized multiplication expression
//        CHECK ( (new Mult(new Mult(new Num (2), new Let("x", new Num(5), new Add(new Var("x") , new Num(1)) )), new Num(3)))->to_pretty_string()== "(2 * _let x = 5\n"
//        "      _in  x + 1) * 3");
//        //Let nested to the left in add expression which is nested to the right within a multiplication expression
//        CHECK((new Mult(new Num(5), new Add(new Let("x", new Num(5), new Var("x")), new Num(1))))->to_pretty_string() == "5 * ((_let x = 5\n"
//        "       _in  x) + 1)");
//        //Let in lhs of add
//        CHECK ( (new Add(new Let("x", new Num(2), new Add(new Var("x"), new Num(9))), new Num(4)))->to_pretty_string() == "(_let x = 2\n"
//        "  _in  x + 9) + 4");
//        //Let in lhs of multiplication expression
//        CHECK((new Mult(new Let("x", new Num(5), new Add(new Var("x"), new Num(8))), new Num(3)))->to_pretty_string() == "(_let x = 5\n"
//        "  _in  x + 8) * 3");
//        //Let nest as right argument of un-parenthesized multiplication expression
//        CHECK((new Add (new Mult(new Num(4), new Let("x", new Num(5), new Add(new Var("x"), new Num(1)))), new Num(9)))->to_pretty_string() == "4 * (_let x = 5\n"
//        "      _in  x + 1) + 9");
//        //Let nested to the left within let that is nested to the left within add
//        CHECK ((new Add(new Let("x", new Num(3), new Let("y", new Num(3), new Add(new Var("y"), new Num(2))) ), new Var("x")))->to_pretty_string() == "(_let x = 3\n"
//                                                                                                                                                                   "  _in  _let y = 3\n"
//                                                                                                                                                                   "       _in  y + 2) + x");
//        //Let nested in lhs of Add expression nested within body of let expression
//        CHECK((new Let("x", new Num(5), new Add(new Let("y" , new Num(3), new Add(new Var("y"), new Num(2))), new Var("x"))))
//        ->to_pretty_string() == "_let x = 5\n"
//                           " _in  (_let y = 3\n"
//                           "       _in  y + 2) + x");
//        //Triple nested let
//        CHECK( ( new Let( "x", new Num(5),
//                              new Add( new Let( "y", new Num(3),
//                                                    new Add( new Var("y"), new Let("z",new Num(6),
//                                                                                            new Add(new Var("a"), new Num(8))) ) ), new Var("x") ) ) )
//        ->to_pretty_string()== "_let x = 5\n"
//                          " _in  (_let y = 3\n"
//                          "       _in  y + _let z = 6\n"
//                          "                _in  a + 8) + x" );
//    }
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
