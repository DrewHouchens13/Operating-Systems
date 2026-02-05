/*
 * list_test.c
 * Test file for linked list implementation
 * 
 * Author: Drew Houchens
 * Course: Operating Systems
 * Project 1: C Programming and Makefile
 * 
 * Description: This file contains comprehensive tests for the linked list
 * implementation. It tests all functions including edge cases such as
 * empty lists, NULL pointers, and proper memory management.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"

/* Function prototypes for test functions */
void test_create_list(void);
void test_add_to_list(void);
void test_remove_from_list(void);
void test_print_list(void);
void test_flush_list(void);
void test_free_list(void);
void test_edge_cases(void);
void test_integration(void);

/*
 * Main function - runs all tests
 * Parameters: None
 * Returns: 0 on success
 */
int main(void) {
    puts("==============================================");
    puts("    Linked List Implementation Tests");
    puts("==============================================\n");
    
    test_create_list();
    test_add_to_list();
    test_remove_from_list();
    test_print_list();
    test_flush_list();
    test_free_list();
    test_edge_cases();
    test_integration();
    
    puts("\n==============================================");
    puts("    All Tests Completed Successfully!");
    puts("==============================================");
    
    return 0;
}

/*
 * Test the create_list function
 * Parameters: None
 * Returns: Nothing
 */
void test_create_list(void) {
    puts("----------------------------------------------");
    puts("TEST: create_list()");
    puts("----------------------------------------------");
    
    /* Create a new list */
    list *myList = create_list();
    
    /* Verify the list was created */
    if (myList != NULL) {
        puts("PASS: List created successfully (not NULL)");
    }
    else {
        puts("FAIL: List creation returned NULL");
        return;
    }
    
    /* Verify the head is NULL (empty list) */
    if (myList->head == NULL) {
        puts("PASS: New list head is NULL (empty list)");
    }
    else {
        puts("FAIL: New list head should be NULL");
    }
    
    /* Clean up */
    free_list(&myList);
    puts("");
}

/*
 * Test the add_to_list function
 * Parameters: None
 * Returns: Nothing
 */
void test_add_to_list(void) {
    puts("----------------------------------------------");
    puts("TEST: add_to_list()");
    puts("----------------------------------------------");
    
    list *myList = create_list();
    int result;
    
    /* Test adding first item */
    result = add_to_list(myList, "First Item");
    if (result == 0) {
        puts("PASS: First item added successfully (return 0)");
    }
    else {
        puts("FAIL: First item add returned non-zero");
    }
    
    /* Verify head is no longer NULL */
    if (myList->head != NULL) {
        puts("PASS: List head is not NULL after adding item");
    }
    else {
        puts("FAIL: List head should not be NULL");
    }
    
    /* Verify the item was copied correctly */
    if (strcmp(myList->head->item, "First Item") == 0) {
        puts("PASS: First item string matches");
    }
    else {
        puts("FAIL: First item string does not match");
    }
    
    /* Test adding second item */
    result = add_to_list(myList, "Second Item");
    if (result == 0) {
        puts("PASS: Second item added successfully (return 0)");
    }
    else {
        puts("FAIL: Second item add returned non-zero");
    }
    
    /* Verify second item is at the end */
    if (myList->head->next != NULL && 
        strcmp(myList->head->next->item, "Second Item") == 0) {
        puts("PASS: Second item is at end of list");
    }
    else {
        puts("FAIL: Second item not found at end");
    }
    
    /* Test adding third item */
    result = add_to_list(myList, "Third Item");
    if (result == 0 && myList->head->next->next != NULL) {
        puts("PASS: Third item added to end of list");
    }
    else {
        puts("FAIL: Third item not added correctly");
    }
    
    /* Clean up */
    free_list(&myList);
    puts("");
}

/*
 * Test the remove_from_list function
 * Parameters: None
 * Returns: Nothing
 */
