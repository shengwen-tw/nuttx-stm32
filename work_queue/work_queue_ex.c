#include <fcntl.h>
#include <nuttx/config.h>
#include <nuttx/sched.h>
#include <nuttx/wqueue.h>
#include <stdio.h>
#include <unistd.h>

#define WORK_QUEUE_PRIORITY 100
#define WORK_QUEUE_STACKSIZE 2048

static struct work_s worker;

static void worker_function(int argc, char *argv[])
{
    printf("Hello, World!\n");
}

int main(int argc, char *argv[])
{
    /* Initialize the work queue */
    int ret = work_queue(HPWORK, &worker, (worker_t)worker_function, NULL, 0);
    if (ret != OK) {
        printf("Failed to queue work: %d\n", ret);
        return ret;
    }

    return 0;
}
