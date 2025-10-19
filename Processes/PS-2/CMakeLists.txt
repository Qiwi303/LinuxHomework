project(Shell)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCES interactiveShell.cpp Shell.cpp)

add_executable(shell ${SOURCES})
