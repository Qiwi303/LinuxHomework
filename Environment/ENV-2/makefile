	
CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++20

SRCS = cnas_main.cpp  cnas_class.cpp sorting_algorithm.cpp
OBJS = $(SRCS:.cpp=.o)

CNAS = program

all: $(CNAS)

$(CNAS): $(OBJS)
	$(CXX) $(OBJS) -o $(CNAS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(CNAS)
