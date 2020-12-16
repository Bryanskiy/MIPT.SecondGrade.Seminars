#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <pthread.h>
#include <sys/sem.h>

#ifdef LOG_ON_ 
static inline void print_block_separator(FILE* out) {
    fprintf(out, "===============================================\n");
}
#endif

typedef struct request_kind_t {
    size_t id;
    size_t washer_time;
    size_t dryer_time;
} request_kind_t;

typedef struct thread_data_t {
    size_t* table_ptr;
    size_t table_size;
    request_kind_t* requests_kinds;
    size_t requests_kinds_num;
    size_t request_count;
    int semid;
} thread_data_t;

enum sems {
    FULL = 1,
    EMPTY = 0,
    MUTEX = 2
};

void apply(int semid, int n, size_t id) {
    struct sembuf mybuf;
    mybuf.sem_op = n;
    mybuf.sem_flg = 0;
    mybuf.sem_num = id;

    if (semop(semid, &mybuf, 1) < 0)
    {
        fprintf(stderr, "Can\'t wait for condition\n");
        exit(-1);
    }
}

int init_sem(int num) {
    int semid;
    char pathname[] = "table.c";

    key_t key;

    if ((key = ftok(pathname, 0)) < 0) {
        printf("Can\'t generate key\n");
        exit(-1);
    }

    if ((semid = semget(key, num, 0666 | IPC_CREAT)) < 0) {
        printf("Can\'t get semid\n");
        exit(-1);
    }

    return semid;
}

request_kind_t* init_requests(FILE* input, size_t request_count) {
    request_kind_t* ret = (request_kind_t*)calloc(request_count, sizeof(request_kind_t));
    if(ret == NULL) {
        fprintf(stderr, "Can't allocate memory\n");
        exit(1);
    }

    for(size_t i = 0; i < request_count; ++i) {
        int check = fscanf(input, "%lu %lu %lu", &(ret[i].id), &(ret[i].washer_time), &(ret[i].dryer_time));
        if(check != 3) {
            fprintf(stderr, "Incorrect requests input\n");
            exit(1);
        }
    }

    return ret;
}  

size_t find_washer_time(request_kind_t* arr, size_t size, size_t id) {
    for(size_t i = 0; i < size; ++i) {
        if(arr[i].id == id) {
            return arr[i].washer_time;
        }
    }

    fprintf(stderr, "Can't find request with %lu id\n", id);
    exit(-1);
}

size_t find_dryer_time(request_kind_t* arr, size_t size, size_t id) {
    for(size_t i = 0; i < size; ++i) {
        if(arr[i].id == id) {
            return arr[i].dryer_time;
        }
    }

    fprintf(stderr, "Can't find request with %lu id\n", id);
    exit(-1);
}

void* washer_main_func(void* data_) {
    thread_data_t data = *(thread_data_t*)data_;

    for(size_t i = 0; i < data.request_count; ++i) {
        size_t id;
        int ret = scanf("%lu", &id);
        if(ret != 1) {
            fprintf(stderr, "Wrong id input\n");
            exit(-1);
        }
        size_t washer_time = find_washer_time(data.requests_kinds, data.requests_kinds_num, id);

        printf("Washing %lu, time: %lu\n", id, washer_time);
        sleep(washer_time);
        apply(data.semid, -1, EMPTY);
        apply(data.semid, -1, MUTEX);

        for(size_t i = 0; i < data.table_size; ++i) {
            if(data.table_ptr[i] == 0) {
                data.table_ptr[i] = id;
            }
        }
        printf("Add %lu to the table\n", id);
        apply(data.semid, 1, MUTEX);
        apply(data.semid, 1, FULL);
    }
}

void* dryer_main_func(void* data_) {
    thread_data_t data = *(thread_data_t*)data_;
    for(size_t i = 0; i < data.request_count; ++i) {
        apply(data.semid, -1, FULL);
        apply(data.semid, -1, MUTEX);

        size_t id;
        for(size_t i = 0; i < data.table_size; ++i) {
            if(data.table_ptr[i] != 0) {
                id = data.table_ptr[i];
                data.table_ptr[i] = 0;
            }
        }

        size_t dryer_time = find_dryer_time(data.requests_kinds, data.requests_kinds_num, id);
        printf("Start drying %lu, drying time: %lu\n", id, dryer_time);
        sleep(dryer_time);
        printf("Finish drying %lu\n", id);

        apply(data.semid, 1, MUTEX);
        apply(data.semid, 1, EMPTY);

    }
}

void work(size_t table_size, size_t request_count, request_kind_t* requests_kinds, size_t requests_kinds_count) {
    /* id array */
    size_t* table = (size_t*)calloc(table_size, sizeof(size_t));
    if(NULL == table) {
        fprintf(stderr, "Can't allocate memory\n");
        exit(1);
    }

    pthread_t washer, dryer; 
    int washer_id, dryer_id;

    /* 0 - empty, 1 - full, 2 - mutex */
    int semid = init_sem(3);
    apply(semid, table_size, EMPTY);
    apply(semid, 1, MUTEX);

    thread_data_t thread_data = {table, table_size, requests_kinds, requests_kinds_count, request_count, semid};
    washer_id = pthread_create(&washer, (pthread_attr_t*)NULL, washer_main_func, &thread_data);
    dryer_id = pthread_create(&dryer,(pthread_attr_t*)NULL, dryer_main_func, &thread_data);

    if(washer_id != 0) {
      printf ("Error on thread create, return value = %d\n", washer_id);
      exit(-1);
   } 

   if(dryer_id != 0) {
      printf ("Error on thread create, return value = %d\n", dryer_id);
      exit(-1);
   }     

    pthread_join(washer, (void**)NULL);
    pthread_join(dryer, (void**)NULL);

    free(table);
}

int main(int argc, char** argv) {
    /* check main arguments */
    if(argc != 2) {
        fprintf(stderr, "Expected only requests file name\n");
        exit(1);
    }

    /* input table size*/ 
    size_t table_size;
    printf("Enter count of table places\n");
    int ret = scanf("%lu", &table_size);
    if(ret != 1) {
        fprintf(stderr, "Incorrect input\n");
        exit(1);
    }

    /* input request count */ 
    size_t request_count;
    printf("Enter request count\n");
    ret = scanf("%lu", &request_count);
    if(ret != 1) {
        fprintf(stderr, "Incorrect input\n");
        exit(1);
    }
    /* open request file */
    FILE* requests_file = fopen(argv[1], "r");
    if(requests_file == NULL) {
        fprintf(stderr, "Can't open requests file\n");
        exit(1);
    }
    size_t requests_kinds_count;
    ret = fscanf(requests_file, "%lu", &requests_kinds_count);
    if(ret != 1) {
        fprintf(stderr, "Incorrect count of requests kinds\n");
        exit(1);
    }

    printf("Start working!\n");
    request_kind_t* requests_kinds = init_requests(requests_file, requests_kinds_count);
#ifdef LOG_ON_
    print_block_separator(stdout);
    fprintf(stdout, "count of requests kinds = %lu, requests: \n", requests_kinds_count);
    for(size_t i = 0; i < requests_kinds_count; ++i) {
        fprintf(stdout, "[%lu] request: id = %lu, washer time = %lu, disher time = %lu\n", i, 
            requests_kinds[i].id, requests_kinds[i].washer_time, requests_kinds[i].disher_time);
    }
#endif    
    work(table_size, request_count, requests_kinds, requests_kinds_count);

    fclose(requests_file);
    free(requests_kinds);
}