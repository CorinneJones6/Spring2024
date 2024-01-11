#include "cmdline.hpp"
#include <iostream>
#include <string>
#include <cstdlib>

bool doesContain(char** argv, int size, std::string s){
  for(int i=0; i<size; i++){
    if(argv[i] == s){
      return true;
    }
  }
  return false; 
}

void use_arguments(int argc, char **argv){
  std::string helpTag = "--help"; 
  std::string testTag = "--test";
  int size = argc; 
  bool canTest = true; 

  if(argc > 1){
    if (doesContain(argv, size, helpTag)){
      std::cout << "You can use the following tags: " << std::endl;
      std::cout << "--help" << std::endl;
      std::cout << "--test" << std::endl;
        exit(0); 
    }
    else if(doesContain(argv, size, testTag)){
      if(canTest){
      std::cout << "Test passed! " << std::endl;
      canTest = false;
      }
      if(!canTest){
        std::cerr << "Already tested" << std::endl;
        exit(1);
      }
    }
    else {
      std::cout << "Invalid argument provided" << std::endl;
    }
  }
  else {
    std::cout << "Invalid argument provided" << std::endl;
  }
};