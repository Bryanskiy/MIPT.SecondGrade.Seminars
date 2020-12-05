#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>

int main() {
    size_t pipe_size = 0;
    int fd[2];

    if(pipe(fd) < 0) {
        fprintf(stderr, "Cant create pipe\n");
        exit(1);
    }

    while(1) {
        write(fd[1], "a", 1);
        fprintf(stdout, "pipe size: %lu\n", pipe_size);
        ++pipe_size;}
}