================================================================================
                        Operating Systems - Project 1
                        C Programming and Makefile
================================================================================

Project Members:
----------------
Drew Houchens

Honor Statement:
----------------
We have neither given nor received unauthorized assistance on this work.

================================================================================
                            VM Information
================================================================================

VM Directory:       dhouchen/HouchensUbuntu
Code Path in VM:    [/home/dhouchen/Operating-Systems/Project1]
Account Password:   [BRONCOS]


================================================================================
                          Build Instructions
================================================================================

1. Navigate to the project directory containing all source files.

2. Build the project using make:
   $ make

3. Run the test program:
   $ ./list_test

4. Clean up compiled files (optional):
   $ make clean

================================================================================
                          Project Description
================================================================================

This project implements a singly linked list data structure in C. The linked
list stores character strings and supports the following operations:

Files Included:
---------------
- list.h        : Header file with struct definitions and function prototypes
- list.c        : Implementation of all linked list functions
- list_test.c   : Comprehensive test file for the linked list implementation
- Makefile      : Build automation with gcc and -Wall flag
- README.txt    : This documentation file
- vm_setup.sh   : Helper script for automated build and test on Ubuntu VM

VM Setup Script (vm_setup.sh):
------------------------------
A convenience script is provided to automate the build and test process on the
Ubuntu VM. To use it:

   $ chmod +x vm_setup.sh
   $ ./vm_setup.sh

The script will:
1. Install required tools (make, gcc, dos2unix) if not present
2. Convert all source files to Unix line endings
3. Build the project using make
4. Run the test suite automatically

Functions Implemented:
----------------------
1. create_list()      - Creates a new empty list
2. add_to_list()      - Adds a string to the end of the list
3. remove_from_list() - Removes and returns the string from the head
4. print_list()       - Prints all strings in the list
5. flush_list()       - Clears the list but keeps it valid for reuse
6. free_list()        - Frees all memory and sets list pointer to NULL

================================================================================
                       Implementation Approach
================================================================================

Data Structures:
----------------
- The Node struct contains a char pointer (item) for the string and a pointer
  to the next node (next).
- The List struct contains a single pointer to the head node.
- Memory for strings is allocated using strdup(), which handles malloc and
  strcpy internally.

Memory Management:
------------------
- All malloc calls are checked for NULL returns to handle allocation failures.
- The remove_from_list function returns ownership of the string to the caller,
  who is responsible for freeing it.
- flush_list frees all nodes and their strings but keeps the list valid.
- free_list calls flush_list first, then frees the list struct itself.

Testing Strategy:
-----------------
The test file (list_test.c) includes:
- Individual tests for each function
- Edge case testing (NULL pointers, empty lists)
- Integration test demonstrating full workflow
- Verification of FIFO behavior (add to end, remove from head)

================================================================================
                          Challenges Overcome
================================================================================

1. Memory Ownership: Understanding that remove_from_list transfers string
   ownership to the caller required careful design to avoid memory leaks.

2. Double Pointer in free_list: Using list** as parameter allows setting the
   caller's pointer to NULL, preventing dangling pointer issues.

3. Cross-Platform Development: Code was developed on Windows and tested on
   Ubuntu VM. Used dos2unix to convert line endings before compilation.

================================================================================
                          Resources Used
================================================================================

- Stanford CS Library (cslibrary.stanford.edu) - C programming reference
- GNU Make Manual - Makefile syntax and best practices
- man pages for malloc, free, strdup, strlen, strncpy

================================================================================
                        AI Assistance Statement
================================================================================

I received assistance from Claude Opus 4.5 (Anthropic) in building the test
file (list_test.c) and documentation (README.txt). The core linked list
implementation logic in list.c was developed with my own understanding of
data structures and memory management in C.

================================================================================
                          Notes for Grading
================================================================================

- All code compiles without warnings using gcc with -Wall flag
- No arrays are used in the implementation
- All memory is properly freed (no memory leaks)
- Test file demonstrates comprehensive coverage of all functions

================================================================================
