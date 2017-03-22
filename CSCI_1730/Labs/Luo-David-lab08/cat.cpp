/*
*   /\___/\
*  ( o   o )
*  (  =^=  )
*  (        )
*  (         )
*  (          )))))))))))
*/
#include <iostream>
#include <cstdio>
#include <fcntl.h>
#include <unistd.h>
#include <cstring>

using namespace std;

int main(int argc, char *argv[]) {
    if (argc < 2) {
        cout << "Usage: " << argv[0] << " <file>" << endl;
        return EXIT_FAILURE;
    }
    int file_desc;
    char * filename;
    for (int i = 1; i < argc; i++) {
        filename = argv[i];

        if (strcmp(filename,"-") == 0) {
            file_desc = STDIN_FILENO;
        } else {
            file_desc = open(filename, O_RDONLY);
            if (file_desc == -1) {
                perror(argv[0]);
                return EXIT_FAILURE;
            }
        }


        const int BUFFER_SIZE = 1024;
        char buffer[BUFFER_SIZE] = {};
        int r = 0;
        while ((r = read(file_desc, buffer, BUFFER_SIZE)) > 0) {
            if(write(STDOUT_FILENO, buffer, r) == -1){
                perror(argv[i]);
            }
        }
        if (file_desc != 0)
            close(file_desc);

        if (r == -1) {
            perror(argv[0]);
            return EXIT_FAILURE;
        }

    }

    return EXIT_SUCCESS;
}
