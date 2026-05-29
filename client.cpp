// client.cpp — TCP Chat Client
#include <iostream>
#include <thread>
#include <string>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

constexpr int PORT = 8080;
constexpr int BUFFER_SIZE = 1024;
bool running = true;

// Receive thread — reads incoming messages from server
void receiveMessages(int sockFd) {
    char buffer[BUFFER_SIZE];
    while (running) {
        std::memset(buffer, 0, BUFFER_SIZE);
        int bytes = recv(sockFd, buffer, BUFFER_SIZE - 1, 0);
        if (bytes <= 0) {
            std::cout << "[client] Disconnected from server.\n";
            running = false;
            break;
        }
        std::cout << "\n" << buffer << "> " << std::flush;
    }
}

int main(int argc, char* argv[]) {
    std::string host = (argc > 1) ? argv[1] : "127.0.0.1";
    std::string name = (argc > 2) ? argv[2] : "user";

    int sockFd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockFd < 0) { perror("socket"); return 1; }

    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port   = htons(PORT);
    inet_pton(AF_INET, host.c_str(), &serverAddr.sin_addr);

    if (connect(sockFd, (sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        perror("connect"); return 1;
    }
    std::cout << "[client] Connected as " << name << ". Type to chat.\n";

    // Spawn receive thread — runs concurrently with send loop
    std::thread recvThread(receiveMessages, sockFd);
    recvThread.detach();

    std::string input;
    while (running && std::getline(std::cin, input)) {
        if (input == "/quit") break;
        std::string msg = name + ": " + input + "\n";
        send(sockFd, msg.c_str(), msg.size(), 0);
    }

    running = false;
    close(sockFd);
    return 0;
}