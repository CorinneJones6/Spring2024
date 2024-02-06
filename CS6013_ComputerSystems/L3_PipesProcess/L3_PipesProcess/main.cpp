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
    int rc;
    
    int fds[2] = { -1, -1 };
    rc = pipe( fds );
    
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
        rc = close( readFd );
        if( rc == -1){
            perror( "Parent close() failed" );
            exit(1);
        }
        cout << "PARENT: my child has pid " << pid << "\n";
        
        int rc = (int)write(writeFd, &len, sizeof(int));
        
        if( rc == -1){
            perror( "Parent write() size failed" );
            exit(1);
        }
        rc = (int)write(writeFd, &msg, size);
        
        if( rc == -1){
            perror( "Parent write() message failed" );
            exit(1);
        }
        
        rc = wait( NULL );
        if( rc == -1){
            perror( "Parent wait() failed" );
            exit(1);
        }
        
        cout << "PARENT: done wait for child, exiting now\n";
    }
    //Child
    else {
        rc = close( writeFd );
        
        if( rc == -1){
            perror( "Child close() failed" );
            exit(1);
        }
        
        cout << "CHILD: my pid is " << getpid() << "\n";

        cout << "CHILD: waiting for data from parent...\n";
        
        int size;
        rc = (int)read ( readFd, &size, sizeof(int) );
        
        if( rc == -1){
            perror( "Child read() in size failed" );
            exit(1);
        }
        
        char *message = (char*)malloc(size);
        rc = (int)read( readFd, message, size );
        
        if( rc == -1){
            perror( "Child read() in message failed" );
            exit(1);
        }
        
        cout << "CHILD:          received: " << message <<"\n";
    }
}

int main(int argc, const char * argv[]) {
    
    errno = EFAULT;
    
    if (argc == 2){
        const char* msg = argv[1];
        int len = (int)strlen(msg);

        pipe_example(msg, len);
    }
    else{
        const char* msg = "hello";
        int len = 5;
        
        pipe_example(msg, len);
    }
    
    return 0;
}
