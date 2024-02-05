//
//  main.cpp
//  L3_PipesProcess
//
//  Author: Corinne Jones
//  Date: 2/5/24
//

#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <cstdlib>

using namespace std;

void dup_example(){
    
}

void pipe_example(string str, int len){
    // Create the fds for the pipe before the fork so that both parent
    // and child will have access to them.
    
    string msg = str;
    int size = len;
    
    int fds[2] = { -1, -1 };
    int rc = pipe( fds );
    
    int readFd = fds[0];
    int writeFd = fds[1];
    
    if ( rc !=0 ){
        perror( "Pipe failed" );
        exit(1);
    }
    
    int pid = fork();
    
    if ( pid < 0 ){
        perror( "Fork failed" );
        exit(1);
    }
    
    //This runs twice d/t both forks running it
    cout <<"fds: " << fds[0] << ", " << fds[1] << "\n";
    
    //Parent
    if( pid > 0 ){
        close( readFd );
        cout << "PARENT: my child has pid " << pid << "\n";
        
        write(writeFd, &len, sizeof(int)); 
        write(writeFd, &msg, size);
    
        wait( NULL );
        cout << "PARENT: done wait for child, exiting now\n";
    }
    //Child
    else {
        close( writeFd );
        
        cout << "CHILD: my pid is " << getpid() << "\n";

        cout << "CHILD: waiting for data from parent...\n";
        
        int size;
        read ( readFd, &size, sizeof(int) );
        
        char *message = (char*)malloc(size);
        read( readFd, message, size );
        
        cout << "CHILD:          received: " << message <<"\n";
    }
}

int main(int argc, const char * argv[]) {
    
    if (argc == 2){
        const char* msg = argv[1];
        int len = strlen(msg);

        pipe_example(msg, len);
    }
    else{
        const char* msg = "hello";
        int len = 5;
        
        pipe_example(msg, len);
    }
    
    return 0;
}
