#include "Server.h"
#include <iostream>

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "Usage: server <port>\n";
        return 1;
    }

    uint16_t port = std::stoi(argv[1]);
    try {
        Server server(port);
        server.run();
    } catch (const std::exception& e) {
        std::cerr << e.what() << "\n";
        return 1;
    }
}
