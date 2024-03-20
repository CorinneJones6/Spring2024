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
#include "Val.hpp"

//====================== Expr ======================//

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
void Expr::pretty_print_at(ostream &ostream, precedence_t prec, bool let_parent, streampos &strmpos){
    print(ostream);
}

/**
 * \brief Pretty prints the expression.
 * \param ostream The output stream.
 */
void Expr::pretty_print(ostream &ostream){
    streampos strmpos = 0;
    pretty_print_at(ostream, prec_none, false, strmpos);
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

//======================  AddExpr  ======================//

/**
 * \brief Constructs an addition expression.
 * \param lhs Left-hand side expression.
 * \param rhs Right-hand side expression.
 */
AddExpr::AddExpr(Expr* lhs, Expr* rhs) {
    this->lhs = lhs;
    this->rhs = rhs;
}
/**
 * \brief Checks equality of this expression with another expression.
 * \param e The expression to compare with.
 * \return True if equal, false otherwise.
 */
bool AddExpr::equals(Expr* e) {
    AddExpr* addPtr = dynamic_cast<AddExpr*>(e);
    if (addPtr == nullptr) {
        return false;
    }
    return this->lhs->equals(addPtr->lhs) && this->rhs->equals(addPtr->rhs);
}

/**
 * \brief Interprets the addition of expressions.
 * \return The result of the addition.
 */
Val* AddExpr:: interp(){
    return this->lhs->interp()->add_to(this->rhs->interp());
}

/**
 * \brief Checks if the expression contains a variable.
 * \return True if a variable is present, false otherwise.
 */
//bool AddExpr::has_variable(){
//    return this->lhs->has_variable()||this->rhs->has_variable();
//}

/**
 * \brief Substitutes a variable in the expression with another expression.
 * \param str The variable to substitute.
 * \param e The expression to substitute with.
 * \return The new expression after substitution.
 */
Expr* AddExpr::subst(string str, Expr* e){
    return new AddExpr (this->lhs->subst(str, e), this->rhs->subst(str, e));
}

/**
 * \brief Prints the addition expression.
 * \param ostream The output stream.
 */
void AddExpr::print(ostream &ostream){
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
void AddExpr::pretty_print_at(ostream &ostream, precedence_t prec, bool let_parent, streampos &strmpos) {
    if(prec >= prec_add){
        ostream << "(";
    }
    lhs->pretty_print_at(ostream, prec_add, true, strmpos);
    
    ostream << " + ";
    
    rhs->pretty_print_at(ostream, prec_none, let_parent, strmpos);
    
    if(prec >= prec_add){
        ostream << ")";
    }
}

//======================  MultExpr  ======================//

/**
 * \brief Constructs a multiplication expression.
 * \param lhs Left-hand side expression.
 * \param rhs Right-hand side expression.
 */
MultExpr::MultExpr(Expr *lhs, Expr *rhs){
  this->lhs = lhs;
  this->rhs = rhs;
}

/**
 * \brief Checks if this expression is equal to another expression.
 * \param e The expression to compare with.
 * \return True if the expressions are equal, false otherwise.
 */
bool MultExpr:: equals (Expr *e) {
MultExpr* multPtr = dynamic_cast<MultExpr*>(e);
if(multPtr==nullptr){
  return false;
}
  return this->lhs->equals(multPtr->lhs) && this->rhs->equals(multPtr->rhs);
}

/**
 * \brief Evaluates the multiplication of the two expressions.
 * \return The integer result of the multiplication.
 */
Val* MultExpr:: interp() {
    return this->lhs->interp()->mult_with(this->rhs->interp());
}

/**
 * \brief Determines if the expression contains a variable.
 * \return True if a variable is present, false otherwise.
 */
//bool MultExpr::has_variable() {
//    return this->lhs->has_variable()||this->rhs->has_variable();
//}

/**
 * \brief Substitutes a variable within the expression.
 * \param str The variable name to replace.
 * \param e The expression to replace it with.
 * \return A new expression with the substitution made.
 */
Expr* MultExpr::subst(string str, Expr* e){
    return new MultExpr (this->lhs->subst(str, e), this->rhs->subst(str, e));
}

/**
 * \brief Prints the expression to the provided output stream.
 * \param ostream The output stream.
 */
void MultExpr::print (ostream &ostream){
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
void MultExpr::pretty_print_at(ostream &ostream, precedence_t prec, bool let_parent, streampos &strmpos) {
    bool parent = let_parent;
    if (prec >= prec_mult) {
        ostream << "(";
        parent = false;
    }
    
    this->lhs->pretty_print_at(ostream, prec_mult, true, strmpos);
    
    ostream << " * ";
    
    this->rhs->pretty_print_at(ostream, prec_add, parent, strmpos);
    
    if (prec >= prec_mult) {
        ostream << ")";
    }
}

//======================  NumExpr  ======================//

/**
 * \brief Initializes a numeric constant expression.
 * \param rep The numeric value of the expression.
 */
NumExpr::NumExpr (int rep){
  this->val = rep;
}
/**
 * \brief Checks if this numeric constant is equal to another expression.
 * \param e A pointer to the expression to compare with this numeric constant.
 * \return True if the expressions are equal (i.e., if `e` is also a `Num` with the same value), false otherwise.
 */
bool NumExpr::equals (Expr *e) {
  NumExpr* numPtr = dynamic_cast<NumExpr*>(e); // Check if 'e' is a 'Num' object
  if (numPtr == nullptr) {
      return false; // 'e' is not a 'Num' object
  }
  return this->val == numPtr->val;
}

/**
 * \brief Evaluates to its numeric value.
 * \return The value of the numeric constant.
 */
Val* NumExpr:: interp(){
    return new NumVal(val);
}

/**
 * \brief Checks if the numeric constant contains a variable.
 * \return False, as numeric constants do not contain variables.
 */
//bool NumExpr::has_variable() {
//    return false;
//}

/**
 * \brief Substitutes a variable within this numeric expression. Since `Num` does not contain variables, it returns itself.
 * \param str The variable name to look for substitution.
 * \param e The expression to substitute in place of the variable.
 * \return A pointer to this numeric constant, as no substitution occurs.
 */
Expr* NumExpr::subst(string str, Expr* e){
    return this;
}

/**
 * \brief Prints the numeric value to the specified output stream.
 * \param ostream The output stream where the numeric value will be printed.
 */
void NumExpr::print (ostream &ostream){
    ostream<<::to_string(val);
}

//======================  VarExpr  ======================//

/**
 * \brief Constructs a variable expression.
 * \param val The name of the variable.
 */
VarExpr::VarExpr (string val){
  this->val = val;
}

/**
 * \brief Checks if this variable expression is equal to another expression.
 * \param e A pointer to the expression to compare with this variable expression.
 * \return True if `e` is a `Var` object with the same variable name, false otherwise.
 */
bool VarExpr::equals (Expr *e) {
  VarExpr* varPtr = dynamic_cast<VarExpr*>(e); // Check if 'e' is a 'VarExpr' object
  if (varPtr == nullptr) {
      return false; // 'e' is not a 'VarExpr' object
  }
  return this->val == varPtr->val;
}

/**
 * \brief Throws an exception since variables cannot be directly interpreted.
 * \throws std::runtime_error when attempted to interpret a variable.
 */
Val* VarExpr::interp(){
    throw runtime_error("Var cannot be converted to a number");
//    return new NumVal(-1);
}

/**
 * \brief Checks if the expression contains a variable.
 * \return True, as this object represents a variable.
 */
//bool VarExpr::has_variable() {
//    return true;
//}

/**
 * \brief Substitutes the variable with another expression if it matches the variable name.
 * \param str The name of the variable to substitute.
 * \param e The expression to substitute in place of the variable.
 * \return The original variable or the substitution.
 */
Expr* VarExpr::subst(string str, Expr* e){
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
void VarExpr::print (ostream &ostream){
    ostream << val;
}

//====================== LetExpr  ======================//

/**
 * \brief Constructor for a Let expression.
 * \param lhs The variable on the left-hand side to bind the value to.
 * \param rhs The expression on the right-hand side whose value will be bound to lhs.
 * \param body The body of the Let expression where lhs may be used.
 */
LetExpr::LetExpr(string lhs, Expr* rhs, Expr* body){
    this->lhs = lhs;
    this->rhs = rhs;
    this->body = body;
}

/**
 * \brief Checks if this Let expression is equal to another expression.
 * \param e The expression to compare with.
 * \return True if both expressions are Let expressions with equal lhs, rhs, and body; otherwise false.
 */
bool LetExpr::equals(Expr* e){
    LetExpr* _letPtr = dynamic_cast<LetExpr*>(e);
    
    if(_letPtr==nullptr){
        return false;
    }
    return this->lhs==(_letPtr->lhs) && this->rhs->equals(_letPtr->rhs) && this->body->equals(_letPtr->body);
}

/**
 * \brief Interprets the Let expression by evaluating rhs, substituting it into body, and then evaluating the result.
 * \return The integer result of interpreting the Let expression.
 */
Val* LetExpr::interp(){
    
    Val* rhsValue = rhs->interp();
    
    return body->subst(lhs, rhsValue->to_expr())->interp();
}

/**
 * \brief Performs substitution within the Let expression.
 * \param str The variable name to substitute.
 * \param e The expression to replace str with.
 * \return A new Let expression with the substitution made.
 */
Expr* LetExpr::subst(string str, Expr* e){
    // Check if the variable to substitute is the same as the Let's lhs
       if (lhs == str) {
           // If yes, do not substitute within the body, as the Let's variable shadows it
           return new LetExpr(lhs, rhs->subst(str, e), body);
       }
       else {
           // If not, substitute within both rhs and body
           return new LetExpr(lhs, rhs->subst(str, e), body->subst(str, e));
       }
}

/**
 * \brief Prints the Let expression to the provided output stream in a specific format.
 * \param ostream The output stream to print to.
 */
void LetExpr::print(ostream &ostream){
    ostream << "(_let " << lhs << "=" << rhs->to_string() << " _in " << body->to_string() << ")";
}

/**
 * \brief Pretty prints the expression with precedence handling.
 * \param ostream The output stream.
 * \param prec The current precedence level.
 */
void LetExpr::pretty_print_at(ostream &ostream, precedence_t prec, bool let_parent, streampos &strmpos) {
   
    
    if (let_parent) {
        ostream << "(";
    }
    
    streampos startPosition = ostream.tellp();
    
    streampos depth = startPosition-strmpos;
    
    ostream << "_let " << lhs << " = ";
    
    rhs->pretty_print_at(ostream, prec_none, false, strmpos);
    
    ostream << "\n";
    
    strmpos = ostream.tellp();
    
    ostream << string(depth, ' ') << "_in  ";
   
    body->pretty_print_at(ostream, prec_none, false, strmpos);

    if (let_parent) {
        ostream << ")";
    }
}

//======================  BoolExpr  ======================//

BoolExpr::BoolExpr(bool b){
    this->val = b;
}

bool BoolExpr::equals (Expr *e){
    BoolExpr* boolPtr = dynamic_cast<BoolExpr*>(e);
    
    if (boolPtr == nullptr) {
        return false;
    }
    return this->val == boolPtr->val;
}

Val* BoolExpr::interp(){
    return new BoolVal(val);
}

Expr* BoolExpr::subst(string str, Expr* e){
    return this;
}

void BoolExpr::print(ostream &ostream){
    if(val){
        ostream << "_true";
    }
    else if (!val){
        ostream << "_false";
    }
}

void BoolExpr::pretty_print_at(ostream &ostream, precedence_t prec, bool let_parent, streampos &strmpos){
    
    if(val){
        ostream << "_true";
    }
    else if (!val){
        ostream << "_false";
    }
}

//======================  IfExpr  ======================//

IfExpr::IfExpr(Expr* if_, Expr* then_, Expr* else_){
    this->if_ = if_;
    this->then_ = then_;
    this->else_ = else_;
}

bool IfExpr::equals (Expr *e){
    IfExpr* ifPtr = dynamic_cast<IfExpr*>(e);
    
    if (ifPtr == nullptr) {
        return false;
    }
    return this->if_->equals(ifPtr->if_) && this->then_->equals(ifPtr->then_) && this->else_->equals(ifPtr->else_);
}

Val* IfExpr::interp(){
    Val* conditionValue = if_->interp();
    BoolVal* boolCondition = dynamic_cast<BoolVal*>(conditionValue);
    if (boolCondition != nullptr && boolCondition->is_true()) {
        return then_->interp();
    } else {
        return else_->interp();
    }
}

Expr* IfExpr::subst(string str, Expr* e){
    return new IfExpr(this->if_->subst(str, e),this->then_->subst(str, e), this->else_->subst(str, e));
}

void IfExpr::print(ostream &ostream){
    ostream << "(" << "_if";
    this->if_->print(ostream);
    ostream << "_then";
    this->then_->print(ostream);
    ostream << "_else";
    this->else_->print(ostream);
    ostream << ")";
}

void IfExpr::pretty_print_at(ostream &ostream, precedence_t prec, bool let_parent, streampos &strmpos) {
    
    if (let_parent) {
            ostream << "(";
    }
    
    ostream << "_if ";
    
    if_->pretty_print_at(ostream, prec_none, false, strmpos);
    
    ostream << "\n";
    
    strmpos = ostream.tellp();
    
    ostream << "_then ";
    
    then_->pretty_print_at(ostream, prec_none, false, strmpos);
    
    ostream << "\n";
    
    ostream << "_else ";
    
    strmpos = ostream.tellp();
    
    else_->pretty_print_at(ostream, prec_none, false, strmpos);
    
    if (let_parent) {
        ostream << ")";
    }
    
    ostream << "\n";
    
}

//======================  EqExpr  ======================//

EqExpr::EqExpr(Expr* lhs, Expr* rhs){
    this->lhs = lhs;
    this->rhs = rhs;
}

bool EqExpr::equals (Expr *e){
    EqExpr* eqPtr = dynamic_cast<EqExpr*>(e);
    
    if (eqPtr == nullptr) {
        return false;
    }
    return this->rhs->equals(eqPtr->rhs) && this->lhs->equals(eqPtr->lhs);
}

Val* EqExpr::interp(){
   return new BoolVal(rhs->interp()->equals(lhs->interp()));
}

Expr* EqExpr::subst(string str, Expr* e){
    return new EqExpr(this->rhs->subst(str, e), this->lhs->subst(str, e));
}

void EqExpr::print(ostream &ostream){
    ostream << "(";
    this->rhs->print(ostream);
    ostream << "==";
    this->lhs->print(ostream);
    ostream << ")";
}

void EqExpr::pretty_print_at(ostream &ostream, precedence_t prec, bool let_parent, streampos &strmpos){
    
    if (let_parent) {
            ostream << "(";
    }
    
    lhs->pretty_print_at(ostream, prec_none, false, strmpos);
    
    ostream << "==";
    
    rhs->pretty_print_at(ostream, prec_none, false, strmpos);
    
    if (let_parent) {
        ostream << ")";
    }
}

//======================  FunExpr  ======================//

FunExpr::FunExpr(string formal_arg, Expr *body){
    this->formal_arg = formal_arg;
    this->body = body;
}

bool FunExpr::equals (Expr *e){
    FunExpr* funPtr = dynamic_cast<FunExpr*>(e);
    
    if (funPtr == nullptr) {
        return false;
    }
    return this->formal_arg == funPtr->formal_arg && this->body->equals(funPtr->body);
}

Val* FunExpr::interp(){
    return new FunVal(formal_arg, body); 
}

Expr* FunExpr::subst(string str, Expr* e){
    if(formal_arg == str){
        // Return the function as is, no substitution needed
        return this;
    }
    else{
        // Proceed with substitution in the body if the formal argument doesn't match
        return new FunExpr(formal_arg, body->subst(str, e));
    }
}

void FunExpr::print(ostream &ostream){
    ostream << "_fun (" << this->formal_arg << ") " << this->body->to_string();
}

void FunExpr::pretty_print_at(ostream &ostream, precedence_t prec, bool let_parent, streampos &strmpos){
    
}

//======================  CallExpr  ======================//

CallExpr::CallExpr(Expr *to_be_called, Expr *actual_arg){
    this->to_be_called = to_be_called;
    this->actual_arg = actual_arg;
}

bool CallExpr::equals (Expr *e){
    CallExpr* callPtr = dynamic_cast<CallExpr*>(e);
    
    if (callPtr == nullptr) {
        return false;
    }
    return this->to_be_called->equals(callPtr->to_be_called) && this->actual_arg->equals(callPtr->actual_arg);
}

Val* CallExpr::interp(){
    return this->to_be_called->interp()->call(this->actual_arg->interp());
}

Expr* CallExpr::subst(string str, Expr* e){
    return new CallExpr(this->to_be_called->subst(str, e), this->actual_arg->subst(str, e));
}

void CallExpr::print(ostream &ostream){
    ostream << "(" << this->to_be_called->to_string() << ") (" << this->actual_arg->to_string() << ")";
}

void CallExpr::pretty_print_at(ostream &ostream, precedence_t prec, bool let_parent, streampos &strmpos){
    
}
