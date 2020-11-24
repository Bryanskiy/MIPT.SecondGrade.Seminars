#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void apply(int semid, int n, size_t id) {
    struct sembuf mybuf;
    mybuf.sem_op = n;
    mybuf.sem_flg = 0;
    mybuf.sem_num = id;

    printf("apply %d to %lu\n", n, id);
    if (semop(semid, &mybuf, 1) < 0)
    {
        fprintf(stderr, "Can\'t wait for condition\n");
        exit(-1);
    }
}

int main()
{
    int semid;
    char pathname[] = "dead_lock.c";

    key_t key; /* IPC ключ */

    struct sembuf mybuf;

    if ((key = ftok(pathname, 0)) < 0)
    {
        fprintf(stderr, "Can\'t generate key\n");
        exit(-1);
    }

    if ((semid = semget(key, 2, 0666 | IPC_CREAT)) < 0)
    {
        printf("Can\'t get semid\n");
        exit(-1);
    }

    apply(semid, 1, 0);
    apply(semid, 1, 1);

    pid_t pid = fork();
    if (pid < 0)
    {
        fprintf(stderr, "Can't fork\n");
        exit(1);
    }
    else if (pid == 0)
    {
        while (1)
        {
            apply(semid, -1, 0);
            apply(semid, -1, 1);
            apply(semid, 1, 0);
            apply(semid, 1, 1);
        }
    }
    else
    {
        while (1)
        {
            apply(semid, -1, 1);
            apply(semid, -1, 0);
            apply(semid, 1, 0);
            apply(semid, 1, 1);
        }
    }
}