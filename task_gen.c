#include <nuttx/config.h>
#include <stdio.h>
#include <mqueue.h>
#include <stdlib.h>
#include <fcntl.h>

#include "matrix.h"
#include "mq_mul.h"
#include "task_gen.h"

#define SLEEP_SEC 1

#define MINSIZE	 1
#define MAXSIZE  10
#define MINVALUE 0
#define MAXVALUE 5 

static int gen_size(void) {
    return rand() % (MAXSIZE - MINSIZE) + MINSIZE;
}

static int gen_value(void) {
    return rand() % (MAXVALUE - MINVALUE) + MINVALUE;
}

int task_generator(int argc, FAR char *argv[]) {
    int mq_mul = mq_open(MQ_MUL_NAME, O_WRONLY | O_CREAT, 0666);
    if (mq_mul < 0) {
        printf("task_generator: ERROR on mq_open()\n");
        exit(1);
    }

    for (;;) {
        int height = gen_size();
        int common = gen_size();
        int width = gen_size();

        matrix *a = matrix_generate(height, common, gen_value);
        if (a == NULL) {
            printf("task_generator: ERROR on matrix_generate()\n");
            exit(1);
        }

        matrix *b = matrix_generate(common, width, gen_value);
        if (b == NULL) {
            printf("task_generator: ERROR on matrix_generate()\n");
            exit(1);
        }

        struct mul_job mul_job = {a, b};
        int status = mq_send(mq_mul, (const char *)&mul_job, sizeof(mul_job), 0);
        if (status < 0) {
            printf("task_generator: ERROR on mq_send()\n");
            exit(1);
        }

        sleep(SLEEP_SEC);
    }
}

