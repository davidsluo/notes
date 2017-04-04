#include <cstdio>
#include <iostream>
#include <unistd.h>
#include <cstring>

using namespace std;

int remove(const char * name, bool recursive=false, bool force=false);

int main(int argc, char * argv[]) {
    const char * const SYNTAX_ERROR = "Syntax: %s [-rf] [file...]";
    const char * const INVALID_OPTION = "%s: Invalid option '%s'";
    bool recursive = false;
    bool force = false;

    if (argc < 2) {
        cerr << sprintf(SYNTAX_ERROR, argv[0]) << endl;
        exit(EXIT_FAILURE);
    }

    for (int i = 1; i < argc; i++) {
        if (argv[i][0] == '-') {
            switch(arg) {
                case "-rf":
                    recursive = true;
                    force = true;
                    break;
                case "-r":
                    recursive = true;
                    break; 
                case "-f":
                    force = true;
                    break;
                default:
                    cerr << sprintf(INVALID_OPTION, argv[0], argv[i]) << endl;
                    exit(EXIT_FAILURE);
                    break;
            }
        }
    }

    for (int i = 1; i < argc; i++) {
        if (argv[i][0] != '-') {
            remove(argv[i], recursive, force);
        }
    }

    exit(EXIT_SUCCESS);
}

int remove(const char * name, bool recursive, bool force) {
    // Do not delete if <name> is current dir or parent dir's
    
}
