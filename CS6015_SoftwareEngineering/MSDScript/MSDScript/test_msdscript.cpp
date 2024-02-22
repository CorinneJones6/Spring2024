#include <iostream>
#include <time.h>
using namespace std;

static string random_expr_string();
static string random_variable();
void testSingleImpl(string path);
void testDualImpl(string path1, path2);

int main(int argc, char **argv) {
    
    if(argc==2){
        string path = argv[1];
        testSingleImpl(path);
    }
    else if(argc==3){
        string path1 = argv[1];
        string path2 = argv[2];
        testDualImpl(path1, path2);
    }
    else{
        
    }
    
    
    
    srand (clock());
    for (int i = 0; i < 10; i++) {
        string s = "";
        s=(rand() % 26) + 'a';
        string in = random_expr_string();
        cout << "Add/Num expr: "<< in << "\n";
        cout << "Variable: "<< s << endl;
    }

return 0;
}



string random_expr_string() {
    if ((rand() % 10) < 6){
        return to_string(rand());
    }
    else{
        return random_expr_string() + "+" + random_expr_string();
    }
}
