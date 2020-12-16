#include "shared.h"

void send(int msqid, void* message, size_t len)
{
    if (msgsnd(msqid, message, len, 0) < 0)
    {
        printf("Can\'t send message to queue\n");
        msgctl(msqid, IPC_RMID, (struct msqid_ds *)NULL);
        exit(-1);
    }
}

void rcv(int msqid, void* message, size_t maxlen ,long type)
{
    int len;
    if (len = msgrcv(msqid, message, maxlen, type, 0) < 0)
    {
        printf("Can\'t receive message from queue\n");
        exit(-1);
    }
}


int init_msg()
{
    int msqid;
    char pathname[] = "server.c";
    key_t key;

    if ((key = ftok(pathname, 0)) < 0)
    {
        printf("Can\'t generate key\n");
        exit(-1);
    }

    if ((msqid = msgget(key, 0666 | IPC_CREAT)) < 0)
    {
        printf("Can\'t get msqid\n");
        exit(-1);
    }

    return msqid;
}

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
    char pathname[] = "server.c";

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