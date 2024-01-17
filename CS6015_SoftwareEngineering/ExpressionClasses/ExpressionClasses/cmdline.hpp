//
//  cmdline.hpp
//  ExpressionClasses
//
//  Created by Corinne Jones on 1/16/24.
//

#ifndef cmdline_hpp
#define cmdline_hpp

#include <stdio.h>
#include <iostream>
#include <string>
#include "Tests.hpp"
//#include <cstdlib>

bool doesContain(const char* argv[], int size, std::string s);
void use_arguments(int argc, char **argv); 

#endif /* cmdline_hpp */
