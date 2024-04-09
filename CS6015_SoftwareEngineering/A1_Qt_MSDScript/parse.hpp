#pragma once

#define parse_hpp
#include "Expr.hpp"
#include <iostream>
#include "pointer.h"

PTR(Expr) parse_str(string s);

PTR(Expr) parse(istream &in);

PTR(Expr) parse_expr(istream &in);

PTR(Expr) parse_comparg(istream &in);

PTR(Expr) parse_addend(istream &in);

static string parse_term(istream &in); 

PTR(Expr) parse_multicand(istream &in);

PTR(Expr) parse_inner(istream &in);

PTR(Expr) parse_num(istream &in);

static void consume(istream &in, int expect);

static void skip_whitespace(istream &in);

PTR(Expr) parse_var(istream &in);

static void consume_word(istream &in, string &str);

PTR(Expr) parse_let(istream &in);

PTR(Expr) parse_if(istream &in);

PTR(Expr) parse_fun(istream &in); 


