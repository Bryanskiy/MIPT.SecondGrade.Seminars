/* Программа 2 для иллюстрации работы с семафорами */

/* Эта программа получает доступ к одному системному семафору
и увеличивает его значение на 1, чтобы разблокировать программу 1 */

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <stdlib.h>
#include <stdio.h>

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

int main()
{
    int semid;
    char pathname[] = "task_2a.c";

    key_t key;

    if ((key = ftok(pathname, 0)) < 0)
    {
        printf("Can\'t generate key\n");
        exit(-1);
    }

    if ((semid = semget(key, 1, 0666 | IPC_CREAT)) < 0)
    {
        printf("Can\'t get semid\n");
        exit(-1);
    }

    apply(semid, 1, 0);

    printf("Set condition\n");
    return 0;
}