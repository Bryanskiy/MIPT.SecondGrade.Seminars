#include <unistd.h>
#include <stdio.h>

int main() {
    for(int i = 0; i < 2; ++i) {
        pid_t pid = fork();
        if(pid == 0) {
            sleep(i + 1);
            printf("%d\n", i);
        }
    }
}