void test_remove_from_list(void) {
    puts("----------------------------------------------");
    puts("TEST: remove_from_list()");
    puts("----------------------------------------------");
    
    list *myList = create_list();
    char *removed = NULL;
    
    /* Add items to remove */
    add_to_list(myList, "First");
    add_to_list(myList, "Second");
    add_to_list(myList, "Third");
    
    /* Remove first item (should be "First") */
    removed = remove_from_list(myList);
    if (removed != NULL && strcmp(removed, "First") == 0) {
        puts("PASS: Removed first item correctly ('First')");
        free(removed);  /* Caller must free the returned string */
    }
    else {
        puts("FAIL: First removal did not return 'First'");
        if (removed != NULL) {
            free(removed);
        }
    }
    
    /* Remove second item (should now be "Second") */
    removed = remove_from_list(myList);
    if (removed != NULL && strcmp(removed, "Second") == 0) {
        puts("PASS: Removed second item correctly ('Second')");
        free(removed);
    }
    else {
        puts("FAIL: Second removal did not return 'Second'");
        if (removed != NULL) {
            free(removed);
        }
    }
    
    /* Remove third item (should now be "Third") */
    removed = remove_from_list(myList);
    if (removed != NULL && strcmp(removed, "Third") == 0) {
        puts("PASS: Removed third item correctly ('Third')");
        free(removed);
    }
    else {
        puts("FAIL: Third removal did not return 'Third'");
        if (removed != NULL) {
            free(removed);
        }
    }
    
    /* Verify list is now empty */
    if (myList->head == NULL) {
        puts("PASS: List is empty after removing all items");
    }
    else {
        puts("FAIL: List should be empty");
    }
    
    /* Clean up */
    free_list(&myList);
    puts("");
}

/*
 * Test the print_list function
 * Parameters: None
 * Returns: Nothing
 */
void test_print_list(void) {
    puts("----------------------------------------------");
    puts("TEST: print_list()");
    puts("----------------------------------------------");
    
    list *myList = create_list();
    
    /* Test printing empty list (should not crash) */
    puts("Printing empty list (no output expected):");
    print_list(myList);
    puts("PASS: Empty list print did not crash");
    
    /* Add items and print */
    add_to_list(myList, "Apple");
    add_to_list(myList, "Banana");
    add_to_list(myList, "Cherry");
    
    puts("\nPrinting list with 3 items:");
    puts("Expected: Apple, Banana, Cherry (each on new line)");
    puts("Actual output:");
    print_list(myList);
    puts("PASS: Print completed (verify order above)");
    
    /* Clean up */
    free_list(&myList);
    puts("");
}

/*
 * Test the flush_list function
 * Parameters: None
 * Returns: Nothing
 */
void test_flush_list(void) {
    puts("----------------------------------------------");
    puts("TEST: flush_list()");
    puts("----------------------------------------------");
    
    list *myList = create_list();
    
    /* Add items */
    add_to_list(myList, "Item1");
    add_to_list(myList, "Item2");
    add_to_list(myList, "Item3");
    
    /* Verify list is not empty before flush */
    if (myList->head != NULL) {
        puts("PASS: List has items before flush");
    }
    else {
        puts("FAIL: List should have items before flush");
    }
    
    /* Flush the list */
    flush_list(myList);
    
    /* Verify list is empty after flush */
    if (myList->head == NULL) {
        puts("PASS: List head is NULL after flush");
    }
    else {
        puts("FAIL: List head should be NULL after flush");
    }
    
    /* Verify the list pointer is still valid (can add items again) */
    int result = add_to_list(myList, "New Item After Flush");
    if (result == 0 && myList->head != NULL) {
        puts("PASS: Can add items after flush (list still valid)");
    }
    else {
        puts("FAIL: Cannot add items after flush");
    }
    
    /* Clean up */
    free_list(&myList);
    puts("");
}

/*
 * Test the free_list function
 * Parameters: None
 * Returns: Nothing
 */
