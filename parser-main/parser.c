

#include "object.h"
#include "linkedlist.h"
#include <stddef.h>
#include <stdio.h>

#ifndef _PARSER
#define _PARSER

// Input tokens: A linked list of tokens. The output of the tokenize function.
// Return: A linked list that stores the abstract syntax tree (forest, actually)
// for that token list. If a syntax error is encountered, then an error message
// is printed and the program cleanly exits.
Object *parse(Object *tokens) {
    printf("FUCK YOU BITCH");
    ConsCell *tokensList = (ConsCell *)tokens;
    ConsCell *tokenStack = (ConsCell *)makeNull();

    while (tokensList->type != NULL_TYPE) {d
    //     printf("tokensList Type: %u", tokensList->type);

    //     // While not close type, continue to pop from tokensList and add to tokenStack
    //     if (tokensList->car->type != CLOSE_TYPE) {
    //         tokenStack = (ConsCell *)cons(tokensList->car, (Object *)tokenStack);
    //     }
    //     else {
    //         ConsCell *poppedList = (ConsCell *)makeNull();

    //         // While not open type, continue to pop from tokensStack and add to poppedList
    //         while (tokenStack->car->type != OPEN_TYPE) {
    //             poppedList = (ConsCell *)cons(tokenStack->car, (Object *)poppedList);
    //             tokenStack = (ConsCell *)tokenStack->cdr;
    //         }

    //         // Add poppedList to tokenStack
    //         tokenStack = (ConsCell *)cons((Object *)poppedList, (Object *)tokenStack);
    //     }
        
    //     tokensList = (ConsCell *)cdr((Object *)tokensList);
    }

    return reverse((Object *)tokenStack);
}

// Input tree: An abstract syntax tree (forest). The output of parse.
// Prints the tree in a human-readable format that closely resembles the
// original Scheme code that led to the abstract syntax tree.
void printTree(Object *tree) {
    ConsCell *treeList = (ConsCell *)tree;

    // Iterate through the stack
    while (treeList->type != NULL_TYPE) {
        ConsCell *poppedList = (ConsCell *)treeList->car;

        // Iterate through the popped tokensList
        while (poppedList->type != NULL_TYPE) {

            // Determine type, and handle it
            if (treeList->car->type == OPEN_TYPE) {
                printf("(");

            } else if (treeList->car->type == CLOSE_TYPE) {
                printf(")");

            } else if (treeList->car->type == CLOSEBRACE_TYPE) {
                printf("}");

            } else if (treeList->car->type == INT_TYPE) {
                Integer *tmp = (Integer *)car;
                printf(" %d ", tmp->value);
                
            } else if (treeList->car->type == DOUBLE_TYPE) {
                Double *tmp = (Double *)car;
                printf(" %f ", tmp->value);

            } else if (treeList->car->type == STR_TYPE) {
                String *tmp = (String *)car;
                printf(" %s ", tmp->value);
                
            } else if (treeList->car->type == SYMBOL_TYPE) {
                Symbol *tmp = (Symbol *)car;
                printf(" %s ", tmp->value);
                
            } else if (treeList->car->type == BOOL_TYPE) {
                Boolean *tmp = (Boolean *)car;
                if (tmp->value == 1) printf(" #t ");
                else printf(" #f ");
            }

            // Move to next poppedList conscell
            poppedList = (ConsCell *)poppedList->cdr;
        }

        // Move to next tree list conscell
        treeList = (ConsCell *)treeList->cdr;
    }
}

#endif
