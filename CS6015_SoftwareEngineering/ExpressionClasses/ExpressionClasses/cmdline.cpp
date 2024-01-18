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

