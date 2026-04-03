CC = gcc
CFLAGS = -pthread -D_GNU_SOURCE -Wall

all: list-forming my_list-forming

list-forming: list-forming.c
	$(CC) $(CFLAGS) -o list-forming list-forming.c

my_list-forming: my_list-forming.c
	$(CC) $(CFLAGS) -o my_list-forming my_list-forming.c

clean:
	rm -f list-forming my_list-forming
