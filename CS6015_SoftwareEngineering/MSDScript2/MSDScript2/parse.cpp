#include "parse.hpp"

/**
 * Parses an expression from a string.
 *
 * \param s The string from which the expression is parsed.
 * \return Pointer to the parsed expression object.
 */
Expr* parse_str(string s){
    istringstream in(s);
    return parse(in);
}

/**
 * Parses an expression from an input stream and verifies that the end of the file (stream) is reached.
 *
 * \param in Reference to input stream from which the expression is read.
 * \return Pointer to the parsed expression object.
 */
Expr* parse(istream &in) {
    Expr* e;
    e = parse_expr(in);
    skip_whitespace(in);
    if ( !in.eof() ) {
        throw runtime_error("invalid input") ;
    }
    
    return e;
}

Expr *parse_expr(istream &in){
    Expr* e = parse_comparg(in);
    skip_whitespace(in);
    if(in.peek() == '='){
        consume(in, '=');
        if(in.peek() != '='){
            throw runtime_error("need '==' to indicate equal check") ;
        }
        consume(in, '=');
        Expr* rhs = parse_expr(in);
        return new EqExpr(e, rhs);
    }
    return e;
}

/**
 * Parses general expressions that can include addition from an input stream.
 *
 * \param in Reference to input stream from which the expression is read.
 * \return Pointer to the parsed expression object.
 */
Expr* parse_comparg(istream &in) {
    
    Expr *e = parse_addend(in);

    skip_whitespace(in);

    if (in.peek() == '+') {
        consume(in, '+');
        Expr *rhs = parse_comparg(in);
        return new AddExpr(e, rhs);
    }
    return e;
}

/**
 * Parses addends in expressions, handling multiplication as higher priority.
 *
 * \param in Reference to input stream from which the addend is read.
 * \return Pointer to the parsed expression object.
 */
Expr* parse_addend(istream &in) {
    
    Expr *e = parse_multicand(in);

    skip_whitespace(in);

    int c = in.peek();
    if (c == '*') {
        consume(in, '*');
        skip_whitespace(in) ;
        Expr *rhs = parse_addend(in);
        return new MultExpr(e, rhs);
    }
    
    return e ;

}

static string parse_term(istream &in){
    string term;
    while (true) {
         int letter = in.peek();
         if (isalpha(letter)) {
             consume(in, letter);
             char c = letter;
             term += c;
         }
         else
             break;
     }
    return term;
}

Expr* parse_multicand(istream &in){
    Expr *expr = parse_inner(in);

    while(in.peek() == '('){
        consume(in, '(');
        Expr* actual_arg = parse_expr(in);
        consume(in, ')');
        return new CallExpr(expr, actual_arg);
    }
    return expr;
}

/**
 * Parses expressions with multiplication and division candidates from an input stream.
 *
 * \param in Reference to input stream from which the expression is read.
 * \return Pointer to the parsed expression object.
 */
Expr* parse_inner(istream &in) {
    skip_whitespace(in);
    int c = in.peek();
    
    if ((c == '-') || isdigit(c)){
        return parse_num(in);
    }
    
    else if (c == '(') {
        consume(in, '(');
        Expr *e = parse_comparg(in);
        skip_whitespace(in);
        c = in.get();
        if (c != ')'){
            throw runtime_error("missing close parenthesis");
        }
        return e;
    }
    
    else if (isalpha(c)) {
        return parse_var(in);
    }
    
    else if (c=='_'){
        consume(in, '_');
    
        string term = parse_term(in);
     
        if(term == "let"){
            return parse_let(in);
        }
        else if(term == "true"){
            return new BoolExpr(true);
        }
        else if(term == "false"){
            return new BoolExpr(false);
        }
        else if(term == "if"){
            return parse_if(in);
        }
        else if(term == "fun"){
            return parse_fun(in);
        }
        else{
            throw runtime_error("invalid input");
        }
    }
    else {
        consume(in, c);
        throw runtime_error("invalid input");
    }
}

/**
 * Parses numeric values from an input stream.
 *
 * \param in Reference to input stream from which the number is read.
 * \return Pointer to the created Num object representing the parsed number.
 */
