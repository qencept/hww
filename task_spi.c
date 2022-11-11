#include <nuttx/config.h>
#include <stdio.h>
#include <mqueue.h>
#include <stdlib.h>
#include <fcntl.h>

#include "matrix.h"
#include "mq_spi.h"
#include "task_spi.h"

#ifdef CONFIG_ARCH_BOARD_XXX
    #include <nuttx/spi/spi.h>
    #include <arch/board/board.h>
#endif

int task_spi(int argc, FAR char *argv[]) {
    int mq_spi = mq_open(MQ_SPI_NAME, O_RDONLY | O_CREAT, 0666);
    if (mq_spi < 0) {
        printf("task_multiplier: ERROR on mq_open(%s)\n", MQ_SPI_NAME);
        exit(1);
    }

#ifdef CONFIG_ARCH_BOARD_XXX
    struct spi_dev_s *spi_dev = spibus_initialize();
    SPI_SETFREQUENCY(spi_dev, CONFIG_XXX_SPI_FREQ);
    SPI_SETBITS(spi_dev, CONFIG_XXX_SPI_BITS);
    SPI_SETMODE(spi_dev, CONFIG_XXX_SPI_MODE);
#endif

    for (;;) {
        struct spi_job spi_job;
        int status = mq_receive(mq_spi, (char *)&spi_job, sizeof(spi_job), 0);
        if (status < 0) {
            printf("task_spi: ERROR on mq_receive()\n");
            exit(1);
        }

#ifdef CONFIG_ARCH_BOARD_XXX
        uint32_t header[] = {matrix_get_height(spi_job.m), matrix_get_width(spi_job.m)};
        SPI_EXCHANGE(spi_dev, (const void*)&header, NULL, sizeof(header));
        SPI_EXCHANGE(spi_dev, matrix_get_buf(spi_job.m), NULL, matrix_get_bufsize(spi_job.m));
#endif

        matrix_destruct(spi_job.m);
    }
}

