#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>

enum consts {
    max_file_size = 1000
};

int main(int argc, char** argv) {
    if(argc != 2) {
        fprintf(stderr, "expected file name\n");
        exit(1);
    }

    int fd = open(argv[1], O_CREAT | O_RDONLY, 0400);
    if(fd == -1) {
        fprintf(stderr, "can't open file\n");
        exit(1);
    }

    size_t N;
    scanf("%lu", &N);

    char* buffer = (char*)calloc(max_file_size + 1, sizeof(char));
    size_t num = read(fd, buffer, max_file_size);

    int ret;
    int i = 0;
    do {
        ret = write(0, buffer + i, N);
        i += N;
        printf("\n");
    }  while(i <= num);

    close(fd);   
}