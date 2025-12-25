#pragma once
#include <string>
#include <vector>
#include <pthread.h>
#include <cstdint>

class Server {
public:
    explicit Server(uint16_t port);
    ~Server();

    void run();

private:
    struct Client {
        int fd;
        std::string name;
        pthread_t thread;
    };

    int listenFd;
    uint16_t port;

    std::vector<Client*> clients;
    pthread_mutex_t clientsMutex;

    static void* clientEntry(void* arg);
    void handleClient(Client* client);

    void broadcast(const std::string& msg, int exceptFd = -1);
    void removeClient(int fd);
    std::string listUsers();

    static int sendAll(int fd, const std::string& msg);
    static bool recvLine(int fd, std::string& out);
};
