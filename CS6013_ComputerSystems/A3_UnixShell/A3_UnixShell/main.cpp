#include <iostream>
#include <vector>
#include <string>
#include <unistd.h> // for chdir(), fork(), execvp()
#include <sys/wait.h> // for waitpid()
#include <readline/readline.h>
#include <readline/history.h>
#include "shelpers.hpp" // Assuming this header defines tokenize() and getCommands()

using namespace std;

int main() {
    int rc;
    char* rawInput; //C string for readline

    while (true) {
        rawInput = readline("shell$ ");
        
        string input(rawInput); // Convert to a C++ string for easier handling

        if (strlen(rawInput) > 0) {
            add_history(rawInput); //Allows you to use the arrows to go up and down
        }

        if (input == "exit") {
            free(rawInput); // Free input_ before exiting
            break;
        }

        vector<string> tokens = tokenize(input);
        
        if (tokens[0] == "cd") {
            const char* path;

            if (tokens.size() == 1) {
                path = getenv("HOME"); //Gets home directory from local variable
                
                if(path == nullptr){
                    perror("Path did not work");
                    continue;
                }
            }
            else {
                path = tokens[1].c_str(); // Go to specified directory
            }

            rc = chdir(path);
            
            if(rc!=0){
                perror("chdir() failed");
                continue;
            }
            free(rawInput); // Free rawInput and continue to the next iteration
            continue;
        }

        vector<Command> cmds = getCommands(tokens);

        for (Command& cmd : cmds) {
            int pid = fork();

            if (pid < 0) {
                perror("fork() failed");
                exit(1);
            }
            else if (pid > 0) {
                // Parent process
                if (cmd.inputFd != STDIN_FILENO) {
                    close(cmd.inputFd);
                }

                if (cmd.outputFd != STDOUT_FILENO) {
                    close(cmd.outputFd);
                }

                int status;
                waitpid(pid, &status, 0); // Wait for the child process to finish
            }
            else {
                // Child process
                if (cmd.inputFd != STDIN_FILENO) {
                    dup2(cmd.inputFd, STDIN_FILENO);
                }

                if (cmd.outputFd != STDOUT_FILENO) {
                    dup2(cmd.outputFd, STDOUT_FILENO);
                }

                execvp(cmd.execName.c_str(), const_cast<char* const*>(cmd.argv.data()));

                // If execvp returns, it must have failed
                perror("execvp() failed");
                exit(1);
            }
        }

        free(rawInput); // Ensure rawInput is freed at the end of each loop iteration
    }

    return 0;
}
