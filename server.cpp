// server.cpp — Multi-Client TCP Chat Server
#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <string>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <algorithm>

constexpr int PORT = 8080;
constexpr int BUFFER_SIZE = 1024;
constexpr int MAX_CLIENTS = 10;

std::vector<int> clientFds;
std::mutex clientMutex;

// Broadcast message to all clients except sender
void broadcast(const std::string& msg, int senderFd) {
    std::lock_guard<std::mutex> lock(clientMutex);
    for (int fd : clientFds) {
        if (fd != senderFd) {
            send(fd, msg.c_str(), msg.size(), 0);
        }
    }
}

// Thread function — handles one client
void handleClient(int clientFd) {
    char buffer[BUFFER_SIZE];
    while (true) {
        std::memset(buffer, 0, BUFFER_SIZE);
        int bytesRecv = recv(clientFd, buffer, BUFFER_SIZE - 1, 0);

        if (bytesRecv <= 0) {        // client disconnected
            std::cout << "[server] Client fd=" << clientFd << " disconnected\n";
            std::lock_guard<std::mutex> lock(clientMutex);
            clientFds.erase(
                std::remove(clientFds.begin(), clientFds.end(), clientFd),
                clientFds.end()
            );
            close(clientFd);
            return;
        }

        std::string msg(buffer, bytesRecv);
        std::cout << "[fd=" << clientFd << "] " << msg;
        broadcast(msg, clientFd);
    }
}

int main() {
    int serverFd = socket(AF_INET, SOCK_STREAM, 0);
    if (serverFd < 0) { perror("socket"); return 1; }

    int opt = 1;
    setsockopt(serverFd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    sockaddr_in addr{};
    addr.sin_family      = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port        = htons(PORT);

    if (bind(serverFd, (sockaddr*)&addr, sizeof(addr)) < 0) {
        perror("bind"); return 1;
    }
    listen(serverFd, MAX_CLIENTS);
    std::cout << "[server] Listening on port " << PORT << "\n";

    while (true) {
        sockaddr_in clientAddr{};
        socklen_t addrLen = sizeof(clientAddr);
        int clientFd = accept(serverFd, (sockaddr*)&clientAddr, &addrLen);
        if (clientFd < 0) { perror("accept"); continue; }

        std::cout << "[server] New client fd=" << clientFd << "\n";
        {
            std::lock_guard<std::mutex> lock(clientMutex);
            clientFds.push_back(clientFd);
        }
        std::thread(handleClient, clientFd).detach();
    }

    close(serverFd);
    return 0;
}