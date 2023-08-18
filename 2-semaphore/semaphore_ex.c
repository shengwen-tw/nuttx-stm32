#include <fcntl.h>
#include <nuttx/config.h>
#include <nuttx/sched.h>
#include <nuttx/semaphore.h>
#include <stdio.h>
#include <unistd.h>

#define STACK_SIZE 1024
#define PRIORITY 101

static sem_t g_semaphore;

static int receiver_task(int argc, char *argv[])
{
    while (1) {
        sem_wait(&g_semaphore);
        printf("Semaphore acquired.\n");
    }

    return 0;
}

static int sender_task(int argc, char *argv[])
{
    while (1) {
        sem_post(&g_semaphore);
        printf("Semaphore sent.\n");
        usleep(1000000);  // Sleep for 1 seconds
    }

    return 0;
}

int main(int argc, char *argv[])
{
    /* Initialize the semaphore */
    sem_init(&g_semaphore, 0, 0);

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
