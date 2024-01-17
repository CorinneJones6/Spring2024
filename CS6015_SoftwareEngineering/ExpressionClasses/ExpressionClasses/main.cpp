#include <iostream>
#include "Add.hpp"
#include "cmdline.hpp"
#include "Expr.hpp"
#include "Mult.hpp"
#include "Num.hpp"
#include "Tests.hpp"
#include <string>
#include <cstdlib>

int main(int argc, char **argv) {
  use_arguments(argc, argv);
  std::cout << "All requirements completed, now exiting." << std::endl;

  return 0;
}
