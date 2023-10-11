# Define the compiler and flags
CXX = g++
CXXFLAGS = -std=c++11

# Get a list of all .cpp files in the current directory
SRCS := $(wildcard *.cpp)

# Default target to build the "program" executable
program: $(SRCS)
	$(CXX) $(CXXFLAGS) $(SRCS) -o program

# Clean up generated executable
clean:
	rm -f program
