#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include <pthread.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <signal.h>
using namespace std;

// Structure to store information about each peer connection
struct Connection {
    int sock;               // Socket descriptor
    std::string ip;         // Peer IP address
    int port;               // Peer port
    long unsigned int id;   // Unique connection ID
};

// Global variables
std::vector<Connection> connections;  // List of active connections
int listenerSocket;                   // Listening socket for incoming connections
bool running = true;                  // Main loop control flag
static pthread_mutex_t conn_mutex = PTHREAD_MUTEX_INITIALIZER;  // Mutex for thread safety

// Display help menu
void printMenu(){
    std::cout<<"************* Chat Application ************\n";
    std::cout<<"******\n";
    std::cout<<"Use the commands below: \n";
    std::cout << "1. help - Show this help message\n";
    std::cout << "2. myip - Display your IP address\n";
    std::cout << "3. myport - Display your listening port\n";
    std::cout << "4. connect <ip> <port> - Connect to a peer\n";
    std::cout << "5. list - Show all active connections\n";
    std::cout << "6. send <id> <message> - Send a message to a peer\n";
    std::cout << "7. terminate <id> - Close a specific connection\n";
    std::cout << "8. exit - Close all connections and exit\n";
}

// Add a new connection to global list
void addConnection(int sock, std::string ip, int port) {
    pthread_mutex_lock(&conn_mutex);
    connections.push_back({sock, ip, port, connections.size()+1});
    pthread_mutex_unlock(&conn_mutex);
}

// Check if already connected to a specific (IP, port)
int has_connection_to(std::string ip, int port) {
    int found = 0;
    pthread_mutex_lock(&conn_mutex);
    for(size_t i = 0; i < connections.size(); i++){
        if(connections[i].ip == ip && connections[i].port == port){
            found = 1;
            break;
        }
    }
    pthread_mutex_unlock(&conn_mutex);
    return found;
}

// Remove a connection by socket descriptor
void removeConnection(int sock) {
    for (auto it = connections.begin(); it != connections.end(); ++it) {
        if (it->sock == sock) {
            connections.erase(it);
            break;
        }
    }
}

// Get local machine IP address
std::string getLocalIP() {
    char hostname[256];
    gethostname(hostname, sizeof(hostname));            // Get host name
    struct hostent *host = gethostbyname(hostname);     // Resolve hostname to IP
    return inet_ntoa(*((struct in_addr *)host->h_addr_list[0]));
}

// Create a TCP listener socket
int createListenerSocket(int port) {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("[ERROR] socket() failed");
        return -1;
    }

    // Optional: allow quick reuse after restart (but doesn’t bypass conflicts)
    int opt = 1;
    setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(port);

    if (bind(sock, (sockaddr*)&addr, sizeof(addr)) < 0) {
        perror("[ERROR] bind() failed");
        std::cerr << "[ERROR] Port " << port << " may already be in use.\n";
        close(sock);
        return -1;
    }

    if (listen(sock, 10) < 0) {
        perror("[ERROR] listen() failed");
        close(sock);
        return -1;
    }

    return sock;
}

// Thread function to receive messages from a connected peer
void* receiverThreadFunc(void* socketDesc) {
    int sock = (intptr_t)socketDesc;

    // Find IP and port of this peer
    std::string sip;
    int sport = 0;
    for(size_t i = 0; i < connections.size(); i++){
        if(sock == connections[i].sock){
            sip = connections[i].ip;
            sport = connections[i].port;
        }
    }

    // Buffer for receiving messages
    char buffer[200];
    while (running) {
        memset(buffer, 0, sizeof(buffer));
        int bytes = recv(sock, buffer, sizeof(buffer)-1, 0); // Receive data
        if (bytes <= 0) break;                               // If connection closed or error
        std::cout << "\n[INFO] [Message received from "<<sip<<" : "<<sport<<"]: " << buffer << std::endl;
    }

    // Cleanup when peer disconnects
    close(sock);
    removeConnection(sock);
    return NULL;
}

// Thread that listens for new incoming connections
void* listenerThreadFunc(void*) {
    while (running) {
        struct sockaddr_in peerAddr;
        socklen_t len2 = sizeof(peerAddr);
        int newSock = accept(listenerSocket, (sockaddr*)&peerAddr, &len2);  // Accept new connection
        if (newSock < 0) continue;

        // Get local socket info
        struct sockaddr_in localAddr;
        socklen_t len1 = sizeof(localAddr);
        getsockname(newSock, (struct sockaddr*)&localAddr, &len1);
        int localPort = ntohs(localAddr.sin_port);

        std::string ip = inet_ntoa(peerAddr.sin_addr);
        int port = ntohs(peerAddr.sin_port);

        // Prevent duplicate connection
        if(has_connection_to(ip,port)){
            close(newSock);
            continue;
        }

        // Add new connection and start receiver thread
        addConnection(newSock, ip, port);
        pthread_t recvThread;
        pthread_create(&recvThread, NULL, receiverThreadFunc, (void*)(intptr_t)newSock);

        std::cout << "\n[INFO] Connection: " << ip << ":" << port
                  << " → " << inet_ntoa(localAddr.sin_addr) << ":" << localPort << "\n";
        printf("> "); fflush(stdout);
    }
    return NULL;
}

