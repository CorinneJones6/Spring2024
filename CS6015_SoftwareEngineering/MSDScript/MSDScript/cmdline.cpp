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

void use_arguments(int argc, char **argv){
  string helpTag = "--help";
  string testTag = "--test";
  int length = argc;
  bool canTest = true;

  for (int i=1; i<length; i++){
    string s = argv[i];
    
    if(s==helpTag){
        cout << "You can use the following tags: " << endl;
        cout << "--help" << endl;
        cout << "--test" << endl;
        exit(0);
    }
    else if(s==testTag){
        if(!canTest){
            cerr << "Already tested" << endl;
            exit(1);
        }
      else if (canTest) {
          if (Catch::Session().run(1, argv)!= 0){
              exit(1);
          }
          canTest = false;
      }
    }
    else {
      cout << "Invalid argument provided" << endl;
    }
  }
};

