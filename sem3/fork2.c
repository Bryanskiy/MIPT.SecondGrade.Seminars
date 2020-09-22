#include <unistd.h>
#include <stdio.h>

int main() {
    int a = 0;
    int pid = fork();
    if(pid == -1) {
        printf("ERROR");
    }

    else if(pid == 0) {
        printf("I am doughter proccess\n");
        a += 2;
        printf("PID: %u\tPPID:%u\ta = %d\n", getpid(), getppid(), a);
        printf("End doughter proccess\n");
    }

    else {
        printf("I am a parent\n");
        a += 4;
        printf("PID: %u\tPPID:%u\ta = %d\n", getpid(), getppid(), a);
        printf("End parent proccess\n");
    }
}