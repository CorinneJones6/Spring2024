#ifndef parse_hpp
#include "Expr.hpp"
#define parse_hpp

#include <stdio.h>

#endif

Expr * parse_expr(istream &in); 

Expr *parse_num(istream &in);

static void consume(istream &in, int expect);

static void skip_whitespace(istream &in); 
