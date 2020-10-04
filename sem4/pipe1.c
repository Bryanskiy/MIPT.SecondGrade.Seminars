#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>

int main() {
    int coefs[2];
    if(pipe(coefs) < 0) {
        fprintf(stderr, "Cant create pipe coefs\n");
        exit(1);
    }

    int out[2];
    if(pipe(out) < 0) {
        fprintf(stderr, "Cant create pipe out\n");
        exit(1);
    }


    size_t max_len = 100;
    pid_t pid = fork();

    if(pid == -1) {
        fprintf(stderr, "Cant create fork\n");
        exit(1);
    }

    if(pid == 0) {
        char* input = (char*)calloc(max_len, sizeof(char));
        read(coefs[0], input, max_len);
        char* a = strtok(input, " ");
        char* b = strtok(NULL, " ");

        int int_a = atoi(a);
        int int_b = atoi(b);

        int ret = int_a * int_b;
        char* output = (char*)calloc(max_len, sizeof(char));
        size_t i = 0;
        if(ret < 0) {
            output[0] = '-';
            ++i;
        }
        while(ret != 0) {
            int digit = ret % 10;
            output[i] = digit + '0';
            digit = digit / 10;
        }
        write(out[1], output, max_len);   
    }

    if(pid > 0) {
        char* input = (char*)calloc(max_len, sizeof(char));
        read(0, input, max_len);
        write(coefs[1], input, max_len);
        
        char* output = (char*)calloc(max_len, sizeof(char));
        read(out[0], output, max_len);
        write(0, output, max_len);
    }
}