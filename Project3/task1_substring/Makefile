CC = gcc
CFLAGS = -pthread -D_GNU_SOURCE -Wall

all: substring_parallel

substring_parallel: substring_parallel.c
	$(CC) $(CFLAGS) -o substring_parallel substring_parallel.c

clean:
	rm -f substring_parallel
