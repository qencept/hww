#ifndef MQ_MUL_H
#define MQ_MUL_H

#include "matrix.h"

#define MQ_MUL_NAME "mq_mul"

struct mul_job {
    matrix *a;
    matrix *b;
};

#endif
