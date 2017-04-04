#include <cstdio>
#include <iostream>
#include <unistd.h>
#include <cstring>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <cstring>

using namespace std;

int rmpath(const char * path, bool recursive=false, bool force=false);
bool is_directory(const char * path);
bool dir_is_empty(const char * path);

int main(int argc, char * argv[]) {
    bool recursive = false;
    bool force = false;

    if (argc < 2) {
        cerr << "Syntax: " << argv[0] << " [-rf] [file...]" << endl;
        exit(EXIT_FAILURE);
    }
    
    int opt;
    while ((opt = getopt(argc, argv, "rf")) != -1) {
        switch (opt) {
            case 'r':
                recursive = true;
                break;
            case 'f':
                force = true;
                break;
            default:
                fprintf(stderr, "Syntax: %s [-rf] [file...]\n", argv[0]);
                exit(EXIT_FAILURE);
        }
    }

    for (int i = 1; i < argc; i++) {
        if (argv[i][0] != '-') {
            rmpath(argv[i], recursive, force);
        }
    }

    exit(EXIT_SUCCESS);
}

int rmpath(const char * path, bool recursive, bool force) {
    // Do not delete if <path> is current dir or parent dir's
    if (strcmp(path, "..") == 0 || strcmp(path, ".") == 0)
        return -1;

    if (is_directory(path))
        if (recursive) {
            if (dir_is_empty(path))
                return rmdir(path);
            else {
                // for item in curr path
                DIR *dir;
                struct dirent *ent;
                char * subpath;
                if ((dir = opendir(path)) != NULL) {
                    while ((ent = readdir(dir)) != NULL) {
                        if (strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name, "..") == 0)
                            continue;
                        subpath = new char[strlen(path) + strlen(ent->d_name) + 1];
                        strcat(subpath, path);
                        strcat(subpath, "/");
                        strcat(subpath, ent->d_name);
                        rmpath(subpath, recursive, force);
                        delete subpath;
                    }
                    closedir(dir);

                    return rmdir(path);
                } else {
                    return -1;
                }
            }
        } else
            return -1;
    else {
        return unlink(path);
    }
}

bool is_directory(const char * path) {
    struct stat file_stat;
    return stat(path, &file_stat) == 0 && S_ISDIR(file_stat.st_mode);
}

// From http://stackoverflow.com/questions/6383584/check-if-a-directory-is-empty-using-c-on-linux
bool dir_is_empty(const char * path) {
    int n = 0;
    struct dirent *d;
    DIR *dir = opendir(path);
    if (dir == NULL) //Not a directory or doesn't exist
        return true;
    while ((d = readdir(dir)) != NULL) {
        if(++n > 2)
            break;
    }
    closedir(dir);
    return n <= 2;
}
