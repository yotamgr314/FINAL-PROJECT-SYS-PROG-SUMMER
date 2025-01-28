# Compiler and flags
CC = gcc
CFLAGS = -Wall -g

# Source files and output binary
SRCS = main.c httpd.c http_protocol.c router.c
OBJS = $(SRCS:.c=.o)
TARGET = server

# Default target
all: $(TARGET)

# Rule to link object files and create the executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

# Rule to compile source files into object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Run the server
run: $(TARGET)
	./$(TARGET)


clean:
	rm -f $(OBJS) $(TARGET)

