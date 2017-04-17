#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>
#include <string>
#include <cstring>
using namespace std;


bool is_dir(const string path);
int cp(string source, string target, bool recursive=false);
int copy_file(const string source, const string target);
int copy_dir(const string source, const string target);

int main (int argc, char *argv[]) {
    bool recursive = false;

    char c;
    while ((c = getopt(argc, argv, "r")) != -1) {
        switch(c) {
            case 'r':
                recursive = true;
                break;
            default:
                fprintf(stderr, "Usage: %s [-r] source-file target-file\n", argv[0]);
                fprintf(stderr, "       %s [-r] source-file target-dir/\n", argv[0]);
                exit(EXIT_FAILURE);
        }
    }

    char * source = nullptr;
    char * target = nullptr;

    for (int i = 1; i < argc; i++) {
        if (argv[i][0] != '-') {
            if (source == nullptr)
                source = argv[i];
            else
                target = argv[i];
        }
    }

    int status = 0;
    if ((status = cp(source, target, recursive)) < 0) {
        if (status == -1)
            perror(argv[0]);
        else if (status == -2)
            fprintf(stderr, "Source file does not exist.\n");
        else if (status == -3)
            fprintf(stderr, "-r not specified.\n");
        else if (status == -4)
            fprintf(stderr, "Cannot override non-directory file with directory.\n");
        exit(EXIT_FAILURE);
    } else {
        exit(EXIT_SUCCESS);
    }
}

bool exists(string path) {
    struct stat file_stat;
    return stat(path.c_str(), &file_stat) == 0;
}
bool is_dir(string path) {
    struct stat file_stat;
    return stat(path.c_str(), &file_stat) == 0 && S_ISDIR(file_stat.st_mode);
}

/**
 * @return
 *  -1 if perror error
 *  -2 if source doesnt exist
 *  -3 if trying to copy directory not recursively
 *  -4 if trying to copy directory into file
 *  -5 if source and target are the same.
 */
int cp(string source, string target, bool recursive) {
    if (source == target)
        return 0;

    if (exists(source)) {
        if (is_dir(source)) {
            if (recursive) {
                DIR *dir;
                struct dirent *ent;
                string new_target;
                string new_source;
                int status;
                //*************************************************************
                if (exists(target)) {
                    if (is_dir(target)) {
                        // recursively copy source into target/source
                        string base_source = "";
                        base_source += source.substr(source.find_last_of("/"));

                        string base_target = "";
                        base_target += target.substr(target.find_last_of("/"));
                        
                        if (base_source == base_target) {
                            if ((dir = opendir(source.c_str())) != NULL) {
                                while ((ent = readdir(dir)) != NULL) {
                                    // Ignore parent and current directory aliases
                                    if (strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name, "..") == 0)
                                        continue;
                                    
                                    new_target = "";
                                    new_target += target;
                                    new_target += "/";
                                    new_target += ent->d_name;

                                    new_source = "";
                                    new_source += source;
                                    new_source += "/";
                                    new_source += ent->d_name;

                                    status = cp(new_source, new_target, recursive);

                                    if (status < 0) {
                                        return status;
                                    }

                                }
                            } else {
                                return -1;
                            }
                        } else {
                            new_target = "";
                            new_target += target;
                            new_target += base_source;

                            status = cp(source, new_target, recursive);

                            return status;
                        }

                    } else {
                        // Cannot override non-directory file (target) 
                        // with directory (source)
                        return -4;
                    }
                } else {
                    // copy source with new name target
                    {
                        int status = copy_dir(source, target) < 0;

                        if (status < 0)
                            return status;
                    }
                    if ((dir = opendir(source.c_str())) != NULL) {
                        while ((ent = readdir(dir)) != NULL) {
                            // Ignore parent and current directory aliases
                            if (strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name, "..") == 0)
                                continue;

                            new_target = "";
                            new_target += target;
                            new_target += "/";
                            new_target += ent->d_name;

                            new_source = "";
                            new_source += source;
                            new_source += "/";
                            new_source += ent->d_name;

                            status = cp(new_source, new_target, recursive);

                            if (status < 0)
                                return status;

                        }
                        return status;
                    } else {
                        return -1;
                    }
                }
                //*************************************************************

            } else {
                // Recursive not specified; cannot copy.
                return -3;
            }
        } else {
            if (is_dir(target)) { // copy source into target/source
                string new_target = "";
                string base_source = "";

                base_source += source.substr(source.find_last_of("/"));

                new_target += target;
                //strcat(new_target, "/");
                new_target += base_source;
                int status = copy_file(source, new_target);
                return status;
            } else {
                // make copy of source named target
                return copy_file(source, target);
            }

        }
    } else {
        // source doesnt exist; cant copy
        return -2;
    }

    return 0;
}

int copy_dir(string source, string target) {
    DIR* s_dir = opendir(source.c_str());
    if (s_dir == NULL)
        return -1;

    struct stat dir_stat;
    if (stat(source.c_str(), &dir_stat) < 0) {
        closedir(s_dir);
        return -1;
    }

    int s_mode = dir_stat.st_mode;

    int status = mkdir(target.c_str(), s_mode);

    closedir(s_dir);

    return status;
}

// Assumes source exists and path to target is valid
// Overwrites target
int copy_file(string source, string target) {
    int fds = open(source.c_str(), O_RDONLY);
    if (fds < 0) return -1;

    struct stat file_stat;
    if (stat(source.c_str(), &file_stat) < 0)
        return -1;
    int s_mode = file_stat.st_mode;

    int fdt = creat(target.c_str(), s_mode);
    if (fdt < 0) {
        close(fds);
        return -1;
    }

    const int BUFFER_SIZE = 1024;
    char buffer[BUFFER_SIZE] = {}; 
    int r = 0;
    while ((r = read(fds, buffer, BUFFER_SIZE)) > 0) {
        if (write(fdt, buffer, r) < 0) {
            close(fds);
            close(fdt);
            return -1;
        }
    }
    close(fds);
    close(fdt);

    return 0;
}
