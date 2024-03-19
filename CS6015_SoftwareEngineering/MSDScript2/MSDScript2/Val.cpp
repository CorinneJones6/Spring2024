//
//  Val.cpp
//  MSDScript
//
//  Created by Corinne Jones on 3/5/24.
//

#include "Val.hpp"

//======================  NumVal  ======================//

string Val::to_string(){
    stringstream st("");
    this->print(st);
    return st.str();
}

NumVal::NumVal(int val){
    this->val = val;
}

Expr* NumVal::to_expr(){
    return new NumExpr(this->val);
}

bool NumVal::equals (Val *v){
    NumVal* numPtr = dynamic_cast<NumVal*>(v);
    
    if (numPtr == nullptr){
        return false;
    }
    return this->val == numPtr->val;
}

Val* NumVal::add_to(Val* other_val){
    NumVal *other_num = dynamic_cast<NumVal*>(other_val);
    if(other_num == NULL) throw runtime_error("add of a non-number");
    return new NumVal((unsigned)this->val + (unsigned)other_num->val);
}

Val* NumVal::mult_with(Val* other_val){
    NumVal *other_num = dynamic_cast<NumVal*>(other_val);
    if(other_num == NULL) throw runtime_error("mult of a non-number");
    return new NumVal((unsigned)this->val * (unsigned)other_num->val);
}

void NumVal::print (ostream &ostream){
    ostream<<::to_string(val);
}

bool NumVal::is_true() {
    throw runtime_error("number cannot be boolean");
}

Val* NumVal::call(Val* actual_arg){
    throw runtime_error("NumVal does not call()");
}

//======================  BoolVal  ======================//

BoolVal::BoolVal(bool b){
    val = b;
}

Expr* BoolVal::to_expr(){
    return new BoolExpr(this->val);
}

bool BoolVal::equals (Val *v){
    BoolVal* boolPtr = dynamic_cast<BoolVal*>(v);
    
    if (boolPtr == nullptr){
        return false;
    }
    return this->val == boolPtr->val;
}

Val* BoolVal::add_to(Val* other_val){
    throw runtime_error("Bool cannot be added");
}

Val* BoolVal::mult_with(Val* other_val){
    throw runtime_error("Bool cannot be multiplied");
}

void BoolVal::print(ostream &ostream){
    if(val){
        ostream << "_true";
    }
    else if (!val){
        ostream << "_false";
    }
}

bool BoolVal::is_true() {
    return val;
}

Val* BoolVal::call(Val* actual_arg){
    throw runtime_error("BoolVal does not call()");
}

//======================  FunVal  ======================//

FunVal::FunVal(string formal_arg, Expr *body){
    this->formal_arg = formal_arg;
    this->body = body; 
}

Expr* FunVal::to_expr(){
    return new FunExpr(this->formal_arg, this->body);
}

bool FunVal::equals (Val *v){
    FunVal* funPtr = dynamic_cast<FunVal*>(v);
    
    if (funPtr == nullptr){
        return false;
    }
    return this->formal_arg == funPtr->formal_arg && this->body->equals(funPtr->body);
}

Val* FunVal::add_to(Val* other_val){
    throw runtime_error("Function cannot be added");
}

Val* FunVal::mult_with(Val* other_val){
    throw runtime_error("Function cannot be multiplied");
}

void FunVal::print(ostream &ostream){
    ostream << "_fun (" << this->formal_arg << ") " << this->body->to_string();
}

bool FunVal::is_true(){
    throw runtime_error("function cannot be boolean");
}

Val* FunVal::call(Val* actual_arg){
    
    Expr* actualExpr = actual_arg->to_expr();
    
    return body->subst(formal_arg, actualExpr)->interp();
    
}
