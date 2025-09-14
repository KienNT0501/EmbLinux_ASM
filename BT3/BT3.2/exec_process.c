#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
int main(){
    pid_t pid = fork();
    const char* MY_COMMAND = "ls";
    const char* MY_OPTION = "-l";
    if(pid<0) {
        printf("Fork return error\n");
    } else if(pid == 0){
        printf("Before exec, Child Process PID: %d\n", getpid());
        execlp(MY_COMMAND, MY_COMMAND, MY_OPTION, NULL);
        printf("After exec, Child Process PID: %d\n", getpid());
        perror("execlp failed");
        _exit(1);
    } else if(pid > 1){
        printf("Parrent Process PID: %d, Child Process PID: %d\n", getpid(), pid);
        wait(NULL);
    }
    return 0;
}