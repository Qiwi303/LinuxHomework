#include "Client.h"
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <iostream>

Client::Client(const std::string& ip, uint16_t port, const std::string& n)
    : name(n), running(true) {
    fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0) throw std::runtime_error("socket");

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    if (inet_pton(AF_INET, ip.c_str(), &addr.sin_addr) != 1)
        throw std::runtime_error("inet_pton");

    if (connect(fd, (sockaddr*)&addr, sizeof(addr)) < 0)
        throw std::runtime_error("connect");

    sendAll(fd, name + "\n");
}

Client::~Client() {
    close(fd);
}

void Client::run() {
    pthread_create(&recvThread, nullptr, recvEntry, this);

    std::string line;
    while (running && std::getline(std::cin, line)) {
        sendAll(fd, line + "\n");
        if (line == "/exit") break;
    }

    running = false;
    pthread_join(recvThread, nullptr);
}

void* Client::recvEntry(void* arg) {
    static_cast<Client*>(arg)->recvLoop();
    return nullptr;
}

void Client::recvLoop() {
    while (running) {
        std::string msg;
        if (!recvLine(fd, msg)) {
            running = false;
            break;
        }
        std::cout << msg << std::endl;
    }
}

int Client::sendAll(int fd, const std::string& msg) {
    size_t sent = 0;
    while (sent < msg.size()) {
        ssize_t n = send(fd, msg.data() + sent, msg.size() - sent, 0);
        if (n <= 0) return -1;
        sent += n;
    }
    return 0;
}

bool Client::recvLine(int fd, std::string& out) {
    out.clear();
    char c;
    while (true) {
        ssize_t n = recv(fd, &c, 1, 0);
        if (n <= 0) return false;
        if (c == '\n') break;
        out.push_back(c);
    }
    return true;
}
