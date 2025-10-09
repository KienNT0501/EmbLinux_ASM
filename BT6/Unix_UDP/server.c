#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>

#define SERVER_PATH "/tmp/unix_dgram_server"

int main() {
    int sockfd;
    struct sockaddr_un server_addr, client_addr;
    char buffer[128];
    socklen_t len = sizeof(client_addr);

    unlink(SERVER_PATH); // remove old socket file

    // Create datagram socket
    sockfd = socket(AF_UNIX, SOCK_DGRAM, 0);
    if (sockfd == -1) {
        perror("socket");
        exit(1);
    }

    // Bind to a local path
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sun_family = AF_UNIX;
    strcpy(server_addr.sun_path, SERVER_PATH);
    if (bind(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("bind");
        exit(1);
    }

    printf("UNIX_DGRAM server waiting for messages...\n");

    // Receive message
    int n = recvfrom(sockfd, buffer, sizeof(buffer), 0,
                     (struct sockaddr*)&client_addr, &len);
    buffer[n] = '\0';
    printf("Received: %s\n", buffer);

    // Send response
    sendto(sockfd, "Hello from server", 17, 0,
           (struct sockaddr*)&client_addr, len);

    close(sockfd);
    unlink(SERVER_PATH);
    return 0;
}
