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
    }
    else if (tree->type == CONS_TYPE) {
        Symbol *symb = (Symbol *)(car(tree));
        if (car(tree)->type == SYMBOL_TYPE && strcmp(symb->value, "if") == 0) {
            if (eval((cdr(car(tree))->type == BOOL_TYPE)) && (cdr(cdr(car(tree)))->type != NULL_TYPE)))
            if ((strcmp(eval(cdr(car(tree)), frame), "#t")) == 0) {
                return eval(cdr(cdr(car(tree))), frame);
            }
            else if ((strcmp(eval(cdr(car(tree)), frame), "#f")) == 0) {
                return eval(cdr(cdr(cdr(car(tree)))), frame);
            }
            else 
            {
                Object *unspecified;
                unspecified->type = UNSPECIFIED_TYPE;
                return unspecified;
            }
        }
        else if (car(tree)->type == SYMBOL_TYPE && strcmp(symb->value, "if") == 0) {

        }
    }
    else return evaluationError();
}

// Input inputSymbol: A symbol to look up the value of.
// Input frame: The frame in which to look up the symbol
// Looks up inputSymbol with regards to frame, if it exists in current frame or
// ancestors return the cons cell of that object and what it evaluates to, 
// otherwise return null object.
Object *lookup(Object *inputSymbol, Frame *frame) {
    Symbol *symb = (Symbol *)inputSymbol;
    Object *bindingsCopy = &frame->bindings;
    while (bindingsCopy->type != NULL_TYPE) {
        String *firstObjectName = (String *)car(car(bindingsCopy));
        if (strstr(firstObjectName, symb->value) == 0) {
            return car(bindingsCopy);
        }
    }
    if (frame->parent != makeNull()) {
        lookup(inputSymbol, frame->parent);
    } 
    else {
        return makeNull();
    }
}

// Input tree: A cons cell representing the root of the abstract syntax tree for 
// a Scheme program (which may contain multiple expressions).
// Evaluates the program, printing the result of each expression in it.
void interpret(Object *tree) {

    //Set up the global frame and add built-in functions to it (none yet)
    Frame *globalFrame;
    globalFrame->parent = makeNull();
    globalFrame->bindings = cons(makeNull(), makeNull());

    eval(tree, globalFrame);
    return;
}
