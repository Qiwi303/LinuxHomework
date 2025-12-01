
PROJECT(threadPool)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


add_library(parallelScheduler SHARED parallelScheduler.cpp)
target_link_libraries(parallelScheduler pthread)


add_executable(threadPool threadPool.cpp)
target_link_libraries(threadPool parallelScheduler pthread)
