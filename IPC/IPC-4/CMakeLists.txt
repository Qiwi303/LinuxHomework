cmake_minimum_required(VERSION 3.10)

project(ipc_tcp_chat LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Threads REQUIRED)

add_executable(server
    Server.cpp
    ServerMain.cpp
)

add_executable(client
    Client.cpp
    ClientMain.cpp
)

target_compile_options(server PRIVATE -Wall -Wextra -pedantic)
target_compile_options(client PRIVATE -Wall -Wextra -pedantic)

target_link_libraries(server PRIVATE Threads::Threads)
target_link_libraries(client PRIVATE Threads::Threads)
