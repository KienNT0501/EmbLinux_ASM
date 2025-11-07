#include <stdio.h>
#include <stdlib.h>
#include <mqueue.h>
#include <fcntl.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>

const char* QUEUE_NAME = "/my_task_queue";
mqd_t mqd;
void signal_handler(int sig) {
    printf("\nExiting...\n");
    mq_unlink(QUEUE_NAME);
    mq_close (mqd);
    exit(0);
}

int main(int argc, char *argv[]) {
    
    struct mq_attr attr;

    // Set message queue attributes
    attr.mq_flags = 0;
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = 256;
    attr.mq_curmsgs = 0;

    // Open (and create if needed) the queue
    mqd = mq_open(QUEUE_NAME, O_CREAT | O_RDONLY, 0666, &attr);
    if (mqd == (mqd_t)-1) {
        perror("mq_open");
        exit(EXIT_FAILURE);
    }

    printf("Task worker started. Waiting for tasks...\n");
    signal(SIGINT, signal_handler); // set once outside loop

    char buf[256];
    unsigned int prio;

    while (1) {
        ssize_t bytes_read = mq_receive(mqd, buf, sizeof(buf), &prio);
        if (bytes_read >= 0) {
            buf[bytes_read] = '\0'; // null terminate message
            printf("Processing Task (Priority: %u): %s\n", prio, buf);
            sleep(1);
        } else {
            perror("mq_receive");
        }
    }

    mq_close(mqd);
    return 0;
}
