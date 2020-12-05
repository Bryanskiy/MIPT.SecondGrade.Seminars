#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>

int main() {
    int pipe2to1[2];
    int pipe1to2[2];

    if(pipe(pipe1to2) < 0 || pipe(pipe2to1) < 0) {
        fprintf(stderr, "Can't create pipe\n");
        exit(1);
    }

    pid_t pid = fork();
    if(pid < 0) {
        fprintf(stderr, "Can't create a process\n");
        exit(1);
    }

    // first process (parent)
    if(pid > 0) {
        int input1[2];
        scanf("%d %d", &input1[0], &input1[1]);
        if(write(pipe1to2[1], input1, sizeof(int) * 2) != sizeof(int) * 2) {
            fprintf(stderr, "Write input error\n");
            exit(1);
        }

        int ans1;

        if(read(pipe2to1[0], &ans1, sizeof(int)) != sizeof(int)) {
            fprintf(stderr, "Read ans error\n");
            exit(1);
        }

        printf("ans: %d", ans1);
    }

    // second process (doughter)
    else {
        int input2[2];
        if(read(pipe1to2[0], input2, sizeof(int) * 2) != sizeof(int) * 2) {
            fprintf(stderr, "Read input error\n");
            exit(1);
        }

        int ans2 = input2[0] * input2[1];

        if(write(pipe2to1[1], &ans2, sizeof(int)) != sizeof(int)) {
            fprintf(stderr, "Write ans error\n");
            exit(1);
        }
    }
}