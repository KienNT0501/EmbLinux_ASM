#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
int main(){
    pid_t pid = fork();
    int status;
    if(pid < 0){
        printf("Error when create process\n");
    } else if (pid == 0){
        printf("Child Process create by fork, PID: %d\n", getpid());
        exit(36);
    } else if(pid > 1){
        printf("Parent Proccess, PID: %d, Child's PID: %d\n", getpid(), pid);
        wait(&status);
        if(WIFEXITED(status)){
            printf("Child terminated normally, exit code: %d\n", WEXITSTATUS(status));
        } else {
            printf("Child not terminated normally, exit code: %d\n", WEXITSTATUS(status));
        }
    }
    return 0;
}