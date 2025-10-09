#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <signal.h>
void signal_handler (){
    printf("Received signal from parent!\n");
}
int main(){
    pid_t proc = fork();
    if (proc == 0){
        signal (SIGUSR1, signal_handler);
        while (1) pause();
    }
    else if (proc<0){
        perror("Can not create child process\n");
        exit(EXIT_FAILURE);
    }
    else {
        for(int i = 0; i<5;i++){
            sleep(2);
            printf("Sending message! Parent ID: %d, child ID: %d\n", getpid(), proc);
            if (kill(proc, SIGUSR1) == -1) {
                perror("kill() error!\n");
                exit(EXIT_FAILURE);
            }
        }
    }
    return 0;
}