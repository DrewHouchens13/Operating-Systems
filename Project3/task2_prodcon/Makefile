CC = gcc
CFLAGS = -pthread -D_GNU_SOURCE -Wall

all: prodcon

prodcon: prodcon.c
	$(CC) $(CFLAGS) -o prodcon prodcon.c

clean:
	rm -f prodcon
