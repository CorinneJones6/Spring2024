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

PTR(Expr) NumVal::to_expr(){
    return NEW( NumExpr)(this->val);
}

bool NumVal::equals (PTR(Val) v){
    PTR(NumVal) numPtr = CAST(NumVal)(v);
    
    if (numPtr == nullptr){
        return false;
    }
    return this->val == numPtr->val;
}

PTR(Val) NumVal::add_to(PTR(Val) other_val){
    PTR(NumVal) other_num = CAST(NumVal)(other_val);
    if(other_num == NULL) throw runtime_error("add of a non-number");
    return NEW( NumVal)((unsigned)this->val + (unsigned)other_num->val);
}

PTR(Val) NumVal::mult_with(PTR(Val) other_val){
    PTR(NumVal) other_num = CAST(NumVal)(other_val);
    if(other_num == NULL) throw runtime_error("mult of a non-number");
    return NEW( NumVal)((unsigned)this->val * (unsigned)other_num->val);
}

void NumVal::print (ostream &ostream){
    ostream<<::to_string(val);
}

bool NumVal::is_true() {
    throw runtime_error("number cannot be boolean");
}

PTR(Val) NumVal::call(PTR(Val) actual_arg){
    throw runtime_error("NumVal does not call()");
}

//======================  BoolVal  ======================//

BoolVal::BoolVal(bool b){
    val = b;
}

PTR(Expr) BoolVal::to_expr(){
    return NEW( BoolExpr)(this->val);
}

bool BoolVal::equals (PTR(Val) v){
    PTR(BoolVal) boolPtr = CAST(BoolVal)(v);
    
    if (boolPtr == nullptr){
        return false;
    }
    return this->val == boolPtr->val;
}

PTR(Val) BoolVal::add_to(PTR(Val) other_val){
    throw runtime_error("Bool cannot be added");
}

PTR(Val) BoolVal::mult_with(PTR(Val) other_val){
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

PTR(Val) BoolVal::call(PTR(Val) actual_arg){
    throw runtime_error("BoolVal does not call()");
}

//======================  FunVal  ======================//

FunVal::FunVal(string formal_arg, PTR(Expr) body, PTR(Env) env){
    if(env == nullptr) {
           env = Env::empty;
       }
    this->formal_arg = formal_arg;
        this->body = body;
        this->env = env;
}

PTR(Expr) FunVal::to_expr(){
    return NEW( FunExpr)(this->formal_arg, this->body);
}

bool FunVal::equals (PTR(Val) v){
    PTR(FunVal) funPtr = CAST(FunVal)(v);
    
    if (funPtr == nullptr){
        return false;
    }
    return this->formal_arg == funPtr->formal_arg && this->body->equals(funPtr->body);
}

PTR(Val) FunVal::add_to(PTR(Val) other_val){
    throw runtime_error("Function cannot be added");
}

PTR(Val) FunVal::mult_with(PTR(Val) other_val){
    throw runtime_error("Function cannot be multiplied");
}

void FunVal::print(ostream &ostream){
    ostream << "_fun (" << this->formal_arg << ") " << this->body->to_string();
}

bool FunVal::is_true(){
    throw runtime_error("function cannot be boolean");
}

PTR(Val) FunVal::call(PTR(Val) actual_arg){

    return this->body->interp(NEW(ExtendedEnv)(this->formal_arg, actual_arg, this->env));
    
}
