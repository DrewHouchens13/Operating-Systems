# Makefile for Linked List Project
# Author: Drew Houchens
# Course: Operating Systems
# Project 1: C Programming and Makefile
#
# Usage:
#   make          - Build the list_test executable
#   make clean    - Remove all compiled files
#   make all      - Same as make

# Compiler settings
CC = gcc
CFLAGS = -Wall

# Target executable name
TARGET = list_test

# Object files
OBJS = list.o list_test.o

# Default target - build the executable
all: $(TARGET)

# Link object files to create executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

# Compile list.c to list.o
list.o: list.c list.h
	$(CC) $(CFLAGS) -c list.c

# Compile list_test.c to list_test.o
list_test.o: list_test.c list.h
	$(CC) $(CFLAGS) -c list_test.c

# Clean up compiled files
clean:
	rm -f $(OBJS) $(TARGET)

# Phony targets (not actual files)
.PHONY: all clean
