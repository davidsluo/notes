#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <grp.h>
#include <pwd.h>

using namespace std;

void print_finfo(char *filename);
bool is_dir(char *path);
int direntcmp(const struct dirent **a, const struct dirent **b);

int main(int argc, char **argv) {
    int aflag = 0;
    int lflag = 0;

    int c; 
    while ((c = getopt(argc, argv, "al::")) != -1) {
        switch(c) {
            case 'a':
                aflag = 1; 
                break; 
            case 'l':
                lflag = 1;
                break;
            case '?':
                if (isprint(optopt))
                    fprintf(stderr, "Unkown option '-%c'.\n", optopt);
                else 
                    fprintf(stderr, "Unknown option character, '\\x%x'.\n", optopt);
                return EXIT_FAILURE;
        }
    }

    int i = optind;
    do { 
        struct dirent **fnamelist;

        const char *fp = (argv[i] == nullptr) ? (".") : argv[i];

        int numfiles = scandir(fp, &fnamelist, NULL, direntcmp);
        if (numfiles < 0) {
            perror("ls");
        } else {
            if (is_dir(argv[i])){
                printf("%s:\n", argv[i]);
                if (!aflag){
                    printf(" total: %d\n", numfiles - 2);    
                } else{
                    printf(" total: %d\n", numfiles); 
                }
            }
            
            for (int n = 0; n < numfiles; n++) {
                if (!aflag && (fnamelist[n]->d_name[0] == '.')) {
                    n++; 
                    continue; 
                }
                
                if (lflag) {
                    if (is_dir(argv[i])) {
                        char buffer[100]; // change buffer size so it fits everything...
                        snprintf(buffer, 100, "%s/%s", fp, fnamelist[n]->d_name); 
                        print_finfo(buffer);
                    } else {
                        print_finfo(fnamelist[n]->d_name); 
                    }
                }

                printf("%s ", fnamelist[n]->d_name);

                if (lflag)
                    cout << endl; 
                

                free(fnamelist[n]); 
            }
            cout << endl; 
            free(fnamelist);
        }
        i++;
    } while (i < argc); 

    return EXIT_SUCCESS; 
}

void print_finfo(char *filename) {
    struct stat buf;
    stat(filename, &buf);  

    printf((S_ISDIR(buf.st_mode)) ? "d" : "-");
    printf((buf.st_mode & S_IRUSR) ? "r" : "-");
    printf((buf.st_mode & S_IWUSR) ? "w" : "-");

    if ((buf.st_mode & S_ISUID) && (buf.st_mode & S_IXUSR))
        printf("S");
    else if (buf.st_mode & S_ISUID)
        printf("s");
    else if (buf.st_mode & S_IXUSR)
        printf("x");
    else 
        printf("-");

    printf((buf.st_mode & S_IRGRP) ? "r" : "-");
    printf((buf.st_mode & S_IWGRP) ? "w" : "-");

    if ((buf.st_mode & S_ISGID) && (buf.st_mode & S_IXGRP))
        printf("S");
    else if (buf.st_mode & S_ISUID)
        printf("s");
    else if (buf.st_mode & S_IXUSR)
        printf("x");
    else 
        printf("-");

    printf((buf.st_mode & S_IROTH) ? "r" : "-");
    printf((buf.st_mode & S_IWOTH) ? "w" : "-");

    if ((buf.st_mode & S_ISVTX) && (buf.st_mode & S_IXOTH))
        printf("T");
    else if (buf.st_mode & S_ISVTX)
        printf("t");
    else if (buf.st_mode & S_IXOTH)
        printf("x");
    else
        printf("-"); 

    printf(" %lu", buf.st_nlink);

    struct passwd *ubuf = getpwuid(buf.st_uid); 
    struct group *gbuf = getgrgid(buf.st_gid); 
    printf("  %s ", ubuf->pw_name); 
    printf(" %s ", gbuf->gr_name); 

    printf(" %li ", buf.st_size);

    char tbuf[26];
    struct tm *timeinfo;
    timeinfo = localtime(&buf.st_mtime);
    strftime(tbuf, 26, "%b %d %H:%M", timeinfo);  
    printf(" %s ", tbuf);
}

bool is_dir(char *path) {
    struct stat s;
    stat(path, &s);

    return S_ISDIR(s.st_mode);
}

int direntcmp(const struct dirent **a, const struct dirent **b){
    return strcasecmp((*a)->d_name, (*b)->d_name);
}
