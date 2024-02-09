/**
 *\mainpage MSDScript
 *\author Corinne Jones
 *\date 01-16-2024
 */

#include <iostream>
#include "cmdline.hpp"
#include "Expr.hpp"
#include "parse.hpp"
#include <string>
#include <cstdlib>

int main(int argc, char **argv) {

  use_arguments(argc, argv);
    
  cout << "All requirements completed, now exiting." << endl;
    
//    while(1){
//        Expr *e = parse_expr(cin);
//        
//        e->pretty_print(cout);
//        cout << "\n";
//        
//        skip_whitespace(cin);
//        
//        if(cin.eof()){
//            break; 
//        }
//    }

  return 0;
}
