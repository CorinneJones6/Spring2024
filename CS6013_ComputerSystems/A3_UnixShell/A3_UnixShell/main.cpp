//
//  main.cpp
//  A3_UnixShell
//
//  Created by Corinne Jones on 2/8/24.
//

#include <iostream>
#include "shelpers.hpp"
#include <readline/readline.h>
#include <readline/history.h>

using namespace std;

int main(int argc, const char * argv[]) {
    errno = EFAULT;
    
    int rc;
    string input;
    
    while(true){
        
        cout << "shell$ ";
        
        if (!getline(cin, input)){
            break;
        }
        
        if (input == "exit"){
            break;
        }
        
        vector<string> tokens = tokenize(input);
        vector<Command> cmds = getCommands(tokens);
        
        if (tokens[0] == "cd") {
            const char* path;
            
            if (tokens.size() == 1) {
                // 'cd' with no additional parameter should go to HOME
                path = getenv("HOME");
                
                if (path == nullptr) {
                    perror("path did not work");
                    continue;
                }
            }
            else {
                path = tokens[1].c_str();
            }
            rc = chdir(path);

            if(rc!=0){
                perror("chdir() failed");
                continue;
            }
            continue; // Skip the rest of the loop to wait for next command
        }
        
        for(Command& cmd : cmds){
            
            int pid = fork();
            
            if ( pid < 0 ){
                perror( "fork() failed" );
                exit(1);
            }
            
            //parent
            else if( pid > 0 ){
                
                if(cmd.inputFd != STDIN_FILENO){
                    close(cmd.inputFd); 
                }
                
                if(cmd.outputFd != STDOUT_FILENO){
                    close(cmd.outputFd);
                }
                
                int status;
                rc = waitpid(pid, &status, 0);
                
                if (rc == -1) {
                    perror( "waitpid() failed" );
                    exit(1);
                }
            }
            
            //child
            else{
                
                if(cmd.inputFd != STDIN_FILENO){
                    dup2(cmd.inputFd, STDIN_FILENO);
                }
                
                if(cmd.outputFd != STDOUT_FILENO){
                    dup2(cmd.outputFd, STDOUT_FILENO);
                }
                
                rc = execvp(cmd.execName.c_str(), const_cast<char* const*>(cmd.argv.data()));
                
                if (rc == -1) {
                    perror( "execvp() failed" );
                    exit(1);
                }
            }
        }
    }
}
