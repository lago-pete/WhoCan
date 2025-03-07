# Compiler settings
CC = gcc
CFLAGS = -g -Wall -Wstrict-prototypes
LDFLAGS = -g

# Program name
BINS = whocan

# Source and object files
SRCS = main.c whocan.c
OBJS = main.o whocan.o

# Default target
.PHONY: default
default: $(BINS)

# Compile main.c into main.o
main.o: main.c whocan.h
	$(CC) $(CFLAGS) -c main.c -o main.o

# Compile whocan.c into whocan.o
whocan.o: whocan.c whocan.h
	$(CC) $(CFLAGS) -c whocan.c -o whocan.o

# Link object files into the final executable
whocan: $(OBJS)
	$(CC) $(LDFLAGS) $(OBJS) -o whocan

# Clean object files and temporary files
.PHONY: clean
clean:
	rm -f core* *.o *~

# Remove binaries and clean everything
.PHONY: immaculate
immaculate: clean
	rm -f $(BINS)
