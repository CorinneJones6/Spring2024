/**
 * \file main.cpp
 * \brief Command line argument handler for test execution.
 *
 * This file contains the entry point for the application, handling command line arguments
 * to provide help information or to execute tests using the Catch testing framework. It supports
 * '--help' for displaying usage information and '--test' for running tests.
 *
 * Usage:
 *  ./application --help  Displays help information.
 *  ./application --test  Executes all compiled tests.
 *
 * \author Corinne Jones
 * \date 1/16/24
 */
#define CATCH_CONFIG_RUNNER
#include "catch.h"
#include "cmdline.hpp"

run_mode_t use_arguments(int argc, char **argv){
  string helpTg = "--help";
  string testTg = "--test";
  string interpTg = "--interp";
  string printTg = "--print";
  string prettyPrintTg = "--pretty-print";
  string tags[5]={helpTg, testTg, interpTg, printTg, prettyPrintTg};
    
  int length = argc;

  for (int i=1; i<length; i++){
    string s = argv[i];
    
    if(s==helpTg){
        cout << "You can use the following tags: " << endl;
        for(string s : tags){
            cout << s <<endl;
        }
        exit(0);
    }
    else if(s==testTg){
          if (Catch::Session().run(1, argv)!= 0){
              cerr << "Tests did not pass" << endl;
              exit(1);
          }
          else{
              cout << "Tests passed!" << endl;
              exit(0);
          }
    }
    else if(s==interpTg){
        return do_interp;
    }
    else if(s==printTg){
        return do_print;
    }
    else if(s==prettyPrintTg){
        return do_pretty_print;
    }
    else {
      cout << "Invalid argument provided" << endl;
      return do_nothing;
    }
  }
    return do_nothing;
};

