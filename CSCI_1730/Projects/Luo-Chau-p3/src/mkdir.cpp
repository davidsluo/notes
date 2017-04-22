#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <cstring>
#include <libgen.h>
#include <stack>

using namespace std;

int mkpath(char *fpath, mode_t mode);

int main(int argc, char **argv){
    int pflag = 0;
    int fmode = strtol("0755", 0, 8); //change default mode permissions here
   
    if (argc == 1) {
        cout << "Usage: ./mkdir [-p] [-m mode] files ..." << endl;
        return EXIT_FAILURE; 
    }
    
    int c; 
    while ((c = getopt(argc, argv, "m:p:")) != -1){
        switch(c){
            case 'p':
                pflag = 1; 
                break; 
            case 'm':
                fmode = strtol(optarg, 0, 8); 
                break;
            case '?':
                if (optopt == 'm')
                    fprintf(stderr, "-%c requires an argument.\n", optopt);
                else if (isprint(optopt))
                    fprintf(stderr, "Unkown option '-%c'.\n", optopt);
                else 
                    fprintf(stderr, "Unknown option character, '\\x%x'.\n", optopt);
                return EXIT_FAILURE;
        }
    }
    
    for (int i = optind; i < argc; i++){
        if (pflag){
            mkpath(argv[i], fmode);  
        } else {
            if (mkdir(argv[i], fmode) == -1){
                perror("mkdir"); 
            } else {
                chmod(argv[i], fmode); 
            }
        } 
    }

    return EXIT_SUCCESS; 
}

// Slightly modified from <a href=stackoverflow.com/questions/2336242/
// recursive-mkdir-system-call-on-unix> stackoverflow </a> to 
// consider the last item, e.g lal/lbl/lcl. lcl would generally be
// ignored.  
/**
 * @param fpath full path to create a bunch of directories.
 * @param mode mode to set all the directories created to.
 * @return an int indicating success (0) or failure (-1)
 */
int mkpath(char *fpath, mode_t mode){
    char *p;
    // strchr -> move fpath pointer to the
    // the forward slash.
    for (p = strchr(fpath + 1, '/'); p != nullptr; p = strchr(p + 1, '/')){
        *p = '\0'; //need to set null character here so mkdir makes directory name up to the dir path.
        if (mkdir(fpath, mode) == -1){
            if (errno != EEXIST){
                perror("mkdir"); 
                *p = '/';
                return -1;
            }
        } else {
            chmod(fpath, mode); 
        }
        *p = '/'; //add the / to replace the null character just created to retain the file path
    }
    
    // need to make the last directory. 
    if (mkdir(fpath, mode) == -1)
        perror("mkdir"); 
    else 
        chmod(fpath, mode);
    
    return 0;
}
