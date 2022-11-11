#include <nuttx/config.h>
#include <stdio.h>
#include <mqueue.h>
#include <stdlib.h>
#include <fcntl.h>

#include "matrix.h"
#include "matrix_mul.h"
#include "mq_mul.h"
#include "mq_spi.h"
#include "task_mul.h"

int task_multiplier(int argc, FAR char *argv[]) {
    int mq_mul = mq_open(MQ_MUL_NAME, O_RDONLY | O_CREAT, 0666);
    if (mq_mul < 0) {
        printf("task_multiplier: ERROR on mq_open(%s)\n", MQ_MUL_NAME);
        exit(1);
    }

    int mq_spi = mq_open(MQ_SPI_NAME, O_WRONLY | O_CREAT, 0666);
    if (mq_spi < 0) {
        printf("task_multiplier: ERROR on mq_open(%s)\n", MQ_SPI_NAME);
        exit(1);
    }

    for (;;) {
        struct mul_job mul_job;
        int status = mq_receive(mq_mul, (char *)&mul_job, sizeof(mul_job), 0);
        if (status < 0) {
            printf("task_multiplier: ERROR on mq_receive()\n");
            exit(1);
        }

        matrix *m = matrix_multiply(mul_job.a, mul_job.b);
        matrix_destruct(mul_job.a);
        matrix_destruct(mul_job.b);

        struct spi_job spi_job = {m};
        status = mq_send(mq_spi, (const char *)&spi_job, sizeof(spi_job), 0);
        if (status < 0) {
            printf("task_multiplier: ERROR on mq_send()\n");
            exit(1);
        }
    }
}

