#ifndef MATRIX_H
#define MATRIX_H

typedef struct matrix matrix;
typedef int (*gen_t)(void);

// caller responsibility to provide non-NULL arg

matrix *matrix_construct(int height, int width);
void matrix_set_at(matrix *m, int y, int x, int value);
int matrix_get_at(matrix *m, int y, int x);
int matrix_get_height(matrix *m);
int matrix_get_width(matrix *m);
const void *matrix_get_buf(matrix *m);
int matrix_get_bufsize(matrix *m);
int matrix_hash_at(matrix *m, int y, int x);
void matrix_destruct(matrix *m);

matrix *matrix_generate(int height, int width, gen_t gen);

#endif
