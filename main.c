#include <nuttx/config.h>
#include <stdio.h>
#include <stdlib.h>

#include "task_gen.h"
#include "task_mul.h"
#include "task_spi.h"

int main(int argc, FAR char *argv[])
{
    int ret;
    ret = task_create("Task generator",
                      CONFIG_EXAMPLES_HWW_PRIORITY_GEN,
                      CONFIG_EXAMPLES_HWW_STACKSIZE,
                      task_generator,
                      NULL);
    if (ret < 0) {
        printf("main: ERROR on task_create(GEN)\n");
        exit(1);
    }

    ret = task_create("Task multiplier",
                      CONFIG_EXAMPLES_HWW_PRIORITY_MUL,
                      CONFIG_EXAMPLES_HWW_STACKSIZE,
                      task_multiplier,
                      NULL);
    if (ret < 0) {
        printf("main: ERROR on task_create(MUL)\n");
        exit(1);
    }

    ret = task_create("Task SPI",
                      CONFIG_EXAMPLES_HWW_PRIORITY_SPI,
                      CONFIG_EXAMPLES_HWW_STACKSIZE,
                      task_spi,
                      NULL);
    if (ret < 0) {
        printf("main: ERROR on task_create(SPI)\n");
        exit(1);
    }

    return 0;
}