// Connect to another peer
void connectToPeer(std::string ip, int port) {
    if (has_connection_to(ip, port)) {
        cout << "[ERROR] Connection already exists\n";
        return;
    }

    int sock = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    inet_pton(AF_INET, ip.c_str(), &addr.sin_addr);

    // Try to connect
    if (connect(sock, (sockaddr*)&addr, sizeof(addr)) == 0) {
        addConnection(sock, ip, port);
        pthread_t recvThread;
        pthread_create(&recvThread, NULL, receiverThreadFunc, (void*)(intptr_t)sock);
        std::cout << "[INFO] Successfully connected to " << ip << ": " << port << "!\n";
    } else {
        std::cerr << "[ERROR] Connection failed\n";
        close(sock);
    }
}

// Send a message to a peer by connection ID
void sendMessage(long unsigned int id, std::string message) {
    if(message.size() > 200){
        perror("[ERROR] Message too long!\n");
    }
    for (auto &c : connections) {
        if (c.id == id) {
            send(c.sock, message.c_str(), message.size(), 0);  // Send data to peer
            return;
        }
    }
    std::cout << "[ERROR] Invalid connection ID\n";
}

// Print the list of all active connections
void listConnections() {
    std::cout << "ID\tIP\t\tPort\n";
    for (auto &c : connections)
        std::cout << c.id << "\t" << c.ip << "\t" << c.port << "\n";
}

// Close one specific connection by ID
void terminateConnection(long unsigned int id) {
    for (auto it = connections.begin(); it != connections.end(); ++it) {
        if (it->id == id) {
            close(it->sock);
            connections.erase(it);
            std::cout << "[INFO] Connection terminated (ID "<<id<<").\n";
            return;
        }
    }
    std::cout << "[ERROR] Invalid connection ID\n";
}

// Clean exit: close all connections and sockets
void exitProgram() {
    running = false;
    for (auto &c : connections)
        close(c.sock);
    connections.clear();
    close(listenerSocket);
    std::cout << "[INFO] All connections closed. Exiting...\n";
}

// Handle Ctrl+C gracefully
void sigint_handler(int sig) {
    (void)sig;
    printf("\n[INFO] SIGINT received. Use 'exit' to quit cleanly.\n> ");
    fflush(stdout);
}

// ----------------------------- MAIN FUNCTION -----------------------------
int main(int argc, char* argv[]) {
    signal(SIGINT, sigint_handler);

    // Require one argument: listening port
    if (argc != 2) {
        std::cerr << "[INFO] Usage: ./chat <listening_port>\n";
        return 1;
    }

    int myPort = std::stoi(argv[1]);

    listenerSocket = createListenerSocket(myPort);
    if (listenerSocket < 0) {
        std::cerr << "[ERROR] Unable to start listener on port " << myPort << ".\n";
        std::cerr << "[INFO] Try a different port.\n";
        return 1;
    }

    std::cout << "[INFO] P2P Chat started.\n";
    std::cout << "[INFO] Listening on port: " << myPort << "\n";
    std::cout << "[INFO] Type 'help' for commands.\n";

    // Start thread that waits for incoming connections
    pthread_t listenerThread;
    pthread_create(&listenerThread, NULL, listenerThreadFunc, NULL);

    // Command input loop
    std::string cmd;
    while (running) {
        std::cout << "> ";
        std::getline(std::cin, cmd);

        if (cmd == "help") {
            printMenu();
        }
        else if (cmd == "myip") {
            std::cout << "[INFO] Your IP: " << getLocalIP() << "\n";
        }
        else if (cmd == "myport") {
            std::cout << "[INFO] Listening Port: " << myPort << "\n";
        }
        else if (cmd.rfind("connect", 0) == 0) {
            char ipBuf[100];
            int port;
            if (sscanf(cmd.c_str(), "connect %99s %d", ipBuf, &port) == 2) {
                if(port == myPort) {
                    cout << "[ERROR] Cannot connect to itself!\n";
                } else {
                    connectToPeer(std::string(ipBuf), port);
                }
            } else {
                std::cout << "[INFO] Usage: connect <ip> <port>\n";
            }
        }
        else if (cmd == "list") {
            listConnections();
        }
        else if (cmd.rfind("send", 0) == 0) {
            int id;
            char msg[200];
            sscanf(cmd.c_str(), "send %d %[^\n]", &id, msg);
            sendMessage(id, msg);
        }
        else if (cmd.rfind("terminate", 0) == 0) {
            int id;
            sscanf(cmd.c_str(), "terminate %d", &id);
            terminateConnection(id);
        }
        else if (cmd == "exit") {
            exitProgram();
            sleep(1);
            return 0;
        }
        else if (!cmd.empty()) {
            std::cout << "[ERROR] Unknown command. Type 'help' for list.\n";
        }
    }

    pthread_join(listenerThread, NULL);
    return 0;
}
