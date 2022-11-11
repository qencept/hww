#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>
#include "matrix.h"
#include "matrix_mul.h"

/*
matrix *matrix_multiply(matrix *a, matrix *b) {
    if (matrix_get_width(a) != matrix_get_height(b)) {
        return NULL;
    }
    matrix *m = matrix_construct(matrix_get_height(a), matrix_get_width(b));
    if (m == NULL) {
        return NULL;
    }
    for (int y = 0; y < matrix_get_height(m); y++) {
        for (int x = 0; x < matrix_get_width(m); x++) {
            int value = 0;
            for (int i = 0; i < matrix_get_width(a); i++) {
                value += matrix_get_at(a, y, i) * matrix_get_at(b, i, x);
            }
            matrix_set_at(m, y, x, value);
        }
    }
    return m;
}
*/

#define NUM_WORKERS 4

struct arg {
    matrix *a;
    matrix *b;
    matrix *m;
    int idx;
};

static void *worker_thread(FAR void *parameter) {
    struct arg *arg = (struct arg *)parameter;
    for (int y = 0; y < matrix_get_height(arg->m); y++) {
        for (int x = 0; x < matrix_get_width(arg->m); x++) {
            if (matrix_hash_at(arg->m, y, x) % NUM_WORKERS == arg->idx) {
                int value = 0;
                for (int i = 0; i < matrix_get_width(arg->a); i++) {
                    value += matrix_get_at(arg->a, y, i) * matrix_get_at(arg->b, i, x);
                }
                matrix_set_at(arg->m, y, x, value);
            }
        }
    }
    return NULL;
}

matrix *matrix_multiply(matrix *a, matrix *b) {
    if (matrix_get_width(a) != matrix_get_height(b)) {
        return NULL;
    }
    matrix *m = matrix_construct(matrix_get_height(a), matrix_get_width(b));
    if (m == NULL) {
        return NULL;
    }

    pthread_t threads[NUM_WORKERS];
    struct arg args[NUM_WORKERS];
    for (int i = 0; i < NUM_WORKERS; i++) {
        args[i] = (struct arg){a, b, m, i};
        int status = pthread_create(&threads[i], NULL, worker_thread, (pthread_addr_t)&args[i]);
        if (status != 0) {
            printf("matrix_multiply: ERROR on pthread_create()\n");
            exit(1);
        }
    }

    for (int i = 0; i < NUM_WORKERS; i++) {
        pthread_join(threads[i], NULL);
    }

    return m;
}

