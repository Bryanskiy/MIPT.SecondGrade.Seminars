#include <unistd.h>
#include <stdio.h>

int main() {
    int a = 0;
    fork();
    a++;

    printf("PID: %u\nPPID:%u\na = %d\n", getpid(), getppid(), a);
}