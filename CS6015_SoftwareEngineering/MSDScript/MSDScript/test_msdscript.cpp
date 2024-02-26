#include <iostream>
#include <time.h>
#include <random>
#include "exec.hpp"
using namespace std;

/**
 * Generates a random expression as a string.
 *
 * \param depth The current depth of the expression tree. Used to limit recursion and ensure termination.
 * \return A string representing a randomly generated expression.
 */
string random_expr_string(int depth=0) {

    // Limit recursion depth to prevent stack overflow
    if (depth > 10) {
      if (rand() % 2) {
        // Return a random character from a to z
        return string(1, static_cast<char>('a' + (rand() % 26)));
      }
      else {
        // Return a random number as a string
        return to_string(rand() % 100);
      }
    }
    
    int choice = rand() % 5;
    switch(choice) {
      case 0:
        // Random number case
        return to_string(rand() % 100);
      case 1:
        // Addition case
        return random_expr_string(depth + 1) + " + " + random_expr_string(depth + 1);
      case 2:
        // Multiplication case
        return random_expr_string(depth + 1) + " * " + random_expr_string(depth + 1);
      case 3:
        // Random character case
        return string(1, static_cast<char>('a' + (rand() % 26)));
      case 4: {
        // Let expression case
        string var = string(1, static_cast<char>('a' + (rand() % 26)));
        string val = random_expr_string(depth + 1);
        string body = random_expr_string(depth + 1);
        return "_let " + var + "=" + val + " _in " + body;
      }
      default:
        return to_string(rand() % 100);
    }
}

/**
 * Tests the implementation of a given msdscript executable using a single path.
 *
 * \param path The path to the msdscript executable.
 */
void testSingleImpl(string path){
    
    // Define command line arguments for different modes of operation
    const char *interp[] = { path.c_str(), "--interp" };
    const char *print[] = { path.c_str(), "--print" };
    const char *pprint[] = { path.c_str(), "--pretty_print" };
    
    for (int i = 0; i < 100; i++) {
        string str = random_expr_string(0);
        cout << "Trying: " << str << "\n";
        
        // Execute the msdscript program in different modes and compare outputs
        ExecResult interpResult = exec_program(2, interp, str);
        ExecResult printResult = exec_program(2, print, str);
        ExecResult pprintResult = exec_program(2, pprint, str);
        
        // Check if interpreting the printed output matches the original interpretation
        ExecResult interpAgain = exec_program(2, interp, printResult.out);
        
        if(interpAgain.out != interpResult.out){
            cout << "Different results";
        }
    }
    exit(0);
}

/**
 * Tests the implementation of msdscript executable using two different paths.
 *
 * \param path1 The path to the first msdscript executable.
 * \param path2 The path to the second msdscript executable.
 */
void testDualImpl(string path1, string path2){
    
    // Define command line arguments for both executables in different modes
    const char *interp1[] = { path1.c_str(), "--interp" };
    const char *print1[] = { path1.c_str(), "--print" };
    const char *pprint1[] = { path1.c_str(), "--pretty-print" };
           
    const char *interp2[] = { path2.c_str(), "--interp" };
    const char *print2[] = { path2.c_str(), "--print" };
    const char *pprint2[] = { path2.c_str(), "--pretty-print" };
           
           for (int i = 0; i < 100; i++) {
               string str = random_expr_string(0);
               cout << "Trying: " << str << "\n";
               
               // Test and compare outputs for interp, print, and pretty print between two implementations
               ExecResult interp1Result = exec_program(2, interp1, str);
               ExecResult interp2Result = exec_program(2, interp2, str);
               if (interp1Result.out != interp2Result.out){
                   cout << "msdscript interp: " << interp1Result.out;
                   cout << "tester interp: " << interp2Result.out << "\n";
                   throw runtime_error("INTERP - different results\n");
               }
               
               ExecResult print1Result = exec_program(2, print1, str);
               ExecResult print2Result = exec_program(2, print2, str);
               if (print1Result.out != print2Result.out){
                   cout << "msdscript print: " << print1Result.out;
                   cout << "tester print: " << print2Result.out;
                   throw runtime_error("PRINT - different results\n");
               }
               
               ExecResult pprint1Result = exec_program(2, pprint1, str);
               ExecResult pprint2Result = exec_program(2, pprint2, str);
               if (pprint1Result.out != pprint2Result.out){
                   cout << "msdscript pretty print: " << pprint1Result.out;
                   cout << "tester pretty print: " << pprint2Result.out;
                   throw runtime_error("PPRINT - different results\n");
               }
           }
           exit(0);
}

/**
 * Main function. Parses command line arguments and runs tests accordingly.
 *
 * \param argc Number of command line arguments.
 * \param argv Array of command line argument strings.
 * \return Returns 0 on successful completion.
 */
int main(int argc, char **argv) {
    
    // Initialize random seed
    srand (clock());
    
    // Parse command line arguments and run appropriate test implementation
    if(argc==2){
        const char * path = argv[1];
        testSingleImpl(path);
    }
    else if(argc==3){
        string path1 = argv[1];
        string path2 = argv[2];
        testDualImpl(path1, path2);
    }
    else {
        cout << "Please input one of the following: \n";
        cout << "test_msdscript <msdscript_path>\n";
        cout << "test_msdscript <msdscript_path_1> <msdscript_path_2>\n";
        exit(1);
    }

    return 0;
}

