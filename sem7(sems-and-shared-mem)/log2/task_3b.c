#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
    FILE* log = fopen("log.txt", "a");

    for(size_t i = 0; i < 10; ++i) {
        sleep(1);
        fprintf(log, "[B] process\n");
    }
}