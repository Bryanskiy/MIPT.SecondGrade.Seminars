#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>

int main() {
    pid_t pid = fork();
    if(pid == 0) {
        execlp("gcc", "gcc", "hello.c", "-o", "hello.out", NULL);
    }

    int status = 0;
    wait(&status);
    execlp("./hello.out", "./hello.out", NULL);
}