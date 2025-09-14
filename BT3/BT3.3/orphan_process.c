#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
int main(){
    pid_t pid = fork();
     if (pid < 0) {
        perror("fork failed");
        exit(1);
    }
    else if (pid == 0) {
        printf("\nChild PID=%d, Parent PID=%d\n", getpid(), getppid());
        printf("Child process sleeping for 5 s\n");
        for (int i = 0; i < 10; i++) {
            printf("\nChild PID=%d, Parent PID=%d\n", getpid(), getppid());
            fflush(stdout);
            sleep(1);
        }
        exit(0);
    }
    else {
        printf("Parent exiting.\n");
        exit(0);
    }
    return 0;
}