#include "shared.h"

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

#ifdef LOG_ON_
void print_arr(int* arr, int start, int stop) {
    for(int i = start; i < stop; ++i) {
        printf("%d ", arr[i]);
    }

    printf("\n");
}
#endif

int main() {
#ifdef LOG_ON_
    printf("Start client with %d pid\n", getpid());
#endif

    msqid = init_msg();

    struct init_message_t init_mess;
    init_mess.mtype = getpid();
    send(msqid, (void*)&init_mess, sizeof(char));

    struct array_message_t array_mess;
    rcv(msqid, (void*)&array_mess, sizeof(struct useful_info_t), getpid());
    semid = init_sem(array_mess.info.sem_count);
    apply(semid, 1, array_mess.info.sem_id);
#ifdef LOG_ON_
    printf("Rcv arr: ");
    print_arr(array_mess.info.arr, array_mess.info.left_border, array_mess.info.right_border);
#endif
    int ans = 0;
    for(int i = array_mess.info.left_border; i < array_mess.info.right_border; ++i) {
        ans += array_mess.info.arr[i];
    }

    struct sum_message_t sum_mess;
    sum_mess.mtype = getpid();
    sum_mess.ans = ans;

    send(msqid, (void*)&sum_mess, sizeof(int));
}