
CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++20

SRCS = simple-copy.cpp
OBJS = $(SRCS:.cpp=.o)

simple-copy = program

all: $(simple-copy)

$(simple-copy): $(OBJS)
	$(CXX) $(OBJS) -o $(simple-copy)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(simple-copy)