Expr* parse_num(istream &in) {
    int n = 0;
    bool negative = false;

    if (in.peek() == '-') {
        negative = true;

        consume (in, '-');
        if(!isdigit(in.peek())){
            throw runtime_error("not a num") ;
        }
    }

    while (1) {
        int c = in.peek();
        if (isdigit(c))
        {
            consume(in, c);
            n = n*10 + (c - '0');
        }
        else {
            break;
        }
    }

    if (negative)
        n = n * -1;
    return new NumExpr(n);
}

/**
 * Consumes a specific expected character from the input stream or throws an error if it doesn't match.
 *
 * \param in Reference to input stream from which the character is consumed.
 * \param expect The character expected to be consumed.
 */
static void consume(istream &in, int expect) {
    int c = in.get();
    if (c!=expect) {
        throw runtime_error("consume mismatch");
    }
}


/**
 * Skips whitespace characters in the input stream.
 *
 * \param in Reference to input stream from which whitespace is skipped.
 */
static void skip_whitespace(istream &in) {
    while (1) {
        int c = in.peek();
        if (!isspace(c))
            break;
        consume(in, c);
    }
}

/**
 * Parses an expression from standard input.
 *
 * \return Pointer to the parsed expression object.
 */
Expr* parse_input(){
    string input;
    getline( cin, input);
    cout << "input: " << input << endl;
    stringstream ss(input);
    return parse_comparg(ss);
}

/**
 * Parses variable names from an input stream.
 *
 * \param in Reference to input stream from which the variable name is read.
 * \return Pointer to the Var object representing the parsed variable.
 */
Expr* parse_var(istream &in) {
    string str;
    while (true) {
        int c = in.peek();
        if (isalpha(c)) {
            consume(in, c);
            str += static_cast<char>(c);
        } else {
            break;
        }
    }
    return new VarExpr(str);
}

/**
 * \brief Consumes a specified string from the input stream.
 *
 * This function reads characters from the input stream until it matches
 * the characters in the given string, throwing a runtime_error if a
 * mismatch is encountered.
 *
 * \param in The input stream to consume characters from.
 * \param str The string to be consumed from the input stream.
 * \throws runtime_error If a character mismatch occurs during consumption.
 */
static void consume_word(istream &in, string str){
    for(char c : str){
        if (in.get()!=c){
            throw runtime_error("consume mismatch");
        }
    }
}

/**
 * \brief Parses a _let expression from the input stream.
 *
 * This function parses a _let expression from the input stream, constructing
 * a Let object with the parsed components.
 *
 * \param in The input stream to parse from.
 * \return A pointer to the Let expression object.
 */
Expr* parse_let(istream &in){
    
    skip_whitespace(in);
    
    Expr *e = parse_var(in);
    
    string lhs = e->to_string();
    
    skip_whitespace(in);
    
    consume(in, '=');
    
    skip_whitespace(in);
    
    Expr *rhs = parse_comparg(in);
    
    skip_whitespace(in);
    
    consume_word(in, "_in");
    
    skip_whitespace(in);
    
    Expr *body = parse_comparg(in);
    
    return new LetExpr(lhs, rhs, body);
}

Expr* parse_if(istream &in){
    skip_whitespace(in);
    
    Expr* ifStatement = parse_expr(in);
    
    skip_whitespace(in);
    
    consume_word(in, "_then");
    
    skip_whitespace(in);
    
    Expr* thenStatement = parse_expr(in);
    
    skip_whitespace(in);
    
    consume_word(in, "_else");
    
    skip_whitespace(in);
    
    Expr* elseStatment = parse_expr(in);
    
    return new IfExpr(ifStatement, thenStatement, elseStatment);
}

Expr* parse_fun(istream &in){
    skip_whitespace(in);
    
    consume(in, '(');
    
    Expr *e = parse_var(in);
    
    string var = e->to_string();
    
    consume(in, ')');
    
    skip_whitespace(in);
    
    e = parse_expr(in);
    
    return new FunExpr(var, e);
}



