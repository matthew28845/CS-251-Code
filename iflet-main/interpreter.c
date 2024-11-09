#include <stdio.h>
#include <string.h>
#include "object.h"
#include "talloc.h"
#include "linkedlist.h"

// Input:
// Descr: Prints evaluation error and performs texit()
Object *evaluationError() {
    texit(1);

    // Appease the compiler
    return makeNull();
}

// Input inputSymbol: A symbol to look up the value of.
// Input frame: The frame in which to look up the symbol
// Looks up inputSymbol with regards to frame, if it exists in current frame or
// ancestors return the cons cell of that object and what it evaluates to, 
// otherwise return null object.
Object *lookup(Object *inputSymbol, Frame *frame) {
    Symbol *symb = (Symbol *)inputSymbol;
    Object *bindingsCopy = frame->bindings;
    while (bindingsCopy->type != NULL_TYPE) {
        String *firstObjectName = (String *)car(car(bindingsCopy));
        if (strcmp(firstObjectName->value, symb->value) == 0) {
            return car(bindingsCopy);
        }
    }
    if (frame->parent != NULL) {
        lookup(inputSymbol, frame->parent);
    } 
    return makeNull();
}

// Input tree: A cons cell representing the root of the abstract syntax tree for 
// a single Scheme expression (not an entire program).
// Input frame: The frame, with respect to which to perform the evaluation.
// Return: The value of the given expression with respect to the given frame.
Object *eval(Object *tree, Frame *frame) {
    if (tree->type == INT_TYPE) {
        return tree;
    }
    else if (tree->type == DOUBLE_TYPE) {
        return tree;
    }
    else if (tree->type == STR_TYPE) {
        return tree;
    }
    else if (tree->type == BOOL_TYPE) {
        return tree;
    }
    else if (tree->type == SYMBOL_TYPE) {
        lookup(tree, frame);
    }
    else if (tree->type == CONS_TYPE) {
        Symbol *symb = (Symbol *)(car(tree));
        
        if (car(tree)->type == SYMBOL_TYPE && strcmp(symb->value, "if") == 0) {
            //Now we know we are looking at an if statement, so first check for if / cond / true / false, then check for if / cond / true.
            Object *cond = cdr(car(tree));
            Object *ifTrue = cdr(cdr(car(tree)));
            Object *ifFalse = cdr(cdr(cdr(car(tree))));
            if ((cond->type != NULL_TYPE) && (ifTrue->type != NULL_TYPE) && (ifFalse->type != NULL_TYPE)) {
                Boolean *condBool = (Boolean *)eval(cond, frame); //cast as bool to check if false
                if (condBool->value == 0) {
                    return eval(ifFalse, frame);
                }
                else { // any value of the first argument other than the boolean #f is considered true
                    return eval(ifTrue, frame);
                }
            }
            else if ((cond->type != NULL_TYPE) && (ifTrue->type != NULL_TYPE) && (ifFalse->type == NULL_TYPE)) {
                Boolean *condBool = (Boolean *)eval(cond, frame); //cast as bool to check if false
                if (condBool->value == 0) {
                    Object *unspecified;
                    unspecified->type = UNSPECIFIED_TYPE;
                    return unspecified;
                }
                else { // any value of the first argument other than the boolean #f is considered true
                    return eval(ifTrue, frame);
                }       
            }
        }
        else if (car(tree)->type == SYMBOL_TYPE && strcmp(symb->value, "let") == 0) {
            return car(tree);
        }
    }
    return evaluationError();
}

// Input tree: A cons cell representing the root of the abstract syntax tree for 
// a Scheme program (which may contain multiple expressions).
// Evaluates the program, printing the result of each expression in it.
void interpret(Object *tree) {

    //Set up the global frame and add built-in functions to it (none yet)
    Frame *globalFrame;
    globalFrame->parent = NULL;
    globalFrame->bindings = makeNull();

    Object *result = eval(tree, globalFrame);
    switch(result->type) {
        case INT_TYPE: {
            Integer *tmp = (Integer *)result;
            printf(" %d ", tmp->value);
            break;
        }
        case DOUBLE_TYPE: {
            Double *tmp = (Double *)result;
            printf(" %f ", tmp->value);
            break;
        }
        case STR_TYPE: {
            String *tmp = (String *)result;
            printf("\"%s\"", tmp->value);
            break;
        }
        case SYMBOL_TYPE: {
            Symbol *tmp = (Symbol *)result;
            printf(" %s ", tmp->value);
            break;
        }
        case BOOL_TYPE: {
            Boolean *tmp = (Boolean *)result;
            if (tmp->value == 1) printf(" #t ");
            else printf(" #f ");
            break;
        }
        default:
            break;
    }
    return;
}
