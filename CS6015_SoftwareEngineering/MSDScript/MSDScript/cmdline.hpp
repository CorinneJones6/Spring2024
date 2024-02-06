/**
 * \file cmdline.hpp
 * \brief Command line arguments handler for the ExpressionClasses project.
 *
 * Provides a utility function to process and utilize command line arguments passed to an application.
 * This includes parsing, validation, and possibly setting up initial parameters or configurations
 * based on the arguments provided during the execution of the program.
 *
 * \author Corinne Jones
 * \date 1/16/24
 */

#pragma once

#include <stdio.h>
#include <iostream>
#include <string>

using namespace std;

void use_arguments(int argc, char **argv); 
