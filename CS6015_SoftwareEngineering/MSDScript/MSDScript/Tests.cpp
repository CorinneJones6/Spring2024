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
#include "catch.h"


TEST_CASE("NUM TESTS"){
    
    SECTION("Testing equals()") {
        CHECK( (new Num(1))->equals(new Num(1))==true );
        CHECK( (new Num(1))->equals(new Num(2))==false );
        CHECK( (new Num(INT_MAX))->equals(new Num(INT_MAX))==true );
        CHECK( (new Num(INT_MIN))->equals(new Num(INT_MIN))==true );
        CHECK( (new Num(0))->equals(new Num(0))==true);
        CHECK( (new Num(-1))->equals(new Num(-1))==true);
        CHECK( (new Num(-1))->equals(new Num(-3))==false);
    }
    
    SECTION("Testing interp()"){
        CHECK( (new Num(-1))->interp()==-1 );
        CHECK( (new Num(0))->interp()==0 );
        CHECK( (new Num(INT_MAX))->interp()==INT_MAX );
    }
    
    SECTION("Testing has_variable()"){
        CHECK( (new Num(-100))->has_variable()==false );
        CHECK( (new Num(100))->has_variable()==false );
    }
    
    SECTION("Testing subst()"){
        //tested in other tests
    }
    
    SECTION("Testing to_string()"){
        CHECK( (new Num(5))->to_string() == "5" );
        CHECK( (new Num(-3))->to_string() == "-3" );
    }
}

TEST_CASE("ADD TESTS"){
    
    SECTION("Testing equals()"){
        CHECK( (new Add(new Num(2),new Num(3)))->equals(new Add(new Num(2),new Num(3)))==true );
        CHECK( ((new Add(new Num(2),new Num(3)))->equals(new Add(new Num(3),new Num(2))))==false );
        CHECK( ((new Add(new Num(-1),new Num(-2)))->equals(new Add(new Num(-1),new Num(-2))))==true);
        CHECK( ((new Add(new Num(INT_MAX),new Num(INT_MIN)))->equals(new Add(new Num(INT_MAX),new Num(INT_MIN))))==true);
        CHECK( ((new Add(new Num(0),new Num(0)))->equals(new Add(new Num(0),new Num(0))))==true );
        CHECK( (new Add(new Num(NULL),new Num(3)))->equals(new Add(new Num(2),new Num(3)))==false );
    }
    
    SECTION("Testing interp()"){
        CHECK( (new Add(new Add(new Num(10), new Num(15)),new Add(new Num(20),new Num(20))))
              ->interp()==65);
        CHECK( (new Add(new Add(new Num(-1), new Num(-1)),new Add(new Num(-2),new Num(-2))))
              ->interp()==-6);
        CHECK( (new Add(new Add(new Num(0), new Num(0)),new Add(new Num(0),new Num(0))))
              ->interp()==0);
        CHECK( (new Add(new Add(new Num(100), new Num(100)),new Add(new Num(200),new Num(200))))
              ->interp()==600);
    }
    
    SECTION("Testing has_variable()"){
        CHECK( (new Add(new Var("x"), new Num(1)))->has_variable() == true );
        CHECK( (new Add(new Num(1), new Var("y")))->has_variable() == true );
        CHECK( (new Add(new Var("x"), new Var("y")))->has_variable() == true );
        CHECK( (new Add(new Num(0), new Num(0)))->has_variable() == false );
    }
    
    SECTION("Testing subst()"){
        CHECK( (new Add(new Var("x"), new Num(7)))
              ->subst("x", new Var("y"))
              ->equals(new Add(new Var("y"), new Num(7))) );
    }
    
    SECTION("Testing to_string()"){
        CHECK( (new Add(new Num(5), new Num(3)))->to_string() == "(5+3)" );
        CHECK( (new Add(new Num(-2), new Num(4)))->to_string() == "(-2+4)" );
    }
}

