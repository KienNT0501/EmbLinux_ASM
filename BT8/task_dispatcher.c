#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <mqueue.h>
#include <ctype.h>
#include <string.h>

const char* QUEUE_NAME = "/my_task_queue";

static void usageError(const char *progName)
{
    fprintf(stderr, "Usage: %s <priority (integer)> <task_description>\n", progName);
    exit(EXIT_FAILURE);
}

bool isNumber(const char* str) {
    if (str == NULL || *str == '\0') return false;
    for (size_t i = 0; i < strlen(str); ++i) {
        if (!isdigit((unsigned char)str[i]))
            return false;
    }
    return true;
}

int main(int argc, char *argv[]) {
    if (argc < 3)
        usageError(argv[0]);

    if (!isNumber(argv[1]))
        usageError(argv[0]);

    mqd_t mqd = mq_open(QUEUE_NAME, O_WRONLY);
    if (mqd == (mqd_t)-1) {
        perror("mq_open");
        exit(EXIT_FAILURE);
    }

    unsigned int prio = atoi(argv[1]);
    if (mq_send(mqd, argv[2], strlen(argv[2]) + 1, prio) == -1) {
        perror("mq_send");
        exit(EXIT_FAILURE);
    }

    printf("Sent task: \"%s\" with priority %u\n", argv[2], prio);

    mq_close(mqd);
    return 0;
}
