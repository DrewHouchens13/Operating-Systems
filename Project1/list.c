/*
 * list.c
 * Implementation file for linked list
 * 
 * Author: Drew Houchens
 * Course: Operating Systems
 * Project 1: C Programming and Makefile
 * 
 * Description: This file contains the implementation of a singly linked list
 * that stores character strings. All memory is dynamically allocated and
 * properly freed to prevent memory leaks.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"

/*
 * Allocate space for a new list and set its head to NULL.
 * Parameters: None
 * Returns: The created list if successful, NULL otherwise.
 */
list *create_list() {
    /* Allocate memory for the list structure */
    list *newList = (list *)malloc(sizeof(list));
    
    /* Check if allocation was successful */
    if (newList != NULL) {
        /* Initialize the head to NULL (empty list) */
        newList->head = NULL;
    }
    
    return newList;
}

/*
 * Allocates a new node and copies the string from item to this node
 * (use malloc, strlen, and strncpy; or try strdup). Adds this new node
 * to end of the list ll.
 * Parameters: list *ll - pointer to the list
 *             char *item - string to add to the list
 * Returns: 0 if successful, non-zero otherwise.
 */
int add_to_list(list *ll, char *item) {
    /* Check for NULL list pointer */
    if (ll == NULL) {
        return -1;
    }
    
    /* Check for NULL item pointer */
    if (item == NULL) {
        return -1;
    }
    
    /* Allocate memory for the new node */
    node *newNodePtr = (node *)malloc(sizeof(node));
    
    /* Check if node allocation was successful */
    if (newNodePtr == NULL) {
        return -1;
    }
    
    /* Copy the string using strdup (allocates memory and copies) */
    newNodePtr->item = strdup(item);
    
    /* Check if string allocation was successful */
    if (newNodePtr->item == NULL) {
        free(newNodePtr);
        return -1;
    }
    
    /* Set the next pointer to NULL (will be added at end) */
    newNodePtr->next = NULL;
    
    /* If the list is empty, new node becomes the head */
    if (ll->head == NULL) {
        ll->head = newNodePtr;
    }
    else {
        /* Traverse to the end of the list */
        node *currentPtr = ll->head;
        
        while (currentPtr->next != NULL) {
            currentPtr = currentPtr->next;
        }
        
        /* Add the new node at the end */
        currentPtr->next = newNodePtr;
    }
    
    return 0;
}

/*
 * Removes the head of the list ll (and move the head of ll to the next node
 * in the list), extracts the string stored in the head, and returns a
 * pointer to this string. Also frees the removed head node.
 * Parameters: list *ll - pointer to the list
 * Returns: Pointer to the extracted string, or NULL if list is empty.
 */
char *remove_from_list(list *ll) {
    /* Check for NULL list pointer */
    if (ll == NULL) {
        return NULL;
    }
    
    /* Check if list is empty */
    if (ll->head == NULL) {
        return NULL;
    }
    
    /* Save the head node and its item */
    node *headNode = ll->head;
    char *extractedItem = headNode->item;
    
    /* Move head to the next node */
    ll->head = headNode->next;
    
    /* Free the old head node (but NOT the string - caller owns it now) */
    free(headNode);
    
    return extractedItem;
}

/*
 * Prints every string in each node of the list ll, with a new line
 * character at the end of each string.
 * Parameters: list *ll - pointer to the list
 * Returns: Nothing
 */
void print_list(list *ll) {
    /* Check for NULL list pointer */
    if (ll == NULL) {
        return;
    }
    
    /* Traverse the list and print each item */
    node *currentPtr = ll->head;
    
    while (currentPtr != NULL) {
        printf("%s\n", currentPtr->item);
        currentPtr = currentPtr->next;
    }
}

/*
 * Flushes (clears) the entire list and re-initializes the list. The passed
 * pointer ll should still point to a valid, empty list when this function
 * returns. Any memory allocated to store nodes in the list should be freed.
 * Parameters: list *ll - pointer to the list
 * Returns: Nothing
 */
void flush_list(list *ll) {
    /* Check for NULL list pointer */
    if (ll == NULL) {
        return;
    }
    
    /* Traverse and free all nodes and their strings */
    node *currentPtr = ll->head;
    node *nextPtr = NULL;
    
    while (currentPtr != NULL) {
        /* Save the next pointer before freeing */
        nextPtr = currentPtr->next;
        
        /* Free the string stored in this node */
        free(currentPtr->item);
        
        /* Free the node itself */
        free(currentPtr);
        
        /* Move to the next node */
        currentPtr = nextPtr;
    }
    
    /* Re-initialize the list head to NULL (empty list) */
    ll->head = NULL;
}

/*
 * De-allocates all data for the list. Ensure all memory allocated for list
 * ll is freed, including any allocated strings and list ll itself.
 * Parameters: list **ll - pointer to pointer to the list
 * Returns: Nothing
 */
void free_list(list **ll) {
    /* Check for NULL pointer to pointer */
    if (ll == NULL) {
        return;
    }
    
    /* Check for NULL list pointer */
    if (*ll == NULL) {
        return;
    }
    
    /* First flush the list to free all nodes and strings */
    flush_list(*ll);
    
    /* Free the list structure itself */
    free(*ll);
    
    /* Set the pointer to NULL */
    *ll = NULL;
}
