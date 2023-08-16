#include <fcntl.h>
#include <mqueue.h>
#include <nuttx/config.h>
#include <nuttx/sched.h>
#include <stdio.h>
#include <unistd.h>

/*
 * my_app.c:
 * demonstrate how to create NuttX tasks and using mqueue for
 * inter-process communication (IPC).
 */

#define STACK_SIZE 1024
#define PRIORITY 101

#define MQ_MAX_MSGS 10
#define MQ_MSG_SIZE 16

static mqd_t msg_queue;

static int receiver_task(int argc, char *argv[])
{
    struct mq_attr mq_attr;
    mq_getattr(msg_queue, &mq_attr);

    while (1) {
        char msg[MQ_MSG_SIZE];
        ssize_t bytes_read = mq_receive(msg_queue, msg, MQ_MSG_SIZE, NULL);
        if (bytes_read > 0) {
            printf("Received message: %s\n", msg);
        }
    }

    return 0;
}

static int sender_task(int argc, char *argv[])
{
    const char *message = "Hello, world!";

    while (1) {
        mq_send(msg_queue, message, strlen(message) + 1, 0);
        usleep(2000000);  // Sleep for 1 seconds
    }

    return 0;
}

int main(int argc, char *argv[])
{
    /* Create  message queue */
    struct mq_attr mq_attr = {.mq_maxmsg = MQ_MAX_MSGS,
                              .mq_msgsize = MQ_MSG_SIZE,
                              .mq_flags = 0,
                              .mq_curmsgs = 0};
    msg_queue = mq_open("/my_msg_queue", O_RDWR | O_CREAT, 0666, &mq_attr);
    if (msg_queue == (mqd_t) -1) {
        printf("Failed to create message queue\n");
        return 1;
    }

    /* Create receiver task */
    int receiver_id =
        task_create("receiver", PRIORITY, STACK_SIZE, receiver_task, NULL);
    if (receiver_id < 0) {
        printf("Failed to create receiver task: %d\n", receiver_id);
        return 1;
    }

    /* Create sender task */
    int sender_id =
        task_create("sender", PRIORITY, STACK_SIZE, sender_task, NULL);
    if (sender_id < 0) {
        printf("Failed to create sender task: %d\n", sender_id);
        return 1;
    }

    return 0;
}
