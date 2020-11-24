#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

enum consts
{
    LAST_MESSAGE = 255,
};

struct mymsgbuf
{
    long mtype;
    long long int number;
};

void send(int msqid, struct mymsgbuf message, size_t len)
{
    if (msgsnd(msqid, (struct msgbuf *)&message, len, 0) < 0)
    {
        printf("Can\'t send message to queue\n");
        msgctl(msqid, IPC_RMID, (struct msqid_ds *)NULL);
        exit(-1);
    }
}

void rcv(int msqid, struct mymsgbuf* message, size_t maxlen ,long type)
{
    int len;
    if (len = msgrcv(msqid, message, maxlen, type, 0) < 0)
    {
        printf("Can\'t receive message from queue\n");
        exit(-1);
    }
}

int init()
{
    int msqid; /* IPC дескриптор для очереди сообщений */
    char pathname[] = "task2_a.c"; /* Имя файла, использующееся для генерации ключа.
Файл с таким именем должен существовать в текущей директории */
    key_t key;  /* IPC ключ */

    /* Генерируем IPC ключ из имени файла в текущей директории
и номера экземпляра очереди сообщений 0. */

    if ((key = ftok(pathname, 0)) < 0)
    {
        printf("Can\'t generate key\n");
        exit(-1);
    }

    /* Пытаемся получить доступ по ключу к очереди сообщений, если она существует,
или создать ее, если она еще не существует, с правами доступа
read & write для всех пользователей */

    if ((msqid = msgget(key, 0666 | IPC_CREAT)) < 0)
    {
        printf("Can\'t get msqid\n");
        exit(-1);
    }

    return msqid;
}

int main()
{
    srand(time(NULL));

    int msqid = init();

    struct mymsgbuf input;
    long long int input_number;
    scanf("%lld", &input_number);
    input.mtype = 1;
    input.number = input_number;
    send(msqid, input, sizeof(long long int));

    struct mymsgbuf output;
    rcv(msqid, &output, sizeof(long long int), 2);
    printf("Factorial = %lld", output.number);

    msgctl(msqid, IPC_RMID, (struct msqid_ds *)NULL);
    return 0;
}