TEST_CASE("MULT TESTS"){
    
    SECTION("Testing equals()"){
        CHECK((new Mult(new Num(2),new Num(3)))->equals(new Mult(new Num(2),new Num(3)))==true );
        CHECK((new Mult(new Num(2),new Num(3)))->equals(new Mult(new Num(3),new Num(2)))==false );
        CHECK( ((new Mult(new Num(-1),new Num(-2)))->equals(new Mult(new Num(-1),new Num(-2))))==true);
        CHECK( ((new Mult(new Num(INT_MAX),new Num(INT_MIN)))->equals(new Mult(new Num(INT_MAX),new Num(INT_MIN))))==true);
        CHECK( ((new Mult(new Num(0),new Num(0)))->equals(new Mult(new Num(0),new Num(0))))==true );
        CHECK( (new Mult(new Num(NULL),new Num(3)))->equals(new Add(new Num(2),new Num(3)))==false );
    }

    SECTION("Testing interp()"){
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
    }
    
    SECTION("Testing has_variable()"){
        CHECK( (new Mult(new Num(2), new Num(1)))->has_variable() == false );
    }
    
    SECTION("Testing subst()"){
        CHECK( (new Mult(new Var("x"), new Num(7)))
              ->subst("x", new Var("y"))
              ->equals(new Mult(new Var("y"), new Num(7))) );
        CHECK( (new Var("x"))
              ->subst("x", new Mult(new Var("y"),new Num(7)))
              ->equals(new Mult(new Var("y"),new Num(7))) );
    }
    
    SECTION("Testing to_string()"){
        CHECK( (new Mult(new Num(6), new Num(7)))->to_string() == "(6*7)" );
        CHECK( (new Mult(new Num(-1), new Num(3)))->to_string() == "(-1*3)" );
    }
}

TEST_CASE("VAR TESTS"){
    
    SECTION("Testing equals()"){
        CHECK( (new Var("x"))->equals(new Var("x")) == true );
        CHECK( (new Var("x"))->equals(new Var("y")) == false );
        CHECK( (new Num(1))->equals(new Var("x")) == false );
        Var* var2 = nullptr; // Creating a nullptr
        CHECK(( new Var("c"))->equals(var2)==false );
    }
    
    SECTION("Testing interp()"){
        CHECK_THROWS_WITH( (new Var("x"))->interp(), "Var cannot be converted to a number" );
    }
    
    SECTION("Testing has_variable()"){
        CHECK( (new Var("C"))->has_variable() == true );
        CHECK( (new Var("xyz"))->has_variable() == true );
    }
    
    SECTION("Testing subst()"){
        CHECK( (new Var("x"))
              ->subst("x", new Add(new Var("y"),new Num(7)))
              ->equals(new Add(new Var("y"),new Num(7))) );
        Var* var = new Var("x");
        CHECK( (var)->subst("y", new Num(42)) == var );
    }
    
    SECTION("Testing to_string()"){
        CHECK( (new Var("x"))->to_string() == "x" );
        CHECK( (new Var("y"))->to_string() == "y" );
    }
}

TEST_CASE("Testing pretty_print()"){
    CHECK ( (new Mult(new Num(1), new Add(new Num(2), new Num(3))))->to_pretty_string() ==  "1 * (2 + 3)" );
    CHECK ( (new Mult(new Mult(new Num(8), new Num(1)), new Var("y")))->to_pretty_string() ==  "(8 * 1) * y" );
    CHECK ( (new Mult(new Add(new Num(3), new Num(5)), new Mult(new Num(6), new Num(1))))->to_pretty_string() ==  "(3 + 5) * 6 * 1" );
    CHECK ( (new Mult(new Mult(new Num(7), new Num(7)), new Add(new Num(9), new Num(2))) )->to_pretty_string() ==  "(7 * 7) * (9 + 2)" );
}

