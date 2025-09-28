
CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++20

SRCS = read-file.cpp
OBJS = $(SRCS:.cpp=.o)

read-file = program

all: $(read-file)

$(read-file): $(OBJS)
	$(CXX) $(OBJS) -o $(read-file)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(read-file)
