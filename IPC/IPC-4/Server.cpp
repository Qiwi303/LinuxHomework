#include "Server.h"
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <cstring>
#include <cerrno>
#include <iostream>

Server::Server(uint16_t p) : port(p) {
    listenFd = socket(AF_INET, SOCK_STREAM, 0);
    if (listenFd < 0) throw std::runtime_error("socket");

    int opt = 1;
    if (setsockopt(listenFd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
        throw std::runtime_error("setsockopt");

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(port);

    if (bind(listenFd, (sockaddr*)&addr, sizeof(addr)) < 0)
        throw std::runtime_error("bind");

    if (listen(listenFd, 16) < 0)
        throw std::runtime_error("listen");

    pthread_mutex_init(&clientsMutex, nullptr);
}

Server::~Server() {
    close(listenFd);
    pthread_mutex_destroy(&clientsMutex);
}

void Server::run() {
    while (true) {
        int fd = accept(listenFd, nullptr, nullptr);
        if (fd < 0) continue;

        auto* client = new Client{fd, "", {}};

        pthread_mutex_lock(&clientsMutex);
        clients.push_back(client);
        pthread_mutex_unlock(&clientsMutex);

        if (pthread_create(&client->thread, nullptr, clientEntry, this) != 0) {
            close(fd);
            removeClient(fd);
        }
    }
}

void* Server::clientEntry(void* arg) {
    auto* server = static_cast<Server*>(arg);
    Client* client = nullptr;

    pthread_mutex_lock(&server->clientsMutex);
    client = server->clients.back();
    pthread_mutex_unlock(&server->clientsMutex);

    server->handleClient(client);
    return nullptr;
}

void Server::handleClient(Client* client) {
    std::string name;
    if (!recvLine(client->fd, name)) {
        removeClient(client->fd);
        close(client->fd);
        return;
    }

    client->name = name;
    broadcast("* " + name + " joined");

    while (true) {
        std::string msg;
        if (!recvLine(client->fd, msg)) break;

        if (msg == "/exit") break;

        if (msg == "/list") {
            sendAll(client->fd, listUsers());
            continue;
        }

        broadcast("[" + client->name + "] " + msg);
    }

    broadcast("* " + client->name + " left");
    removeClient(client->fd);
    close(client->fd);
}

void Server::broadcast(const std::string& msg, int exceptFd) {
    pthread_mutex_lock(&clientsMutex);
    for (auto* c : clients) {
        if (c->fd != exceptFd)
            sendAll(c->fd, msg + "\n");
    }
    pthread_mutex_unlock(&clientsMutex);
}

void Server::removeClient(int fd) {
    pthread_mutex_lock(&clientsMutex);
    for (auto it = clients.begin(); it != clients.end(); ++it) {
        if ((*it)->fd == fd) {
            delete *it;
            clients.erase(it);
            break;
        }
    }
    pthread_mutex_unlock(&clientsMutex);
}

std::string Server::listUsers() {
    pthread_mutex_lock(&clientsMutex);
    std::string res = "Users:";
    for (auto* c : clients) res += " " + c->name;
    res += "\n";
    pthread_mutex_unlock(&clientsMutex);
    return res;
}

int Server::sendAll(int fd, const std::string& msg) {
    size_t sent = 0;
    while (sent < msg.size()) {
        ssize_t n = send(fd, msg.data() + sent, msg.size() - sent, 0);
        if (n <= 0) return -1;
        sent += n;
    }
    return 0;
}

bool Server::recvLine(int fd, std::string& out) {
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
