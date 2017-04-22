#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <fcntl.h>

using namespace std;

const int BUF_SIZE = 1024;

void readfile(int fd, int lines);

int main(int argc, char **argv){
    int lines = 10;
    int fd; 
    char buffer[BUF_SIZE];
    
    int c; 
    while ((c = getopt(argc, argv, "n:")) != -1){
        switch(c){
            case 'n':
                lines = atoi(optarg);
                break; 
            case '?':
                if (optopt == 'n')
                    fprintf(stderr, "-%c requires an argument.\n", optopt);
                else if (isprint(optopt))
                    fprintf(stderr, "Unkown option '-%c'.\n", optopt);
                else 
                    fprintf(stderr, "Unknown option character, '\\x%x'.\n", optopt);
                return EXIT_FAILURE;
        }
    }

    if (argc == 1){
        int n; 
        while ((n = read(STDIN_FILENO, &buffer, BUF_SIZE)) > 0){
            write(STDOUT_FILENO, &buffer, n); 
        }
        return EXIT_SUCCESS;
    } else {
        for (int i = optind; i < argc; i++){ 
             fd = open(argv[i], O_RDONLY);
             readfile(fd, lines);
        }
    }
    
    close(fd); 

    return EXIT_SUCCESS;
}

void readfile(int fd, int lines){
    char buf;
    int numlines = 0;

    int n; 
    while (((n = read(fd, &buf, 1)) > 0) && (numlines < lines)){
        if (buf == '\n'){
            numlines++; 
        } 

        cout << buf;
    }
}