TEST_CASE("LET TESTS"){
    
    SECTION("Testing equals()"){
        //equal let expressions
        Expr* letExpr1 = new Let("x", new Num(5), new Add(new Var("x"), new Num(3)));
        Expr* letExpr2 = new Let("x", new Num(5), new Add(new Var("x"), new Num(3)));
        CHECK( letExpr1->equals(letExpr2) == true );
        
        //different lhs
        Expr* letExpr3 = new Let("x", new Num(5), new Add(new Var("x"), new Num(3)));
        Expr* letExpr4 = new Let("y", new Num(5), new Add(new Var("x"), new Num(3)));
        CHECK( letExpr3->equals(letExpr4) == false );
        
        //different rhs
        Expr* letExpr5 = new Let("x", new Num(4), new Add(new Var("x"), new Num(3)));
        Expr* letExpr6 = new Let("x", new Num(5), new Add(new Var("x"), new Num(3)));
        CHECK( letExpr5->equals(letExpr6) == false );

        //different body
        Expr* letExpr7 = new Let("x", new Num(5), new Add(new Var("x"), new Num(2)));
        Expr* letExpr8 = new Let("x", new Num(5), new Add(new Var("x"), new Num(3)));
        CHECK( letExpr7->equals(letExpr8) == false );
    }
    
    SECTION("Testing interp()"){
        Var* varX = new Var("x");
        Expr* num5 = new Num(5);
        Expr* addExpr = new Add(varX, new Num(2));
        CHECK( (new Let("x", num5, addExpr))->interp() == 7 );
    }
    
    SECTION("Testing has_variable()"){
        //rhs variable present
        CHECK( (new Let("x", new Add(new Var("y"), new Num(1)), new Num(2)))->has_variable() == true );
        
        //body variable present
        CHECK( (new Let("x", new Num(1), new Add(new Var("y"), new Num(2))))->has_variable() == true );
        
        //nested let variable present
        CHECK( (new Add (new Mult(new Num(4), new Let("x", new Num(5), new Add(new Var("x"), new Num(1)))), new Num(9)))->has_variable() == true );
        
        //nested let variable not present
        CHECK( (new Mult(new Mult(new Num (2), new Let("x", new Num(5), new Add(new Num(2) , new Num(1)) )), new Num(3)))->has_variable() == false);
        
        //nested let variable not present
        CHECK( (new Let("x", new Num(1), new Let("y", new Num(1),new Num(1))))->has_variable()==false);
        
        //no variable present
        CHECK( (new Let("x", new Num(1), new Num(2)))->has_variable() == false );
    }
    
    SECTION("Testing subst()"){
        //Substitution in rhs, not shadowed by Let's lhs
        Let* letExpr = new Let("x", new Add(new Var("y"), new Num(1)), new Var("x"));
        Expr* substituted = letExpr->subst("y", new Num(5));
        CHECK( substituted->equals(new Let("x", new Add(new Num(5), new Num(1)), new Var("x"))) == true );
        
        //Substitution shadowed by Let's lhs
        Let* letExpr1 = new Let("x", new Num(5), new Add(new Var("x"), new Num(3)));
        Expr* substituted1 = letExpr1->subst("x", new Num(10));
        CHECK( substituted1->equals(new Let("x", new Num(5), new Add(new Var("x"), new Num(3)))) == true );
        
        //Substitution in body, not shadowed
        Let* letExpr2 = new Let("x", new Num(5), new Add(new Var("y"), new Num(3)));
        Expr* substituted2 = letExpr2->subst("y", new Num(2));
        CHECK( substituted2->equals(new Let("x", new Num(5), new Add(new Num(2), new Num(3)))) == true );
    }
    
    SECTION("Testing print()"){
        std::ostringstream out;
        Let* letExpr = new Let("x", new Num(5), new Add(new Var("x"), new Num(3)));
        letExpr->print(out);
        CHECK( out.str() == "(_let x=5 _in (x+3))" );
    }
    
    SECTION("Testing pretty_print()"){
        //Nabil's tests
        Let* innerLet = new Let("y", new Num(3), new Add(new Var("y"), new Num(2)));
        Let* outerLet = new Let("x", new Num(5), new Add(innerLet, new Var("x")));
        string expected = "_let x = 5\n _in (_let y = 3\n      _in y + 2) + x";

        CHECK( outerLet->to_pretty_string()==expected );
            
        //Let nested as right argument of parenthesized multiplication expression
        CHECK ( (new Mult(new Mult(new Num (2), new Let("x", new Num(5), new Add(new Var("x") , new Num(1)) )), new Num(3)))->to_pretty_string() == "(2 * _let x = 5\n"
                                                                                        "      _in x + 1) * 3");
        
        CHECK((new Mult(new Num(5), new Add(new Let("x", new Num(5), new Var("x")), new Num(1))))->to_pretty_string() == "5 * ((_let x = 5\n"
                                                                                        "      _in x) + 1)");
        //Let in lhs of add
        CHECK ( (new Add(new Let("x", new Num(2), new Add(new Var("x"), new Num(9))), new Num(4)))->to_pretty_string() == "(_let x = 2\n"
                                                                                        " _in x + 9) + 4");
        //Let in lhs of multiplication expression
        CHECK((new Mult(new Let("x", new Num(5), new Add(new Var("x"), new Num(8))), new Num(3)))->to_pretty_string() == "(_let x = 5\n"
                                                                                        " _in x + 8) * 3");
        
        //Let nest as right argument of un-parenthesized multiplication expression
        CHECK((new Add (new Mult(new Num(4), new Let("x", new Num(5), new Add(new Var("x"), new Num(1)))), new Num(9)))->to_pretty_string() == "4 * (_let x = 5\n"
                                                                                 "      _in x + 1) + 9");
      
        CHECK( (new Let("x",new Num(5), new Add(new Let("y", new Num(3), new Add(new Var("y"), new Num(2 ))), new Var("x"))))->to_pretty_string() == "_let x = 5\n _in (_let y = 3\n      _in y + 2) + x");
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

           
