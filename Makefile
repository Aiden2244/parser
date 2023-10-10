CC = g++
CFLAGS = -g -Wall
TARGET = reader

# Use wildcard to find all .cpp files and then determine the object files from them
SRCS = $(wildcard *.cpp)
OBJS = $(SRCS:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

%.o: %.cpp
	$(CC) $(CFLAGS) -c $<

clean:
	$(RM) $(TARGET) $(OBJS)

