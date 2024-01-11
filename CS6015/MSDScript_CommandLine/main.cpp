#include "cmdline.hpp"
#include <iostream>
#include <string>
#include <cstdlib>

int main(int argc, char **argv) {
  use_arguments(argc, argv);
  std::cout << "All requirements completed, now exiting." << std::endl;
  return 0;
}