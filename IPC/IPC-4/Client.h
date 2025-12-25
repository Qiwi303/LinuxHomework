#pragma once
#include <string>
#include <pthread.h>
#include <cstdint>

class Client {
public:
    Client(const std::string& ip, uint16_t port, const std::string& name);
    ~Client();

    void run();

private:
    int fd;
    std::string name;
    pthread_t recvThread;
    bool running;

    static void* recvEntry(void* arg);
    void recvLoop();

    static int sendAll(int fd, const std::string& msg);
    static bool recvLine(int fd, std::string& out);
};
