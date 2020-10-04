#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>

size_t file_size() {

}

int main() {
    int max_size = 1000;
    int fd = open("read.txt", O_CREAT | O_RDONLY, 0777);
    if(fd == -1) {
        exit(1);
    }

    char* buffer = (char*)calloc(max_size, sizeof(char));
    size_t num = read(fd, buffer, max_size);

    write(0, buffer, max_size);
    close(fd);   
}