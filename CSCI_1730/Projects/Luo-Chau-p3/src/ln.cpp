#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <string.h>

using namespace std;

int main(int argc, char **argv){
    int sflag = 0;
    char *target, *linkf; 
    
    if (argc == 1 || argc < 3){
        cout << "Usage: ./ln [-s] target-file link-file" << endl;
        return EXIT_FAILURE; 
    } 
    
    int c; 
    while ((c = getopt(argc, argv, "s::")) != -1){
        switch(c){
            case 's':
                sflag = 1; 
                break; 
            case '?':
                if (isprint(optopt))
                    fprintf(stderr, "Unkown option '-%c'.\n", optopt);
                else 
                    fprintf(stderr, "Unknown option character, '\\x%x'.\n", optopt);
                return EXIT_FAILURE;
                break;
        }
    }

    target = argv[optind]; 
    linkf = argv[optind + 1];  
    
    if (!sflag) {
        if (link(linkf, target))
            cout << "ln: " << strerror(errno) << endl;
    } else {
        if (symlink(target, linkf))
            cout << "ln: " << strerror(errno) << endl; 
    }

    return EXIT_SUCCESS; }
