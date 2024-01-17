//
//  cmdline.cpp
//  ExpressionClasses
//
//  Created by Corinne Jones on 1/16/24.
//

#include "cmdline.hpp"


void use_arguments(int argc, char **argv){
  std::string helpTag = "--help";
  std::string testTag = "--test";
  int length = argc;
  bool canTest = true;

  for (int i=1; i<length; i++){
    std::string s = argv[i];
    
    if(s==helpTag){
        std::cout << "You can use the following tags: " << std::endl;
        std::cout << "--help" << std::endl;
        std::cout << "--test" << std::endl;
        exit(0);
    }
    else if(s==testTag){
      if(canTest){
          Tests *t = new Tests();
          t->tests();
          std::cout << "Test passed! " << std::endl;
          canTest = false;
      }
      else if(!canTest){
        std::cerr << "Already tested" << std::endl;
        exit(1);
      }
    }
    else {
      std::cout << "Invalid argument provided" << std::endl;
    }
  }
};

