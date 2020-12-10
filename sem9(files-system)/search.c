#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>

typedef struct input_data_t {
    char* abs_start_dir;
    size_t search_deep;
    char* searching_file;
} input_data_t;

enum consts {
    FILE_ID = 8,
    DIR_ID = 4,
};

input_data_t read_input(int argc, char** argv) {
    input_data_t ret;
    if(argc != 4) {
        fprintf(stderr, "Wrong input, expected: start dir, deep, name of searching file\n");
        exit(1);
    }

    ret.abs_start_dir = realpath(argv[1], NULL);
    ret.search_deep = atoi(argv[2]);
    ret.searching_file = argv[3];

    return ret;
}

#ifdef LOG_ON_
void print_input_data(input_data_t input) {
    fprintf(stdout, "input dir: [ %s ]\n", input.abs_start_dir);
    fprintf(stdout, "searching deep = %lu\n", input.search_deep);
    fprintf(stdout, "searching file: [ %s ]\n", input.searching_file);
}
#endif

void free_input(input_data_t input) {
    free(input.abs_start_dir);
}

/* allocate mem */
char* duplicate(char* str) {
    char* ret;
    ret = (char*)calloc(PATH_MAX, sizeof(char));
    if(NULL == ret) {
        fprintf(stderr, "Bad alloc\n");
        exit(1);
    }

    strcpy(ret, str);
    return ret;
}

size_t find(char* abs_dir, char* file_name, size_t deep) {
    if(0 == deep) {
        return 0;
    }

    DIR* current = opendir(abs_dir);
    struct dirent* dir;
    while ((dir = readdir(current)) != NULL) {
        if(!strcmp(dir->d_name, ".") || !strcmp(dir->d_name, "..")) {
            continue;
        }

        if(dir->d_type == FILE_ID) {
            if(!strcmp(dir->d_name, file_name)) {
                printf("Find file at [ %s ]\n", abs_dir);
            }
        }

        else if(dir->d_type == DIR_ID) {
            char* new_dir = duplicate(abs_dir);
            strcat(new_dir, "/");
            strcat(new_dir, dir->d_name);
            find(new_dir, file_name, deep - 1);
            free(new_dir);        
        }
    }

    return 0;
}

int main(int argc, char** argv) {
    input_data_t input = read_input(argc, argv);
#ifdef LOG_ON_
    print_input_data(input);
#endif

    find(input.abs_start_dir, input.searching_file, input.search_deep);

    free_input(input);
}