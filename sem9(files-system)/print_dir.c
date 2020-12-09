#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>

enum consts {
    MAX_FILE_NAME_SIZE = 256,
};

int main(int argc, char** argv) {
    if (argc != 2) {
        printf("Incorrect arg\n");
        exit(-1);
    }

    DIR* d = opendir(argv[1]);
    struct dirent* dir;
    while ((dir = readdir(d)) != NULL) {
        if (dir->d_type == 4) {
            printf("DIR:  [%s]\n", dir->d_name);
        }    
        else if (dir->d_type == 8) {
            struct stat info;
            stat(dir->d_name, &info);
            printf("FILE: [%s], %lu bytes\n", dir->d_name, info.st_size);
        }
    }
    closedir(d);
    return 0;
}