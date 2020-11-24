/* Эта программа получает доступ к очереди сообщений,
и читает из нее сообщения с любым типом в порядке FIFO до тех пор,
пока не получит сообщение с типом 255, которое будет служить
сигналом прекращения работы. */

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define LAST_MESSAGE 255 /* Тип сообщения для прекращения работы */

int main()
{
    int msqid; /* IPC дескриптор для очереди сообщений */

    char pathname[] = "task1_a.c"; /* Имя файла, использующееся для генерации ключа.
Файл с таким именем должен существовать в текущей директории */

    key_t key; /* IPC ключ */

    int len, maxlen; /* Реальная длина и максимальная длина информативной части сообщения */

    /* Ниже следует пользовательская структура для сообщения */

    struct mymsgbuf
    {
        long mtype;
        int message;
    } mybuf;

    /* Генерируем IPC ключ из имени файла в текущей директории
и номера экземпляра очереди сообщений 0 */

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

    while (1)
    {

        /* В бесконечном цикле принимаем сообщения любого типа в порядке FIFO до тех пор, пока не поступит сообщение с типом LAST_MESSAGE*/

        maxlen = sizeof(int);

        if (len = msgrcv(msqid, (struct msgbuf *)&mybuf, maxlen, 0, 0) < 0)
        {
            printf("Can\'t receive message from queue\n");
            exit(-1);
        }

        /* Если принятое сообщение имеет тип LAST_MESSAGE, прекращаем работу и удаляем очередь сообщений из системы.
В противном случае печатаем текст принятого сообщения. */

        if (mybuf.mtype == LAST_MESSAGE)
        {
            msgctl(msqid, IPC_RMID, (struct msqid_ds *)NULL);
            exit(0);
        }

        printf("message type = %ld, info = %d\n", mybuf.mtype, mybuf.message);
    }

    return 0; /* Исключительно для отсутствия warning'ов при компиляции. */
}