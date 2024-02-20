/**
 * \file cmdline.hpp
 * \brief Command line arguments handler for the ExpressionClasses project.
 *
 * Provides a utility function to process and utilize command line arguments passed to an application.
 * This includes parsing, validation, and setting up initial parameters or configurations
 * based on the arguments provided during the execution of the program.
 *
 * \author Corinne Jones
 * \date 1/16/24
 */

#pragma once

#include <stdio.h>
#include <iostream>
#include <string>
#include "Expr.hpp"
#include "parse.hpp"

using namespace std;

typedef enum {

  do_nothing,
  do_interp,
  do_print,
  do_pretty_print, 

} run_mode_t;

run_mode_t use_arguments(int argc, char **argv);


