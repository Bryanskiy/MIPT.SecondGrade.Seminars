#pragma once

struct matrix_t naive_mult(struct matrix_t A, struct matrix_t B) {
    struct matrix_t C = create_empty(A.m, B.n);

    for(size_t i = 0; i < A.m; ++i) {
        for(size_t j = 0; j < B.n; ++j) {
            double elem = 0.0;
            for(size_t k = 0; k < A.n; ++k) {
                elem += A.elem[i][k] * B.elem[k][j];
            }

            C.elem[i][j] = elem;
        }
    }

    return C;
}