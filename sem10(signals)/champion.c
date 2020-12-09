#include <stdio.h>
#include <signal.h>
#include <unistd.h>

void signal1(int s) {
    static int pid = -1;
    if(pid == -1) {
        pid = s;
    }
    printf("RT\n");
    sleep(1);
    kill(pid, SIGUSR2);
}

void signal2(int s) {
    printf("Champion\n");
    sleep(1);
    kill(getppid(), SIGUSR1);
}

int main() {
    signal(SIGUSR1, signal1);
    signal(SIGUSR2, signal2);
    int pid = fork();
    if (pid == 0) {
        while(1);
    }    
    else {
        signal1(pid);
        while(1);
    }
    return 0;
}