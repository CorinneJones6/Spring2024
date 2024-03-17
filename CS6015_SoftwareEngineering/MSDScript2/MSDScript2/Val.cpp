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

NumVal::NumVal(int i){
    val = i;
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
    return new NumVal(this->val + other_num->val);
}

Val* NumVal::mult_with(Val* other_val){
    NumVal *other_num = dynamic_cast<NumVal*>(other_val);
    if(other_num == NULL) throw runtime_error("mult of a non-number");
    return new NumVal(this->val * other_num->val);
}

void NumVal::print (ostream &ostream){
    ostream<<::to_string(val);
}

bool NumVal::is_true() {
    throw runtime_error("number cannot be boolean");
    return false;
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
    
//    return new NumVal(-1);
}

Val* BoolVal::mult_with(Val* other_val){
    throw runtime_error("Bool cannot be multiplied");
    
//    return new NumVal(-1);
}

void BoolVal::print(ostream &ostream){
    ostream<<::to_string(val);
}

bool BoolVal::is_true() {
    return val;
}
