#include "object.h"
#include "linkedlist.h"
#include "talloc.h"
#include <stddef.h>
#include <stdio.h>

#ifndef _PARSER
#define _PARSER

// Input tokens: A linked list of tokens. The output of the tokenize function.
// Return: A linked list that stores the abstract syntax tree (forest, actually)
// for that token list. If a syntax error is encountered, then an error message
// is printed and the program cleanly exits.
Object *parse(Object *tokens) {
    Object *tokensStack = makeNull();
    int numUnclosedOpens = 0;

    while (tokens->type != NULL_TYPE) {
        // Syntax error handling for open and closed parens
        if (car(tokens)->type == OPEN_TYPE && tokens->type == CONS_TYPE) {
            numUnclosedOpens++;
        }

        if (car(tokens)->type == CLOSE_TYPE && tokens->type == CONS_TYPE) {
            numUnclosedOpens--;

            if (numUnclosedOpens < 0) {
                printf("Syntax error: too many close parentheses\n");
                texit(1);
            }
        }

        // While tokens list item is not close type, continue to pop from tokens list and prepend to tokenStack
        if (car(tokens)->type != CLOSE_TYPE && car(tokens)->type != CLOSEBRACE_TYPE) {
            tokensStack = cons(car(tokens), tokensStack);

        // Handle closetype
        } else if (car(tokens)->type == CLOSE_TYPE) {
            Object *poppedList = makeNull();

            if (car(tokensStack)->type == OPEN_TYPE) {
                poppedList = cons(makeNull(), makeNull());
            }
            
            // While not open type, continue to pop from tokensStack and prepend to poppedList
            while (tokensStack->type != NULL_TYPE && car(tokensStack)->type != OPEN_TYPE) {
                poppedList = cons(car(tokensStack), poppedList);
                tokensStack = cdr(tokensStack);
            }

            tokensStack = cons(poppedList, cdr(tokensStack));

        // If closebrace, iteratively decrease numUnclosedOpens until 0 while popping from tokenStack and prepending to poppedList
        // ================================================================== TODO FIXXXX ==================================================================
        } else if (car(tokens)->type == CLOSEBRACE_TYPE) {
            
            while (numUnclosedOpens > 0) {
                Object *poppedList = makeNull();
                
                if (car(tokensStack)->type == OPEN_TYPE) {
                    poppedList = cons(makeNull(), makeNull());
                
                // While not open type, continue to pop from tokensStack and prepend to poppedList
                } 
                
                while (tokensStack->type != NULL_TYPE && car(tokensStack)->type != OPEN_TYPE) {
                    poppedList = cons(car(tokensStack), poppedList);
                    tokensStack = cdr(tokensStack);
                }

                tokensStack = cons(poppedList, cdr(tokensStack));
                numUnclosedOpens--;
            }
            
        }

        // Move to next item in tokens list
        tokens = cdr(tokens);
    }
    
    // Handle Errors
    if (numUnclosedOpens > 0) {
        printf("Syntax error: not enough close parentheses\n");
        texit(1);
    }
    if (numUnclosedOpens < 0) {
        printf("Syntax error: too many close parentheses\n");
        texit(1);
    }

    return reverse(tokensStack);
}

// Input tree: An abstract syntax tree (forest). The output of parse.
// Prints the tree in a human-readable format that closely resembles the
// original Scheme code that led to the abstract syntax tree.
// ================================================================== TODO FIXXXX ==================================================================
void printTree(Object *tree) {
    if (tree->type == CONS_TYPE && car(tree)->type == CONS_TYPE) {
        printf("(");
        printTree(car(tree));
        printf(") ");

    } else if (tree->type == CONS_TYPE) {
        Object *poppedItem = car(tree);

        switch(poppedItem->type) {
            case INT_TYPE: {
                Integer *tmp = (Integer *)poppedItem;
                printf(" %d ", tmp->value);
                break;
            }
            case DOUBLE_TYPE: {
                Double *tmp = (Double *)poppedItem;
                printf(" %f ", tmp->value);
                break;
            }
            case STR_TYPE: {
                String *tmp = (String *)poppedItem;
                printf("\"%s\"", tmp->value);
                break;
            }
            case SYMBOL_TYPE: {
                Symbol *tmp = (Symbol *)poppedItem;
                printf(" %s ", tmp->value);
                break;
            }
            case BOOL_TYPE: {
                Boolean *tmp = (Boolean *)poppedItem;
                if (tmp->value == 1) printf(" #t ");
                else printf(" #f ");
                break;
            }
            default:
                break;
        }
    }   

    if (tree->type == CONS_TYPE) printTree(cdr(tree));
}

#endif
