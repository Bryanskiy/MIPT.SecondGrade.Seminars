#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int main() {
    printf("[PARENT] PID: %u\tPPID:%u\t\n", getpid(), getppid());
    for(int i = 0; i < 5; ++i) {
        pid_t pid = fork();
        if(pid == 0) {
            printf("[DOUGHTER] PID: %u\tPPID:%u\t\n", getpid(), getppid());
            break;
        }
    }
}