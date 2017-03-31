#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <iostream>
#include <cstdio>
#include <fcntl.h>
#include <cstring>
#include <grp.h>
#include <pwd.h>
#include <sys/sysmacros.h>

using namespace std;

void make_perm_string(struct stat file, char * perms) {
    
    // Partially from man 2 stat
    switch (file.st_mode & S_IFMT) {
        case S_IFBLK: 
            perms[0] = 'b'; 
            break;
        case S_IFCHR: 
            perms[0] = 'c'; 
            break;
        case S_IFDIR: 
            perms[0] = 'd'; 
            break;
        case S_IFIFO: 
            perms[0] = 'p'; 
            break;
        case S_IFLNK: 
            perms[0] = 'l'; 
            break;
        case S_IFREG: 
            perms[0] = '-'; 
            break;
        case S_IFSOCK: 
            perms[0] = 's'; 
            break;
    }

    perms[1] = ((file.st_mode & S_IRUSR) ? 'r' : '-');
    perms[2] = ((file.st_mode & S_IWUSR) ? 'w' : '-');
    perms[3] = ((file.st_mode & S_IXUSR) ? 'x' : '-');

    perms[4] = ((file.st_mode & S_IRGRP) ? 'r' : '-');
    perms[5] = ((file.st_mode & S_IWGRP) ? 'w' : '-');
    perms[6] = ((file.st_mode & S_IXGRP) ? 'x' : '-');

    perms[7] = ((file.st_mode & S_IROTH) ? 'r' : '-');
    perms[8] = ((file.st_mode & S_IWOTH) ? 'w' : '-');
    perms[9] = ((file.st_mode & S_IXOTH) ? 'x' : '-');

    if (file.st_mode & S_ISUID)
        perms[3] = ((file.st_mode & S_IXUSR) ? 's' : 'S');
    if (file.st_mode & S_ISGID)
        perms[6] = (file.st_mode & S_IXGRP) ? 's' : 'S';
    if (file.st_mode & S_ISVTX)
        perms[9] = (file.st_mode & S_IXOTH) ? 't' : 'T';

    if (perms[10] != '\0')
        perms[10] = '\0';
}

int main(int argc, char * argv[]) {
    if (argc < 2) {
        cerr << "Syntax: " << argv[0] << " <filename>" << endl;
        return EXIT_FAILURE;
    }
    
  //int fd;

  //if (argv[1] == "-")
  //    fd = FILENO_STDIN;
  //else {
  //    fd = open(argv[1], O_RDONLY);

  //    if (fd == -1) {
  //        perror(argv[0]);
  //        return EXIT_FAILURE;
  //    }
  //}

    
    struct stat file;

    if (strcmp(argv[1], "-") == 0) {
        if (fstat(STDIN_FILENO, &file) == -1) {
            perror(argv[0]);
            exit(EXIT_FAILURE);
        }
    }
    else if (lstat(argv[1], &file) == -1) {
        perror(argv[0]);
        exit(EXIT_FAILURE);
    }
    
    char perm_string[10] = {};
    make_perm_string(file, perm_string); 

    printf("File: %s\n", argv[1]);

    printf("Size: %llu\t", (long long) file.st_size);
    printf("Blocks: %lu\t", file.st_blocks);
    printf("IO Block: %lu\t", file.st_blksize);
    // From man 2 stat
    switch (file.st_mode & S_IFMT) {
        case S_IFBLK:  printf("block device\n");            break;
        case S_IFCHR:  printf("character device\n");        break;
        case S_IFDIR:  printf("directory\n");               break;
        case S_IFIFO:  printf("FIFO/pipe\n");               break;
        case S_IFLNK:  printf("symlink\n");                 break;
        case S_IFREG:  printf("regular file\n");            break;
        case S_IFSOCK: printf("socket\n");                  break;
        default:       printf("unknown?\n");                break;
    }

    printf("Device: %luh/%lud\t", file.st_dev, file.st_dev);
    printf("Inode: %llu\t", (long long) file.st_ino);
    printf("Links: %d\t", (unsigned int) file.st_nlink);
    printf("Device type: %x, %x\n", major(file.st_rdev), minor(file.st_rdev));

    printf("Access: (%04o/%s)\t", 
            (file.st_mode & ~S_IFMT), perm_string);
    printf("Uid: (% 8d/%8s)\t", file.st_uid, getpwuid(file.st_uid)->pw_name);
    printf("Gid: (% 8d/%8s)\n", file.st_gid, getgrgid(file.st_gid)->gr_name);

    printf("Access: %s", ctime(&(file.st_atime)));
    printf("Modify: %s", ctime(&(file.st_mtime)));
    printf("Change: %s", ctime(&(file.st_ctime)));

    printf("Birth: %s\n", "-");

}

