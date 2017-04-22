#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <string>
#include <vector>

using namespace std;

void readfile(int fd, int num, bool nflag, bool cflag);

int main(int argc, char **argv){
    int lines = 10;
    int bytes = 0;
    int fd; 
    bool fflag = false, cflag = false, nflag = false;
    
    int c; 
    while ((c = getopt(argc, argv, "f:n:c:")) != -1){ 
        switch(c){
            case 'n': //number of lines
                if (cflag){
                    cout << "Usage: ./tail [-f] [-c number | -n number] [file]" << endl;
                    return EXIT_FAILURE; 
                }
                lines = atoi(optarg);
                nflag = true;
                break; 
            case 'c': //file offset (read from the offset to the end of the file.
                if (nflag){
                    cout << "Usage: ./tail [-f] [-c number | -n number] [file]" << endl;
                    return EXIT_FAILURE; 
                }
                bytes = atoi(optarg);
                cflag = true;
                break;
            case 'f': //continue to read if input file is FIFO or regular file; read & copy further bytes from input when they 
                      //become available. if no file operand and stdin is a pipe, ignore fflag. 
                fflag = true;
                break;             
            case '?':
                if (optopt == 'c')
                    fprintf(stderr, "-%c requires an argument.\n", optopt);
                else if (optopt == 'n')
                    fprintf(stderr, "-%c requires an argument.\n", optopt);
                else if (isprint(optopt))
                    fprintf(stderr, "Unkown option '-%c'.\n", optopt);
                else 
                    fprintf(stderr, "Unknown option character, '\\x%x'.\n", optopt);

                return EXIT_FAILURE;
        }
    }

    // if no options, then set nflag.
    if (!(nflag || cflag)) 
        nflag = true;

    // if no file operand and stdin is a pipe, change fflag back to false.

    if (argc == 1){
        char buffer; 
        string output = ""; 
        
        int n;
        while ((n = read(STDIN_FILENO, &buffer, 1)) > 0){
            output += buffer; 
        }

        cout << output;
    } else {
        int i;
        for (i = optind; i < argc; i++){ 
             if (strcmp(argv[i], "-") == 0){
                char buffer; 
                string output = ""; 
                
                int n;
                while ((n = read(STDIN_FILENO, &buffer, 1)) > 0){
                    output += buffer;
                }

                cout << output;
                continue;            
             } else {
                 printf("<=== %s ===>\n", argv[i]); 
                  
                 fd = open(argv[i], O_RDONLY);
                 int num = (cflag) ? (bytes) : (lines); 
                 readfile(fd, num, nflag, cflag);
                 close(fd); 
            }
        } 
        
        if (fflag){ // check if file is a regular file or a FIFO
            char *lastfile = argv[argc - 1]; 
           
            int fd = open(lastfile, O_RDONLY); 
          
            if (fd < 0)
                perror("open"); 
            
            char buf; 
            int n;
        
            while (true){ 
                while ((n = read(fd, &buf, 1)) > 0){
                    cout << buf;       
                }
            }
        }
    }  

    return EXIT_SUCCESS; 
}

void readfile(int fd, int num, bool nflag, bool cflag){
    char buf;
    int i = 0;

    struct stat file;
    fstat(fd, &file);

    int pos;
    lseek(fd, 0, SEEK_END);
    pos = file.st_size; 

    int n; 
    while (i < (num + 1)){
        lseek(fd, --pos, SEEK_SET);
        if ((n = read(fd, &buf, 1) > 0)){
            if (nflag && buf == '\n'){
                i++; 
            } else if (cflag){
                i++; 
            }
        }
    }

    while ((n = read(fd, &buf, 1)) > 0){
        cout << buf;
    }
}
