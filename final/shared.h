#pragma once

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <pthread.h>
#include <sys/sem.h>
#include <string.h>

enum consts {
    ARR_MAX_LEN = 100,
    MAX_CLIENT_COUNT = 10,
};

void send(int msqid, void* message, size_t len);
void rcv(int msqid, void* message, size_t maxlen ,long type);
int init_msg();
int init_sem(int n);
void apply(int semid, int n, size_t id);