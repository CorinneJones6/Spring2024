/**
 *\mainpage MSDScript
 *\author Corinne Jones
 *\date 01-16-2024
 */

#include <iostream>
#include "cmdline.hpp"
#include "Expr.hpp"
#include "Val.hpp"
#include "parse.hpp"
#include <string>
#include <cstdlib>

int main(int argc, char **argv) {
    try{
        run_mode_t type = use_arguments(argc, argv);
        
        switch(type) {
            case do_nothing:
                break;
            case do_interp: {
                Expr *e = parse(cin);
                Val* i = e->interp();
                cout << i << "\n";
                break;
            }
            case do_print: {
                Expr *e = parse(cin);
                string str = e->to_string();
                cout << str << "\n";
                break;
            }
            case do_pretty_print: {
                Expr *e = parse(cin);
                string str = e->to_pretty_string();
                cout << str << "\n";
                break;
            }
        }
        
        return 0;
    }
    catch (runtime_error exn) {
        cerr << exn.what() << "\n";
        return 1;
      }
}
