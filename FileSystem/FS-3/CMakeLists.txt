project(hacker)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCES hacker-rm.cpp)

add_executable(hacker ${SOURCES})
