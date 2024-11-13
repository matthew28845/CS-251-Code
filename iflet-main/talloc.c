


#include <stdlib.h>
#include "object.h"
#include <stdio.h>

#ifndef _TALLOC
#define _TALLOC

typedef struct Node Node;

struct Node {
    int* addr;
    Node* next;
};

Node* head;

// Input size: The number of bytes to allocate from the heap.
// Return: A pointer to heap-allocated memory of size bytes. NULL upon failure.
// A replacement for the built-in C function malloc. This function tracks the 
// allocated heap memory in a data structure, such that tfree can free it later.
int *talloc(size_t size) {
    // Make a new node head
    // Add old head to tail of new head
    Node* tempHead = malloc(sizeof(Node));
    tempHead->addr = malloc(size);
    tempHead->next = head;
    head = tempHead;

    return head->addr;
} 

// Frees all heap memory previously talloced (as well as any memory needed to 
// administer that memory).
void tfree() {
    // Create a temporary node to hold head for freeing
    Node *currentNode = head;

    // Traverse to next node, and free temp node
    while (head != NULL) {
        free(head->addr);

        currentNode = head;
        head = head->next;

        free(currentNode);
    }
}

// Input status: A C error code. Zero if no error, non-zero if error.
// A replacement for the built-in C function exit. Calls tfree and then exit.
void texit(int status) {
    tfree();
    exit(status);
}

#endif

