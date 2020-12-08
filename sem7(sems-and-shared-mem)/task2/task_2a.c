/* Программа 1 для иллюстрации работы с семафорами */

/* Эта программа получает доступ к одному системному семафору,
ждет пока его значение не станет больше или равным 1 после запусков программы 2,
а затем уменьшает его на 1*/

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <stdio.h>
#include <stdlib.h>

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
    int semid; /* IPC дескриптор для массива IPC семафоров */

    char pathname[] = "task_2a.c"; /* Имя файла, использующееся для генерации ключа. Файл с таким именем должен существовать в текущей директории */

    key_t key; /* IPC ключ */

    struct sembuf mybuf; /* Структура для задания операции над семафором */

    /* Генерируем IPC ключ из имени файла в текущей директории
и номера экземпляра области разделяемой памяти 0 */

    if ((key = ftok(pathname, 0)) < 0)
    {
        printf("Can\'t generate key\n");
        exit(-1);
    }

    /* Пытаемся получить доступ по ключу к массиву семафоров, если он существует,
или создать его из одного семафора, если он еще не существует, с правами доступа
read & write для всех пользователей */

    if ((semid = semget(key, 1, 0666 | IPC_CREAT)) < 0)
    {
        printf("Can\'t get semid\n");
        exit(-1);
    }

    /* Выполним операцию D(semid1,1) для нашего массива семафоров.*/
    apply(semid, -5, 0);

    printf("Condition is present\n");
    return 0;
}