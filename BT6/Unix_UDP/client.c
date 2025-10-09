#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>

#define SERVER_PATH "/tmp/unix_dgram_server"
#define CLIENT_PATH "/tmp/unix_dgram_client"

int main() {
    int sockfd;
    struct sockaddr_un server_addr, client_addr;
    char buffer[128];
    socklen_t len;

    unlink(CLIENT_PATH);

    // Create datagram socket
    sockfd = socket(AF_UNIX, SOCK_DGRAM, 0);
    if (sockfd == -1) {
        perror("socket");
        exit(1);
    }

    // Bind client to its own path
    memset(&client_addr, 0, sizeof(client_addr));
    client_addr.sun_family = AF_UNIX;
    strcpy(client_addr.sun_path, CLIENT_PATH);
    if (bind(sockfd, (struct sockaddr*)&client_addr, sizeof(client_addr)) == -1) {
        perror("bind");
        exit(1);
    }

    // Set server address
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sun_family = AF_UNIX;
    strcpy(server_addr.sun_path, SERVER_PATH);

    // Send message
    sendto(sockfd, "Hello from client", 17, 0,
           (struct sockaddr*)&server_addr, sizeof(server_addr));

    // Receive reply
    len = sizeof(server_addr);
    int n = recvfrom(sockfd, buffer, sizeof(buffer), 0,
                     (struct sockaddr*)&server_addr, &len);
    buffer[n] = '\0';
    printf("Received: %s\n", buffer);

    close(sockfd);
    unlink(CLIENT_PATH);
    return 0;
}
