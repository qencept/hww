#include <stdlib.h>
#include "matrix.h"

struct matrix {
    int width;
    int height;
    int *buf;
};

matrix *matrix_construct(int height, int width) {
    int *buf = malloc(height * width * sizeof(int));
    if (buf == NULL) {
        return NULL;
    }
    matrix *m = malloc(sizeof(matrix));
    if (m == NULL) {
        free(buf);
        return NULL;
    }
    m->width = width;
    m->height = height;
    m->buf = buf;
    return m;
}

void matrix_set_at(matrix *m, int y, int x, int value) {
    m->buf[m->width * y + x] = value;
}


int matrix_get_at(matrix *m, int y, int x) {
    return m->buf[m->width * y + x];
}

int matrix_get_height(matrix *m) {
    return m->height;
}

int matrix_get_width(matrix *m) {
    return m->width;
}

const void *matrix_get_buf(matrix *m) {
    return (void*)m;
}

int matrix_get_bufsize(matrix *m) {
    return (m->width * + m->height) * sizeof(int);
}

int matrix_hash_at(matrix *m, int y, int x) {
    return m->width * y + x;
}

void matrix_destruct(matrix *m) {
    free(m->buf);
    free(m);
}

matrix *matrix_generate(int height, int width, gen_t gen) {
    matrix *m = matrix_construct(height, width);
    if (m == NULL) {
        return NULL;
    }
    for (int y = 0; y < matrix_get_height(m); y++) {
        for (int x = 0; x < matrix_get_width(m); x++) {
            matrix_set_at(m, y, x, gen());
        }
    }
    return m;
}

