/**
 * \file Expr.cpp
 * \brief Implementation of expression classes for arithmetic operations.
 *
 * Provides the implementation for various expressions including addition, multiplication,
 * numbers, and variables within an expression evaluation context.
 *
 * Created by Corinne Jones on 1/16/24.
 */

#include "Expr.hpp"

//====================== EXPR ======================//

/**
 * \brief Converts expression to string representation.
 * \return String representation of the expression.
 */
string Expr::to_string(){
    stringstream st("");
    this->print(st);
    return st.str();
}

/**
 * \brief Pretty prints the expression at a given precedence.
 * \param ostream The output stream to print to.
 * \param prec The precedence context in which to print.
 */
void Expr::pretty_print_at(ostream &ostream, precedence_t prec){
    print(ostream);
}

/**
 * \brief Pretty prints the expression.
 * \param ostream The output stream.
 */
void Expr::pretty_print(ostream &ostream){
    pretty_print_at(ostream, prec_none);
}

/**
 * \brief Converts the expression to a pretty string.
 * \return A pretty string representation of the expression.
 */
string Expr::to_pretty_string(){
    stringstream st("");
    this->pretty_print(st);
    return st.str();
}

//======================  ADD  ======================//

/**
 * \brief Constructs an addition expression.
 * \param lhs Left-hand side expression.
 * \param rhs Right-hand side expression.
 */
Add::Add(Expr* lhs, Expr* rhs) {
    this->lhs = lhs;
    this->rhs = rhs;
}
/**
 * \brief Checks equality of this expression with another expression.
 * \param e The expression to compare with.
 * \return True if equal, false otherwise.
 */
bool Add::equals(Expr* e) {
    Add* addPtr = dynamic_cast<Add*>(e);
    if (addPtr == nullptr) {
        return false;
    }
    return this->lhs->equals(addPtr->lhs) && this->rhs->equals(addPtr->rhs);
}

/**
 * \brief Interprets the addition of expressions.
 * \return The result of the addition.
 */
int Add:: interp(){
    return this->lhs->interp() + this->rhs->interp();
}

/**
 * \brief Checks if the expression contains a variable.
 * \return True if a variable is present, false otherwise.
 */
bool Add::has_variable(){
    return this->lhs->has_variable()||this->rhs->has_variable();
}

/**
 * \brief Substitutes a variable in the expression with another expression.
 * \param str The variable to substitute.
 * \param e The expression to substitute with.
 * \return The new expression after substitution.
 */
Expr* Add::subst(string str, Expr* e){
    return new Add (this->lhs->subst(str, e), this->rhs->subst(str, e));
}

/**
 * \brief Prints the addition expression.
 * \param ostream The output stream.
 */
void Add::print(ostream &ostream){
    ostream << "(";
    lhs->print(ostream);
    ostream << "+";
    rhs->print(ostream);
    ostream << ")";
}

/**
 * \brief Pretty prints the addition expression with proper precedence.
 * \param ostream The output stream.
 * \param prec The precedence level.
 */
void Add::pretty_print_at(ostream &ostream, precedence_t prec) {
    if(prec >= prec_add){
        ostream << "(";
    }
    lhs->pretty_print_at(ostream, prec_add);
    
    ostream << " + ";
    
    rhs->pretty_print_at(ostream, prec_none);
    
    if(prec >= prec_add){
        ostream << ")";
    }
}

//======================  MULT  ======================//

/**
 * \brief Constructs a multiplication expression.
 * \param lhs Left-hand side expression.
 * \param rhs Right-hand side expression.
 */
Mult::Mult(Expr *lhs, Expr *rhs){
  this->lhs = lhs;
  this->rhs = rhs;
}

/**
 * \brief Checks if this expression is equal to another expression.
 * \param e The expression to compare with.
 * \return True if the expressions are equal, false otherwise.
 */
bool Mult:: equals (Expr *e) {
Mult* multPtr = dynamic_cast<Mult*>(e);
if(multPtr==nullptr){
  return false;
}
  return this->lhs->equals(multPtr->lhs) && this->rhs->equals(multPtr->rhs);
}

/**
 * \brief Evaluates the multiplication of the two expressions.
 * \return The integer result of the multiplication.
 */
int Mult:: interp() {
    return this->lhs->interp() * this->rhs->interp();
}

/**
 * \brief Determines if the expression contains a variable.
 * \return True if a variable is present, false otherwise.
 */
bool Mult::has_variable() {
    return this->lhs->has_variable()||this->rhs->has_variable();
}

/**
 * \brief Substitutes a variable within the expression.
 * \param str The variable name to replace.
 * \param e The expression to replace it with.
 * \return A new expression with the substitution made.
 */
Expr* Mult::subst(string str, Expr* e){
    return new Mult (this->lhs->subst(str, e), this->rhs->subst(str, e));
}

/**
 * \brief Prints the expression to the provided output stream.
 * \param ostream The output stream.
 */
void Mult::print (ostream &ostream){
    ostream << "(";
    this->lhs->print(ostream);
    ostream << "*";
    this->rhs->print(ostream);
    ostream << ")";
}

/**
 * \brief Pretty prints the expression with precedence handling.
 * \param ostream The output stream.
 * \param prec The current precedence level.
 */
void Mult::pretty_print_at(ostream &ostream, precedence_t prec) {
    if (prec >= prec_mult) {
        ostream << "(";
    }
    
    this->lhs->pretty_print_at(ostream, prec_mult);
    
    ostream << " * ";
    
    this->rhs->pretty_print_at(ostream, prec_add);
    
    if (prec >= prec_mult) {
        ostream << ")";
    }
}

//======================  NUM  ======================//

