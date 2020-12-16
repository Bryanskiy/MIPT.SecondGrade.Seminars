#include "shared.h"

int thread_ans[MAX_CLIENT_COUNT];
int msqid = 0;
int semid = 0;

struct useful_info_t {
    int arr[ARR_MAX_LEN];
    int left_border;
    int right_border;
    int sem_id;
    int sem_count;
};

struct array_message_t {
    long mtype;
    struct useful_info_t info;
};

struct init_message_t {
    long mtype;
    char tmp;
};

struct sum_message_t {
    long mtype;
    int ans;
};


struct thread_data_t {
    int* arr;
    int arr_size;
    int idx;
    int clients_count;
    int pid;
};

#ifdef LOG_ON_
    void print_thread_data(struct thread_data_t class) {
        printf("thread idx: %d, pid = %d\n", class.idx, class.pid);
    }
#endif

void* communocation(void* data_) {
    struct thread_data_t data = *(struct thread_data_t*)data_;

    /* create and message for client */
    struct array_message_t m;
    m.mtype = data.pid;
    memcpy(m.info.arr, data.arr, data.arr_size * sizeof(int));
    m.info.left_border = (data.arr_size / data.clients_count) * data.idx;
    if(data.idx == data.clients_count - 1) {
        m.info.right_border = data.arr_size;
    } else {
        m.info.right_border = (data.arr_size / data.clients_count) * (data.idx + 1);
    }
    m.info.sem_count = data.clients_count;
    m.info.sem_id = data.idx;
#ifdef LOG_ON_
    printf("[%d] Call: send part of array to %ld, left border = %d, right border = %d\n", data.idx , m.mtype, m.info.left_border, m.info.right_border);
#endif    
    send(msqid, (void*)&m, sizeof(struct useful_info_t));

    /* get result */
    struct sum_message_t result;
    apply(semid, -1, data.idx);
    rcv(msqid, (void*)&result, sizeof(int), data.pid);
#ifdef LOG_ON_
    printf("Get ans from %ld: %d\n", result.mtype, result.ans);
#endif
    /* write result */
    thread_ans[data.idx] = result.ans;
}

int main() {
    msqid = init_msg();
#ifdef LOG_ON_
    printf("Create mess queue with %d id\n", msqid);
#endif
    int input_data[ARR_MAX_LEN];
    int clients_pids[MAX_CLIENT_COUNT];
    int clients_count;
    int input_elements_num;
    printf("Please, enter number of elements: ");
    scanf("%d", &input_elements_num);
    if(input_elements_num > ARR_MAX_LEN) {
        fprintf(stderr, "So many elements, max number of elements: %d", ARR_MAX_LEN);
        exit(0);
    }
    printf("please, enter elements: ");
    for(int i = 0; i < input_elements_num; ++i) {
        scanf("%d", input_data + i);
    }

    printf("Enter clients count: ");
    scanf("%d", &clients_count);

    semid = init_sem(clients_count);

    for(int i = 0; i < clients_count; ++i) {
        struct init_message_t init_mess;
        rcv(msqid, (void*)&init_mess, sizeof(char), 0);       
        clients_pids[i] = init_mess.mtype;
#ifdef LOG_ON_
    printf("Get init message from %d\n", clients_pids[i]);
#endif 
    }


    pthread_t treads[MAX_CLIENT_COUNT];
    struct thread_data_t threads_data[MAX_CLIENT_COUNT];
    for(int i = 0; i < clients_count; ++i) {
        threads_data[i].arr = input_data;
        threads_data[i].arr_size = input_elements_num;
        threads_data[i].idx = i;
        threads_data[i].pid = clients_pids[i];
        threads_data[i].clients_count = clients_count;
        pthread_create(&(treads[i]), NULL, communocation, &(threads_data[i]));
    }

    for(int i = 0; i < clients_count; ++i) {
        pthread_join(treads[i], (void**)NULL);
    }

    int ans = 0;
    for(int i = 0; i < clients_count; ++i) {
        ans += thread_ans[i];
    }

    printf("ans = %d\n", ans);
    msgctl(msqid, IPC_RMID, (struct msqid_ds *)NULL);
    semctl(semid, IPC_RMID, 0);
}