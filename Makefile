# Compiler and flags
CXX = g++
CXXFLAGS = -Iinclude

# Source and object files
SRC = $(wildcard src/*.cpp) main.cpp
OBJ = $(SRC:.cpp=.o)

# Test source and object files
TEST_SRC = $(wildcard tests/*.cpp)
TEST_OBJ = $(TEST_SRC:.cpp=.o)

# Targets
all: my_program

my_program: $(OBJ)
	$(CXX) $(CXXFLAGS) $^ -o $@

# Pattern rule for object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Test compilation
test: $(filter-out main.o, $(OBJ)) $(TEST_OBJ)
	$(CXX) $(CXXFLAGS) $^ -o $@

# Run tests
run_tests: test
	./test

# Clean
clean:
	rm -f my_program test $(OBJ) $(TEST_OBJ)

# Phony targets
.PHONY: all test run_tests clean

