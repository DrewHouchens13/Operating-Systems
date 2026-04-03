/*
 * substring_parallel.c
 *
 * Parallel substring counter using POSIX threads.
 * Partitions the search index space across NUM_THREADS threads so that
 * boundary substrings are never missed.
 *
 * Usage: ./substring_parallel <num_threads>
 * Input:  strings.txt  (line 1 = haystack s1, line 2 = needle s2)
 *
 * Author:  Drew Houchens
 * Course:  OS 4500
 * Project: 3 - Task 1
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>

#define MAX 1024

int n1, n2;
char *s1, *s2;

typedef struct {
    int start;
    int end;
    int local_count;
} thread_arg_t;

int readf(void)
{
    FILE *fp;
    if ((fp = fopen("strings.txt", "r")) == NULL) {
        printf("ERROR: can't open strings.txt!\n");
        return 0;
    }

    s1 = (char *)malloc(sizeof(char) * MAX);
    if (s1 == NULL) {
        printf("ERROR: Out of memory!\n");
        fclose(fp);
        return -1;
    }

    s2 = (char *)malloc(sizeof(char) * MAX);
    if (s2 == NULL) {
        printf("ERROR: Out of memory!\n");
        free(s1);
        fclose(fp);
        return -1;
    }

    s1 = fgets(s1, MAX, fp);
    s2 = fgets(s2, MAX, fp);
    fclose(fp);

    if (s1 == NULL || s2 == NULL)
        return -1;

    n1 = strlen(s1) - 1;  /* exclude trailing newline */
    n2 = strlen(s2) - 1;

    if (n1 < n2)
        return -1;

    return 0;
}

/*
 * Each thread scans indices [arg->start, arg->end) of s1,
 * checking whether s2 appears starting at each index.
 * The result is stored in arg->local_count (private to each thread),
 * so no mutex is needed during the search itself.
 */
void *count_substring(void *param)
{
    thread_arg_t *arg = (thread_arg_t *)param;
    int i, j, k, match;

    arg->local_count = 0;

    for (i = arg->start; i < arg->end; i++) {
        match = 1;
        for (j = i, k = 0; k < n2; j++, k++) {
            if (*(s1 + j) != *(s2 + k)) {
                match = 0;
                break;
            }
        }
        if (match)
            arg->local_count++;
    }

    return NULL;
}

int main(int argc, char *argv[])
{
    int i, num_threads, total;
    int range, start;

    if (argc != 2) {
        printf("Usage: %s <num_threads>\n", argv[0]);
        return 1;
    }

    num_threads = atoi(argv[1]);
    if (num_threads <= 0) {
        printf("ERROR: num_threads must be a positive integer\n");
        return 1;
    }

    if (readf() != 0) {
        printf("ERROR: failed to read input strings\n");
        return 1;
    }

    int num_positions = n1 - n2 + 1; /* total starting positions to check */

    if (num_threads > num_positions)
        num_threads = num_positions;

    pthread_t *threads = malloc(sizeof(pthread_t) * num_threads);
    thread_arg_t *args = malloc(sizeof(thread_arg_t) * num_threads);

    if (threads == NULL || args == NULL) {
        printf("ERROR: Out of memory!\n");
        return 1;
    }

    /*
     * Partition the index range [0, num_positions) evenly.
     * The last thread picks up any remainder positions.
     */
    range = num_positions / num_threads;
    start = 0;

    for (i = 0; i < num_threads; i++) {
        args[i].start = start;
        if (i == num_threads - 1)
            args[i].end = num_positions;
        else
            args[i].end = start + range;
        args[i].local_count = 0;
        start = args[i].end;
    }

    for (i = 0; i < num_threads; i++)
        pthread_create(&threads[i], NULL, count_substring, &args[i]);

    for (i = 0; i < num_threads; i++)
        pthread_join(threads[i], NULL);

    /* Sum local counts — no lock needed since all threads have finished */
    total = 0;
    for (i = 0; i < num_threads; i++)
        total += args[i].local_count;

    printf("The number of substrings is: %d\n", total);

    free(threads);
    free(args);
    free(s1);
    free(s2);
    return 0;
}
