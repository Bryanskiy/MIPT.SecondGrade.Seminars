#include <unistd.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <sys/wait.h>

int main() {
    int N; 
    int tmp = scanf("%d", &N);
    assert(tmp == 1);

    printf("[PARENT] PID: %u\tPPID:%u\t\n", getpid(), getppid());
    for(int i = 0; i < N - 1; ++i){
        pid_t pid = fork();
        if(pid > 0) {
            break;
        }

        printf("[DOUGHTER:%d] PID: %u\tPPID:%u\t\n", i + 1, getpid(), getppid());
    }

    int status = 0;
    wait(&status);
}