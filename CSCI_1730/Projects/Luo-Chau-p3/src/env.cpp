#include <iostream>
#include <cstdlib>

using namespace std;

int main(int argc, char **argv, char **envp){
    for (char **env = envp; *env != 0; env++){
        cout << *env << endl; 
    }

    return EXIT_SUCCESS;
}
