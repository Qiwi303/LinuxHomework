#include "Client.h"
#include <iostream>

int main(int argc, char** argv) {
    if (argc != 4) {
        std::cerr << "Usage: client <ip> <port> <name>\n";
        return 1;
    }

    try {
        Client c(argv[1], std::stoi(argv[2]), argv[3]);
        c.run();
    } catch (const std::exception& e) {
        std::cerr << e.what() << "\n";
        return 1;
    }
}
