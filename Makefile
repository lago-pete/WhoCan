# Compiler settings
CC = gcc
CFLAGS = -g -Wall -Wstrict-prototypes
LDFLAGS = -g

# Program name
BINS = whocan

# Source and object files
SRCS = main.c
OBJS = main.o

.PHONY: default
default: $(BINS)

# Compile main.c into main.o
main.o: main.c
	$(CC) $(CFLAGS) -c main.c -o main.o

# Link object files into the final executable
whocan: $(OBJS)
	$(CC) $(LDFLAGS) $(OBJS) -o whocan

.PHONY: clean
clean:
	rm -f core* *.o *~

.PHONY: immaculate
immaculate: clean
	rm -f $(BINS)