/**
 * \brief Initializes a numeric constant expression.
 * \param val The numeric value of the expression.
 */
Num::Num (int val){
  this->val = val;
}
/**
 * \brief Checks if this numeric constant is equal to another expression.
 * \param e A pointer to the expression to compare with this numeric constant.
 * \return True if the expressions are equal (i.e., if `e` is also a `Num` with the same value), false otherwise.
 */
bool Num::equals (Expr *e) {
  Num* numPtr = dynamic_cast<Num*>(e); // Check if 'e' is a 'Num' object
  if (numPtr == nullptr) {
      return false; // 'e' is not a 'Num' object
  }
  return this->val == numPtr->val;
}

/**
 * \brief Evaluates to its numeric value.
 * \return The value of the numeric constant.
 */
int Num:: interp(){
    return this->val;
}

/**
 * \brief Checks if the numeric constant contains a variable.
 * \return False, as numeric constants do not contain variables.
 */
bool Num::has_variable() {
    return false;
}

/**
 * \brief Substitutes a variable within this numeric expression. Since `Num` does not contain variables, it returns itself.
 * \param str The variable name to look for substitution.
 * \param e The expression to substitute in place of the variable.
 * \return A pointer to this numeric constant, as no substitution occurs.
 */
Expr* Num::subst(string str, Expr* e){
    return this;
}

/**
 * \brief Prints the numeric value to the specified output stream.
 * \param ostream The output stream where the numeric value will be printed.
 */
void Num::print (ostream &ostream){
    ostream<<::to_string(val);
}

//======================  VAR  ======================//

/**
 * \brief Constructs a variable expression.
 * \param val The name of the variable.
 */
Var::Var (string val){
  this->val = val;
}

/**
 * \brief Checks if this variable expression is equal to another expression.
 * \param e A pointer to the expression to compare with this variable expression.
 * \return True if `e` is a `Var` object with the same variable name, false otherwise.
 */
bool Var::equals (Expr *e) {
  Var* varPtr = dynamic_cast<Var*>(e); // Check if 'e' is a 'VarExpr' object
  if (varPtr == nullptr) {
      return false; // 'e' is not a 'VarExpr' object
  }
  return this->val == varPtr->val;
}

/**
 * \brief Throws an exception since variables cannot be directly interpreted.
 * \throws std::runtime_error when attempted to interpret a variable.
 */
int Var::interp(){
    throw std::runtime_error("Var cannot be converted to a number");
    
    return 1;
}

/**
 * \brief Checks if the expression contains a variable.
 * \return True, as this object represents a variable.
 */
bool Var::has_variable() {
    return true;
}

/**
 * \brief Substitutes the variable with another expression if it matches the variable name.
 * \param str The name of the variable to substitute.
 * \param e The expression to substitute in place of the variable.
 * \return The original variable or the substitution.
 */
Expr* Var::subst(string str, Expr* e){
    if(val==str){
        return e;
    }
    else {
        return this;
    }
}

/**
 * \brief Prints the variable's name to the provided output stream.
 * \param ostream The output stream.
 */
void Var::print (ostream &ostream){
    ostream << val;
}

//======================  LET  ======================//
/**
 * \brief Constructor for a Let expression.
 * \param lhs The variable on the left-hand side to bind the value to.
 * \param rhs The expression on the right-hand side whose value will be bound to lhs.
 * \param body The body of the Let expression where lhs may be used.
 */
Let::Let(Var* lhs, Expr* rhs, Expr* body){
    this->lhs = lhs;
    this->rhs = rhs;
    this->body = body;
}

/**
 * \brief Checks if this Let expression is equal to another expression.
 * \param e The expression to compare with.
 * \return True if both expressions are Let expressions with equal lhs, rhs, and body; otherwise false.
 */
bool Let::equals(Expr* e){
 Let* _letPtr = dynamic_cast<Let*>(e);
    
 if(_letPtr==nullptr){
        return false;
 }
 return this->lhs->equals(_letPtr->lhs) && this->rhs->equals(_letPtr->rhs) && this->body->equals(_letPtr->body);
}

/**
 * \brief Interprets the Let expression by evaluating rhs, substituting it into body, and then evaluating the result.
 * \return The integer result of interpreting the Let expression.
 */
int Let::interp(){
    
     int rhsValue = rhs->interp();

     Num* tempNum = new Num(rhsValue);

     Expr* substitutedBody = body->subst(lhs->val, tempNum);

     return substitutedBody->interp();
}

/**
 * \brief Determines if the Let expression contains a variable.
 * \return True if either rhs or body contains a variable; otherwise false.
 */
bool Let::has_variable(){
    return this->rhs->has_variable()||this->body->has_variable();
}

/**
 * \brief Performs substitution within the Let expression.
 * \param str The variable name to substitute.
 * \param e The expression to replace str with.
 * \return A new Let expression with the substitution made.
 */
Expr* Let::subst(string str, Expr* e){
    // Check if the variable to substitute is the same as the Let's lhs
       if (lhs->val == str) {
           // If yes, do not substitute within the body, as the Let's variable shadows it
           return new Let(lhs, rhs->subst(str, e), body);
       } else {
           // If not, substitute within both rhs and body
           return new Let(lhs, rhs->subst(str, e), body->subst(str, e));
       }
}


/**
 * \brief Prints the Let expression to the provided output stream in a specific format.
 * \param ostream The output stream to print to.
 */
void Let::print(ostream &ostream){
    ostream << "(_let " << lhs->val << "=" << rhs->to_string() << " _in " << body->to_string() << ")";
}

void Let::pretty_print_at(ostream &ostream, precedence_t prec){
    
}
