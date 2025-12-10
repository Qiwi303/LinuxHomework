cmake_minimum_required(VERSION 3.10)
project(shared_array)

set(CMAKE_CXX_STANDARD 17)

add_executable(first
    first.cpp
    sharedArray.cpp
)

add_executable(second
    second.cpp
    sharedArray.cpp
)

target_link_libraries(first pthread)
target_link_libraries(second pthread)
