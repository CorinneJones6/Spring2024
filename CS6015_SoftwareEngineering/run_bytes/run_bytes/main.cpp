#include <iostream>
#include <time.h>
using namespace std;


static string random_bytes();
static string rand_var();
static string random_expr_string();

int main(int argc, char **argv) {
    srand (clock()); //seed the random
    for (int i = 0; i < 10; i++) {
        string rand_str = random_bytes();
        cout << "Output: "<< rand_str << "\n";
    }
    
//    const char * const wc_argv[] = {"/usr/bin/wc", "-w"};
//    ExecResult wc_result = exec_program();
//    if(wc_result!=0)
    
    
return 0;
}

string random_bytes() {
    string word = "";
    for (int i = rand() % 32; i-- > 0; )
        word += rand() % 256;
    return word;
}

string rand_var(){
    string s;
    for(int i=rand()%6; i++;) {
        s = "";
        s=(rand()%26) + 'a';
    }
    return s;
}

string random_expr_string(){
    if((rand() % 10) < 6){
        return to_string(rand());
    }
    else{
        return random_expr_string() + "+" + random_expr_string();
    }
}
