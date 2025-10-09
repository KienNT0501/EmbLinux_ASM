#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main() {
    int sockfd;
    struct sockaddr_in servaddr, cliaddr;
    char buffer[1024];
    socklen_t len;

    // 1️⃣ Create socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    // 2️⃣ Bind to IP/Port
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(8080);
    bind(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr));

    printf("UDP Server listening on port 8080...\n");

    // 3️⃣ Receive data
    len = sizeof(cliaddr);
    int n = recvfrom(sockfd, buffer, sizeof(buffer), 0,
                     (struct sockaddr*)&cliaddr, &len);
    buffer[n] = '\0';
    printf("Received: %s\n", buffer);

    // 4️⃣ Send response
    sendto(sockfd, "Hello from server", 17, 0,
           (struct sockaddr*)&cliaddr, len);

    close(sockfd);
}
