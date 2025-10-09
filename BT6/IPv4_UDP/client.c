#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main() {
    int sockfd;
    struct sockaddr_in servaddr;
    char buffer[1024];

    // Create socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    // Set server info
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(8080);
    inet_pton(AF_INET, "127.0.0.1", &servaddr.sin_addr);

    // Send message
    sendto(sockfd, "Hello from client", 17, 0,
           (struct sockaddr*)&servaddr, sizeof(servaddr));

    // Receive reply
    socklen_t len = sizeof(servaddr);
    int n = recvfrom(sockfd, buffer, sizeof(buffer), 0,
                     (struct sockaddr*)&servaddr, &len);
    buffer[n] = '\0';
    printf("Received: %s\n", buffer);

    close(sockfd);
}
