#include <stdio.h>
#include <stdlib.h>
#include <string.h>

size_t split(char* input, char** output) {
    char* word = strtok(input, " \t\n");
    size_t i = 0;

    for (i = 0; word != NULL; i++) {
        output[i] = word;
        word = strtok(NULL, " \t\n");
    }

    return i;
}

size_t get_file_size(FILE* file) {
    fseek(file, 0L, SEEK_END); 
    size_t res = ftell(file);
    fseek(file, 0, SEEK_SET);

    return res;
}

size_t read(char const * file_name, char** ptr) {
    FILE* file = fopen(file_name, "r");
    if (file == NULL) {
        fprintf(stderr, "File %s not found\n", file_name);
        exit(1);
    }

    size_t size = get_file_size(file);
    if(size == 0) {
        fprintf(stderr, "Input file is empty\n");
        exit(1);
    }

    *ptr = (char*)malloc((size + 1) * sizeof(char));
    if(*ptr == NULL) {
        fprintf(stderr, "Bad alloc");
        exit(1);
    }

    fread(*ptr, sizeof(char), size, file);
    ptr[size] = '\0';

    fclose(file);
    return size;
}

void print(char** strings, size_t num) {
    for(size_t i = 0; i < num; ++i) {
        printf("%s\n", strings[i]);
    }
}

int main(int argc, char** argv) {

    char* buffer = NULL;
    size_t size = read(argv[1], &buffer);

    char** output = (char**)malloc(size * sizeof(char*));
    size_t num = split(buffer, output);
    print(output, num);

    free(buffer);
    free(output);

    return 0;
}
