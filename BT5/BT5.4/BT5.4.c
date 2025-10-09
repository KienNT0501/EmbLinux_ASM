#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <sys/select.h>
#include <errno.h>

volatile sig_atomic_t sigint_flag = 0;
volatile sig_atomic_t sigterm_flag = 0;

void handle_signal(int sig) {
    if (sig == SIGINT) {
        sigint_flag = 1;
    } else if (sig == SIGTERM) {
        sigterm_flag = 1;
    }
}

int main(void) {
    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = handle_signal;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART; // xin kernel tự động restart system calls nếu có thể

    if (sigaction(SIGINT, &sa, NULL) == -1) {
        perror("sigaction(SIGINT)");
        return 1;
    }
    if (sigaction(SIGTERM, &sa, NULL) == -1) {
        perror("sigaction(SIGTERM)");
        return 1;
    }

    printf("Program started. Type something or press Ctrl+C. PID: %d\n", getpid());

    for (;;) {
        if (sigint_flag) {
            printf("SIGINT received.\n");
            fflush(stdout);
            sigint_flag = 0;
        }
        if (sigterm_flag) {
            printf("SIGTERM received. Exiting...\n");
            fflush(stdout);
            break;
        }

        fd_set readfds;
        FD_ZERO(&readfds);
        FD_SET(STDIN_FILENO, &readfds);

        struct timeval timeout;
        timeout.tv_sec = 1;
        timeout.tv_usec = 0;

        int ret = select(STDIN_FILENO + 1, &readfds, NULL, NULL, &timeout);

        if (ret == -1) {
            if (errno == EINTR) {
                // Bị ngắt bởi tín hiệu — vòng lại để kiểm tra cờ tín hiệu
                continue;
            } else {
                perror("select()");
                exit(EXIT_FAILURE);
            }
        } else if (ret > 0) {
            if (FD_ISSET(STDIN_FILENO, &readfds)) {
                char buffer[256];
                if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
                    buffer[strcspn(buffer, "\n")] = '\0';
                    printf("You typed: %s\n", buffer);
                    fflush(stdout);
                }
            }
        }
        // ret == 0 -> timeout, tiếp tục vòng lặp để kiểm tra tín hiệu
    }

    return 0;
}
