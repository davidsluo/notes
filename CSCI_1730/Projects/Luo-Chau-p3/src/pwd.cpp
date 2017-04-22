#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <string.h>

using namespace std;

int main(int argc, char **argv){
    long maxlen = pathconf(".", _PC_PATH_MAX); 
     
    char *buf = nullptr;      
    cout << getcwd(buf, maxlen) << endl;

    return EXIT_SUCCESS; 
}
