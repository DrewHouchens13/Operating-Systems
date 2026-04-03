/*
 * my_list-forming.c
 *
 * Optimized version of list-forming.c.  Three changes reduce runtime:
 *
 * 1. LOCAL LIST BATCHING -- Each thread builds a private linked list of
 *    K nodes without holding any lock.  Only after all K nodes are ready
 *    does the thread lock the global mutex once and splice the local list
 *    onto the global list.  This drops lock acquisitions from K per thread
 *    to just 1 per thread.
 *
 * 2. pthread_mutex_lock INSTEAD OF pthread_mutex_trylock -- The original
 *    code spins in a while(1) loop calling trylock.  Switching to a
 *    blocking lock lets waiting threads yield the CPU instead of burning
 *    cycles spinning.
 *
 * 3. NO CPU PINNING -- The original pins each thread to a specific CPU.
 *    When many threads contend on the same mutex and global list, pinning
 *    causes cache-line bouncing (the mutex and list tail keep migrating
 *    between per-CPU caches).  Letting the OS scheduler choose CPUs
 *    freely can reduce this overhead.
 *
 * Usage: ./my_list-forming <num_threads>
 *
 * Author:  Drew Houchens
 * Course:  OS 4500
 * Project: 3 - Task 3
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/time.h>
#include <sys/param.h>
#include <sched.h>

#define K 200

struct Node
{
    int data;
    struct Node *next;
};

struct list
{
    struct Node *header;
    struct Node *tail;
};

pthread_mutex_t mutex_lock;

struct list *List;

struct Node *generate_data_node()
{
    struct Node *ptr;
    ptr = (struct Node *)malloc(sizeof(struct Node));

    if (NULL != ptr) {
        ptr->data = 1;
        ptr->next = NULL;
    } else {
        printf("Node allocation failed!\n");
    }
    return ptr;
}

/*
 * Each thread builds a local list of K nodes entirely in its own memory,
 * then acquires the global lock exactly once to attach the whole local
 * list to the global list.
 */
void *producer_thread(void *arg)
{
    struct Node *local_head = NULL;
    struct Node *local_tail = NULL;
    int counter = 0;

    /* Phase 1: build a local list — no lock needed */
    while (counter < K) {
        struct Node *ptr = generate_data_node();
        if (ptr != NULL) {
            if (local_head == NULL) {
                local_head = local_tail = ptr;
            } else {
                local_tail->next = ptr;
                local_tail = ptr;
            }
        }
        counter++;
    }

    /* Phase 2: splice the local list into the global list — one lock */
    if (local_head != NULL) {
        pthread_mutex_lock(&mutex_lock);

        if (List->header == NULL) {
            List->header = local_head;
            List->tail   = local_tail;
        } else {
            List->tail->next = local_head;
            List->tail       = local_tail;
        }

        pthread_mutex_unlock(&mutex_lock);
    }

    return NULL;
}

int main(int argc, char *argv[])
{
    int i, num_threads;
    struct Node *tmp, *next;
    struct timeval starttime, endtime;

    if (argc == 1) {
        printf("ERROR: please provide an input arg (the number of threads)\n");
        exit(1);
    }

    num_threads = atoi(argv[1]);
    pthread_t producer[num_threads];

    pthread_mutex_init(&mutex_lock, NULL);

    List = (struct list *)malloc(sizeof(struct list));
    if (NULL == List) {
        printf("End here\n");
        exit(0);
    }
    List->header = List->tail = NULL;

    gettimeofday(&starttime, NULL);

    for (i = 0; i < num_threads; i++)
        pthread_create(&producer[i], NULL, producer_thread, NULL);

    for (i = 0; i < num_threads; i++)
        pthread_join(producer[i], NULL);

    gettimeofday(&endtime, NULL);

    if (List->header != NULL) {
        next = tmp = List->header;
        while (tmp != NULL) {
            next = tmp->next;
            free(tmp);
            tmp = next;
        }
    }

    pthread_mutex_destroy(&mutex_lock);
    free(List);

    printf("Total run time is %ld microseconds.\n",
           (endtime.tv_sec - starttime.tv_sec) * 1000000 +
           (endtime.tv_usec - starttime.tv_usec));
    return 0;
}
