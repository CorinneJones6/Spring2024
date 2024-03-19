#pragma once

#define parse_hpp
#include "Expr.hpp"
#include <iostream>

Expr *parse_str(string s);

Expr *parse(istream &in);

Expr *parse_expr(istream &in);

Expr *parse_comparg(istream &in);

Expr *parse_addend(istream &in);

static string parse_term(istream &in); 

Expr *parse_multicand(istream &in);

Expr* parse_inner(istream &in);

Expr *parse_num(istream &in);

static void consume(istream &in, int expect);

static void skip_whitespace(istream &in);

Expr* parse_input();

Expr *parse_var(istream &in);

static void consume_word(istream &in, string str);

Expr *parse_let(istream &in);

Expr* parse_if(istream &in);

Expr* parse_fun(istream &in); 