void test_free_list(void) {
    puts("----------------------------------------------");
    puts("TEST: free_list()");
    puts("----------------------------------------------");
    
    list *myList = create_list();
    
    /* Add items */
    add_to_list(myList, "ToBeFreed1");
    add_to_list(myList, "ToBeFreed2");
    
    /* Free the list */
    free_list(&myList);
    
    /* Verify the pointer is now NULL */
    if (myList == NULL) {
        puts("PASS: List pointer is NULL after free_list");
    }
    else {
        puts("FAIL: List pointer should be NULL after free_list");
    }
    
    puts("");
}

/*
 * Test edge cases
 * Parameters: None
 * Returns: Nothing
 */
void test_edge_cases(void) {
    puts("----------------------------------------------");
    puts("TEST: Edge Cases");
    puts("----------------------------------------------");
    
    list *myList = create_list();
    char *removed = NULL;
    
    /* Test remove from empty list */
    removed = remove_from_list(myList);
    if (removed == NULL) {
        puts("PASS: Remove from empty list returns NULL");
    }
    else {
        puts("FAIL: Remove from empty list should return NULL");
        free(removed);
    }
    
    /* Test flush on empty list (should not crash) */
    flush_list(myList);
    if (myList->head == NULL) {
        puts("PASS: Flush on empty list works (no crash)");
    }
    else {
        puts("FAIL: Flush on empty list caused issue");
    }
    
    /* Test print on NULL list (should not crash) */
    print_list(NULL);
    puts("PASS: Print NULL list did not crash");
    
    /* Test flush on NULL list (should not crash) */
    flush_list(NULL);
    puts("PASS: Flush NULL list did not crash");
    
    /* Test add with NULL list */
    int result = add_to_list(NULL, "Test");
    if (result != 0) {
        puts("PASS: Add to NULL list returns non-zero");
    }
    else {
        puts("FAIL: Add to NULL list should return non-zero");
    }
    
    /* Test add with NULL item */
    result = add_to_list(myList, NULL);
    if (result != 0) {
        puts("PASS: Add NULL item returns non-zero");
    }
    else {
        puts("FAIL: Add NULL item should return non-zero");
    }
    
    /* Test remove from NULL list */
    removed = remove_from_list(NULL);
    if (removed == NULL) {
        puts("PASS: Remove from NULL list returns NULL");
    }
    else {
        puts("FAIL: Remove from NULL list should return NULL");
        free(removed);
    }
    
    /* Clean up */
    free_list(&myList);
    puts("");
}

/*
 * Integration test - full workflow
 * Parameters: None
 * Returns: Nothing
 */
void test_integration(void) {
    puts("----------------------------------------------");
    puts("TEST: Integration (Full Workflow)");
    puts("----------------------------------------------");
    
    /* Create list */
    list *myList = create_list();
    if (myList == NULL) {
        puts("FAIL: Could not create list for integration test");
        return;
    }
    puts("Step 1: Created new list");
    
    /* Add multiple items */
    add_to_list(myList, "Task 1");
    add_to_list(myList, "Task 2");
    add_to_list(myList, "Task 3");
    add_to_list(myList, "Task 4");
    add_to_list(myList, "Task 5");
    puts("Step 2: Added 5 items to list");
    
    /* Print the list */
    puts("Step 3: Printing list contents:");
    print_list(myList);
    
    /* Remove some items (FIFO order) */
    char *item1 = remove_from_list(myList);
    char *item2 = remove_from_list(myList);
    printf("Step 4: Removed '%s' and '%s' from head\n", item1, item2);
    free(item1);
    free(item2);
    
    /* Print remaining */
    puts("Step 5: List after removing 2 items:");
    print_list(myList);
    
    /* Flush and verify reusable */
    flush_list(myList);
    puts("Step 6: Flushed the list");
    
    /* Add new items */
    add_to_list(myList, "New Task A");
    add_to_list(myList, "New Task B");
    puts("Step 7: Added 2 new items after flush");
    
    puts("Step 8: Printing list after flush and re-add:");
    print_list(myList);
    
    /* Free everything */
    free_list(&myList);
    if (myList == NULL) {
        puts("Step 9: List freed and pointer set to NULL");
    }
    
    puts("\nPASS: Integration test completed successfully");
    puts("");
}
