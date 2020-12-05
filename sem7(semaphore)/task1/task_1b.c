#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

size_t get_file_size(FILE* file) {
    fseek(file, 0L, SEEK_END); 
    size_t res = ftell(file);
    fseek(file, 0, SEEK_SET);

    return res;
}

int setup_shared_memory(void** array, int* shmid, char* pathname, size_t size) {
    int new = 1;
    key_t key;
    if ((key = ftok(pathname, 0)) < 0) {
        printf("Can't generate key\n");
        exit(-1);
    }
    if ((*shmid = shmget(key, size, 0666 | IPC_EXCL | IPC_CREAT)) < 0) {
        if (errno != EEXIST) {
            printf("Can't create shared memory\n");
            exit(-1);
        }
        else {
            if ((*shmid = shmget(key, size, 0)) < 0) {
                printf("Can't find shared memory\n");
                exit(-1);
            }
            new = 0;
        }
    }

    if ((*array = shmat(*shmid, NULL, 0)) == (void*)(-1)) {
        printf("Can't attach shared memory\n");
        exit(-1);
    }

    return new;
}

int main() {
    char *main_array;
    int main_shmid;
    char pathname[] = "task_1a.c";

    FILE* writing_file = fopen(pathname, "r");
    if(writing_file == NULL) {
        printf("Can't open writing file\n");
        exit(-1);
    }

    size_t writing_file_size = get_file_size(writing_file);

    setup_shared_memory((void*)&main_array, &main_shmid, pathname, writing_file_size);

    printf("%s\n", main_array);

    if (shmdt(main_array) < 0) {
        printf("Can't detach shared memory");
        exit(-1);
    }

    fclose(writing_file);
    shmctl(main_shmid, IPC_RMID, NULL);

    return 0;
}