#pragma once

#include <math.h>
#include <errno.h>
#include <sys/sysinfo.h>
#include <pthread.h>

struct thread_data_t {
    struct matrix_t A, B, C;
    size_t m_start, m_end;
    pthread_t thid;
};

void* mult_part(void* data) {
    struct thread_data_t tmp = *(struct thread_data_t*)data;
    for(size_t i = tmp.m_start; i < tmp.m_end; ++i) {
        for(size_t j = 0; j < tmp.B.n; ++j) {
            double elem = 0.0;
            for(size_t k = 0; k < tmp.A.n; ++k) {
                elem += tmp.A.elem[i][k] * tmp.B.elem[k][j];
            }

            tmp.C.elem[i][j] = elem;
        }
    }
}

struct matrix_t thread_mult(struct matrix_t A, struct matrix_t B) {
    struct matrix_t C = create_empty(A.m, B.n);

    size_t cores = get_nprocs() * 2;
    struct thread_data_t* data = (struct thread_data_t*)malloc(cores * sizeof(struct thread_data_t));

    size_t part_len = A.n / cores + 1;
    size_t current = 0;
    for(size_t i = 0; i < cores; ++i) {
        data[i].A = A;
        data[i].B = B;
        data[i].C = C;

        data[i].m_start = current;
        current += part_len;

        if(current <= A.m) {
            data[i].m_end = current;
        } else {
            data[i].m_end = A.m;
        }

        pthread_create(&data[i].thid, NULL, mult_part, &data[i]);
    }

    for(size_t i = 0; i < cores; i++)
        pthread_join(data[i].thid, NULL);

    return C;
}