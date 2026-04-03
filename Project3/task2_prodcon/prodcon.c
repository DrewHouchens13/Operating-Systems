/*
 * prodcon.c
 *
 * Producer-consumer problem using POSIX threads and condition variables.
 * The producer reads characters from message.txt one at a time and places
 * them into a circular buffer of size 15.  The consumer reads from the
 * buffer and prints each character in order.
 *
 * Usage: ./prodcon
 * Input:  message.txt  (arbitrary text message)
 *
 * Author:  Drew Houchens
 * Course:  OS 4500
 * Project: 3 - Task 2
 */

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

#define BUFFER_SIZE 15

char buffer[BUFFER_SIZE];
int count = 0;
int head  = 0;
int tail  = 0;
int done  = 0;   /* set by producer when EOF is reached */

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t not_full  = PTHREAD_COND_INITIALIZER;
pthread_cond_t not_empty = PTHREAD_COND_INITIALIZER;

/*
 * The producer opens message.txt, reads one character at a time, and
 * inserts it into the circular buffer.  When the buffer is full, it
 * blocks on the not_full condition variable until the consumer frees
 * a slot.  After EOF it sets the 'done' flag and signals the consumer
 * one last time so it knows to exit.
 */
void *producer(void *arg)
{
    FILE *fp = fopen("message.txt", "r");
    if (fp == NULL) {
        printf("ERROR: can't open message.txt!\n");
        pthread_exit(NULL);
    }

    int ch;
    while ((ch = fgetc(fp)) != EOF) {
        pthread_mutex_lock(&mutex);

        while (count == BUFFER_SIZE)
            pthread_cond_wait(&not_full, &mutex);

        buffer[tail] = (char)ch;
        tail = (tail + 1) % BUFFER_SIZE;
        count++;

        pthread_cond_signal(&not_empty);
        pthread_mutex_unlock(&mutex);
    }

    fclose(fp);

    pthread_mutex_lock(&mutex);
    done = 1;
    pthread_cond_signal(&not_empty);
    pthread_mutex_unlock(&mutex);

    return NULL;
}

/*
 * The consumer removes characters from the circular buffer and prints
 * them.  When the buffer is empty it blocks on the not_empty condition
 * variable.  It exits once the producer has set the 'done' flag and the
 * buffer has been fully drained.
 */
void *consumer(void *arg)
{
    char ch;

    while (1) {
        pthread_mutex_lock(&mutex);

        while (count == 0 && !done)
            pthread_cond_wait(&not_empty, &mutex);

        if (count == 0 && done) {
            pthread_mutex_unlock(&mutex);
            break;
        }

        ch = buffer[head];
        head = (head + 1) % BUFFER_SIZE;
        count--;

        pthread_cond_signal(&not_full);
        pthread_mutex_unlock(&mutex);

        printf("%c", ch);
    }

    return NULL;
}

int main(int argc, char *argv[])
{
    pthread_t prod_thread, cons_thread;

    pthread_create(&prod_thread, NULL, producer, NULL);
    pthread_create(&cons_thread, NULL, consumer, NULL);

    pthread_join(prod_thread, NULL);
    pthread_join(cons_thread, NULL);

    printf("\n");

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&not_full);
    pthread_cond_destroy(&not_empty);

    return 0;
}
