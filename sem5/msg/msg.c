#include <stdio.h>
#include <unistd.h>
#include<fcntl.h>

int main(int argc, char** argv) {
    if(argc < 2) {
        fprintf(stderr, "Expected client argument");
        exit(1);
    }

    int fd0to1, fd1to2;
    char* fifo0to1_name = "msg_fifo0to1.fifo";
    char* fifo1to0_name = "msg_fifo1to0.fifo";

    if(mknod(fifo0to1_name, S_IFIFO, 0666, 0) < 0) {
        fprintf(stderr, "Can't create FIFO");
        exit(1);
    }

    if(mknod(fifo1to0_name, S_IFIFO, 0666, 0) < 0) {
        fprintf(stderr, "Can't create FIFO");
        exit(1);
    }

    pid_t pid = fork();
    if(pid < 0) {
        fprintf(stderr, "Can't create process");
        exit(1);
    }

    if(argv[1] == '0') {

    }

    while(1) {
        //for reading
        if(pid == 0) {
            
        }

        //for writing
        if(pid > 0) {
            
        }        
        
    }
}