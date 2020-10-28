#include <stdio.h>
#include <stdlib.h>
#include <time.h>

struct matrix_t {
    size_t m, n;
    double** elem;
};

struct matrix_t create_empty(size_t m, size_t n) {
    struct matrix_t ret = {m, n, NULL};

    ret.elem = (double**)malloc(ret.m * sizeof(double*));
    if(NULL == ret.elem) {
        struct matrix_t invalid = {0u, 0u, NULL};
        return invalid;
    }

    for(size_t i = 0; i < ret.m; ++i) {
        ret.elem[i] = (double*)malloc(ret.n * sizeof(double));
        if(NULL == ret.elem[i]) {
            struct matrix_t invalid = {0u, 0u, NULL};
            return invalid;
        }
    }

    return ret;
}

#include "naive_mult.h"
#include "thread_mult.h"

struct matrix_t read_matrix(FILE* istream) {
    size_t m, n;

    if(NULL == istream) {
        struct matrix_t invalid = {0u, 0u, NULL};
        return invalid;
    }

    int tmp = scanf("%lu %lu", &m, &n);
    if(2 != tmp) {
        fprintf(stderr, "Wrong matrix size imput\n");
        exit(1);
    }

    struct matrix_t ret = create_empty(m, n);

    for(size_t i = 0; i < ret.m; ++i) {
        for(size_t j = 0; j < ret.n; ++j) {
            tmp = scanf("%lf", &(ret.elem[i][j]));
            if(1 != tmp) {
                struct matrix_t invalid = {0u, 0u, NULL};
                return invalid;
            }
        }
    }

    return ret;
}

void print_matrix(struct matrix_t matrix) {
    for(size_t i = 0; i < matrix.m; ++i) {
        for(size_t j = 0; j < matrix.n; ++j) {
            printf("%lf ", matrix.elem[i][j]);
        }
        printf("\n");
    }
}

void delete_matrix(struct matrix_t matrix) {
    if(NULL == matrix.elem) {
        return;
    }

    for(size_t i = 0; i < matrix.m; ++i) {
        if(NULL != matrix.elem[i]) {
            free(matrix.elem[i]);
        }
    }

    free(matrix.elem);
}

int main() {
    struct matrix_t A = read_matrix(stdin);
    struct matrix_t B = read_matrix(stdin);

    clock_t thread_time = clock();
    struct matrix_t C = thread_mult(A, B);
    thread_time = clock() - thread_time;
    printf("Thread multiplication in %f mls\n", ((double)thread_time)/ CLOCKS_PER_SEC * 1000.f);

    clock_t naive_time = clock();
    struct matrix_t D = naive_mult(A, B);
    naive_time = clock() - naive_time;
    printf("Naive multiplication in %f mls\n", ((double)naive_time)/ CLOCKS_PER_SEC * 1000.f);

    delete_matrix(A);
    delete_matrix(B);
    delete_matrix(C);
}