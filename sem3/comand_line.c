#include <stdio.h>

int main(int argc, char** argv, char** envp) {
    size_t i = 0;
    while(envp[i] != NULL) {
        printf("[%lu] : %s\n", i, envp[i]);
        ++i;
    }
}