#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(void) {
    int fd[2];
    if (pipe(fd) == -1) {
        perror("pipe error");
        exit(1);
    }
    // First child: ls
    switch (fork()) {
        case -1:
            perror("fork error");
            exit(1);
        case 0:
            close(fd[0]);                     // close read end
            if(fd[1] != STDOUT_FILENO){
                if(dup2(fd[1], STDOUT_FILENO) == -1)       // stdout → pipe
                    perror("dup2 error!\n");
                close(fd[1]);
            }
            execlp("ls", "ls", (char *)NULL);
            perror("execlp ls failed");
            _exit(1);
        default:
            break;
    }
    // Second child: wc -l
    switch (fork()) {
        case -1:
            perror("fork error");
            exit(1);
        case 0:
            close(fd[1]);                     // close write end
            if(fd[0] != STDIN_FILENO){
                if(dup2(fd[0], STDIN_FILENO) == -1)        // stdin ← pipe
                    perror("dup2 error!\n");
                close(fd[0]);
            }
            execlp("wc", "wc", "-l", (char *)NULL);
            perror("execlp wc failed");
            _exit(1);
        default:
            break;
    }
    // Parent
    close(fd[0]);
    close(fd[1]);
    wait(NULL);
    wait(NULL);
    return 0;
}