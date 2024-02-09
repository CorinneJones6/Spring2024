#include <iostream>
#include "parse.hpp"

Expr * parse_expr(istream &in){
    skip_whitespace(in);
    
    int c = in.peek();
    
    if(c == '-' || isdigit(c)){
        return parse_num(in);
    } else if(c=='('){
        consume(in, '(');
        Expr *e = parse_expr(in);
        skip_whitespace(in);
        c = in.get();
        if(c!=')'){
            throw runtime_error("missing closing parenthesis"); 
        }
        return e;
    }
    else{
        consume(in, c);
        throw runtime_error("invalid input");
    }
}

Expr *parse_num(istream &in) {
    
    int n = 0;
    
    bool negative = false;
    
    if (in.peek() == '-'){
        negative = true;
        consume(in, '-');
    }
    
    while (1) {
        int c = in.peek(); //gets each char from input stream, DOES NOT remove

        if (isdigit(c))
        {
            consume (in, c);
            n = n * 10 + (c - '0'); //converts a string of values to an integer of that value (i.e., '1''2''3' -> 123)
        }
        else {
            break;
        }
    }
    
    if (negative){
        n = -n;
    }
    
    return new Num(n); //once converted to int, can wrap inside an obj
}

static void consume(istream &in, int expect){
    int c = in.get();
    if(c!= expect){
        throw runtime_error("consume mismatch");
    }
}

static void skip_whitespace(istream &in){
    while(1){
        int c =in.peek();
        if(!isspace(c)){
            break;
        }
        consume(in, c);
    }
}
