/*
 * list.h
 * Header file for linked list implementation
 * 
 * Author: Drew Houchens
 * Course: Operating Systems
 * Project 1: C Programming and Makefile
 * 
 * Description: This header file contains the struct definitions and function
 * prototypes for a singly linked list that stores character strings.
 */

#ifndef LIST_H
#define LIST_H

/* Declaring all the structs */
typedef struct Node node;

struct Node {
    char *item;
    node *next;
};

typedef struct List {
    node *head;
} list;

/*
 * Allocate space for a new list and set its head to NULL.
 * Parameters: None
 * Returns: The created list if successful, NULL otherwise.
 */
list *create_list();

/*
 * Allocates a new node and copies the string from item to this node
 * (use malloc, strlen, and strncpy; or try strdup). Adds this new node
 * to end of the list ll.
 * Parameters: list *ll - pointer to the list
 *             char *item - string to add to the list
 * Returns: 0 if successful, non-zero otherwise.
 */
int add_to_list(list *ll, char *item);

/*
 * Removes the head of the list ll (and move the head of ll to the next node
 * in the list), extracts the string stored in the head, and returns a
 * pointer to this string. Also frees the removed head node.
 * Parameters: list *ll - pointer to the list
 * Returns: Pointer to the extracted string, or NULL if list is empty.
 */
char *remove_from_list(list *ll);

/*
 * Prints every string in each node of the list ll, with a new line
 * character at the end of each string.
 * Parameters: list *ll - pointer to the list
 * Returns: Nothing
 */
void print_list(list *ll);

/*
 * Flushes (clears) the entire list and re-initializes the list. The passed
 * pointer ll should still point to a valid, empty list when this function
 * returns. Any memory allocated to store nodes in the list should be freed.
 * Parameters: list *ll - pointer to the list
 * Returns: Nothing
 */
void flush_list(list *ll);

/*
 * De-allocates all data for the list. Ensure all memory allocated for list
 * ll is freed, including any allocated strings and list ll itself.
 * Parameters: list **ll - pointer to pointer to the list
 * Returns: Nothing
 */
void free_list(list **ll);

#endif /* LIST_H */
