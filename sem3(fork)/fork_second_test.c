#include <unistd.h>
#include <stdio.h>

int main() {
    printf("Hello, ");
    fork();
    printf("world!!!");
}