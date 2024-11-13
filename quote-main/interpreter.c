#include <stdio.h>
#include <string.h>
#include "object.h"
#include "talloc.h"
#include "linkedlist.h"

// Descr: Prints evaluation error and performs texit()
Object *evaluationError() {
    printf("Evaluation error\n");
    texit(1);

    // Appease the compiler
    return makeNull();
}

// Input inputSymbol: A symbol to look up the value of.
// Input frame: The frame in which to look up the symbol
// Description: Looks up inputSymbol with regards to frame, if it exists in current frame or
// ancestors return the cons cell of that object and what it evaluates to, 
// otherwise return null object.
Object *lookup(Object *inputSymbol, Frame *frame, bool searchParents) {
    Symbol *symb = (Symbol *)inputSymbol;
    Object *bindingsCopy = frame->bindings;

    // Continue searching through the bindings until we find a match, or until all are checked
    while (bindingsCopy->type != NULL_TYPE) {
        // Get the name of the current binding variable for comparison
        String *bVariable = (String *)car(car(bindingsCopy));

        // Check if the current binding matches the input symbol & return if match
        if (strcmp(bVariable->value, symb->value) == 0) {
            return car(bindingsCopy);
        }

        bindingsCopy = cdr(bindingsCopy);
    }
    // If the symbol is not found in the current frame, check the parent frame
    if ((frame->parent != NULL) && (searchParents == true)) {
        return lookup(inputSymbol, frame->parent, true);
    }

    // Variable is not in any frames
    return makeNull();
}

Frame *createFrame(Frame *parent) {
    Frame *newFrame = talloc(sizeof(Frame));
    newFrame->parent = parent;
    newFrame->bindings = makeNull();
    return newFrame;
}

Object *createUnspecified() {
    Object *unspecified = talloc(sizeof(Object));
    unspecified->type = UNSPECIFIED_TYPE;
    return unspecified;
}

void printResult(Object *result) {
    switch(result->type) {
        case INT_TYPE: {
            Integer *tmp = (Integer *)result;
            printf("%d ", tmp->value);
            break;
        }
        case DOUBLE_TYPE: {
            Double *tmp = (Double *)result;
            printf("%f ", tmp->value);
            break;
        }
        case STR_TYPE: {
            String *tmp = (String *)result;
            printf("%s ", tmp->value);
            break;
        }
        case SYMBOL_TYPE: {
            Symbol *tmp = (Symbol *)result;
            printf("%s ", tmp->value);
            break;
        }
        case BOOL_TYPE: {
            Boolean *tmp = (Boolean *)result;
            if (tmp->value == 1) printf("#t ");
            else printf("#f ");
            break;
        }
        case CONS_TYPE: {
            printf("(");
            while (result->type != NULL_TYPE) {
                printResult(car(result));
                result = cdr(result);
            }
            printf(")");
            break;
        }
        case UNSPECIFIED_TYPE: {
            printf("#<unspecified>\n");
            break;
        }
        default:
            break;
    }
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
        Object *binding = lookup(tree, frame, true);
        if (binding->type == NULL_TYPE) return evaluationError();
        return cdr(binding);
    }
    else if (tree->type == CONS_TYPE) {
        Symbol *symb = (Symbol *)(car(tree));
        // Handle if statements
        if (car(tree)->type == SYMBOL_TYPE && strcmp(symb->value, "if") == 0) {
            //Error out if we have less than 3 items
            if (cdr(cdr(tree))->type != CONS_TYPE) {
                return evaluationError();
            }
            else {
                //Now we know we are looking at an if statement, so first check for if / cond / true / false, then check for if / cond / true.
                Object *cond = car(cdr(tree));
                Object *ifTrue = car(cdr(cdr(tree)));
                Object *ifFalse = cdr(cdr(cdr(tree)));
                
                if ((cond->type != NULL_TYPE) && (ifTrue->type != NULL_TYPE) && (ifFalse->type != NULL_TYPE)) {
                    Boolean *condBool = (Boolean *)eval(cond, frame); //cast as bool to check if false

                    if (condBool->value == 0) {
                        return eval(car(ifFalse), frame);
                    }
                    else { // any value of the first argument other than the boolean #f is considered true
                        return eval(ifTrue, frame);
                    }
                }
                else if ((cond->type != NULL_TYPE) && (ifTrue->type != NULL_TYPE) && (ifFalse->type == NULL_TYPE)) {
                    Boolean *condBool = (Boolean *)eval(cond, frame); //cast as bool to check if false
                    if (condBool->value == 0) {
                        printf("creating unspecified 1\n");
                        return createUnspecified();
                    }
                    else { // any value of the first argument other than the boolean #f is considered true
                        return eval(ifTrue, frame);
                    }       
                }
                //we have some weird thing happening
                else return evaluationError();
            }
        }
        // Handle let statements
        else if (car(tree)->type == SYMBOL_TYPE && strcmp(symb->value, "let") == 0) {
            Frame *newFrame = createFrame(frame);

            Object *pairs = car(cdr(tree));
            Object *expressions = cdr(cdr(tree));

            //if this happens we have a let statement with no body expressions
            if (expressions->type == NULL_TYPE) {
                printf("creating unspecified 2\n");
                return createUnspecified();
            }

            //pairs must be a list of pairs, check for it not being a list and it not being of pairs
            if ((pairs->type != CONS_TYPE) && (pairs->type != NULL_TYPE)) {
                return evaluationError();
            }
            if (pairs->type == CONS_TYPE) {
                if (car(pairs)->type == CONS_TYPE) {
                    if (car(car(pairs))->type == NULL_TYPE) return evaluationError();
                    if (cdr(car(pairs))->type == NULL_TYPE) return evaluationError();
                }
                if (car(pairs)->type != NULL_TYPE) {
                    // Handle the (var value) pairs in a let statement
                    while (pairs->type != NULL_TYPE) {
                        Object *pair = car(pairs);
                        //Handle a pair with less than 2 items
                        if (pair->type != CONS_TYPE) {
                            return evaluationError();
                        }
                        //Handle trying to pass in a non-symbol as var
                        if (car(pair)->type != SYMBOL_TYPE) {
                            return evaluationError();
                        }
                        //Handle trying to redefine symbol already defined in let statement
                        if (lookup(car(pair), newFrame, false)->type != NULL_TYPE) {
                            return evaluationError();
                        }

                        Object *evaluatedValue = eval(car(cdr(pair)), frame);
                        Object *newBinding = cons(car(pair), evaluatedValue);

                        // printf("var %u\n", car(pair)->type);
                        // printf("val %u\n", evaluatedValue->type);

                        newFrame->bindings = cons(newBinding, newFrame->bindings);
                        pairs = cdr(pairs);
                    }
                }
            }

            // Handle the expressions in a let statement
            Object *currentExpression;
            if (expressions->type == CONS_TYPE) {
                expressions = reverse(expressions);
                if (cdr(expressions)->type != NULL_TYPE) {
                    Object *currentExpression = eval(car(expressions), newFrame);
                    printResult(currentExpression);
                    expressions = cdr(expressions);
                }
                else {
                    Object *currentExpression = eval(car(expressions), newFrame);
                    return currentExpression;
                }

            }
            else {
                printf("creating unspecified 3\n");
                return createUnspecified();
            }
            return makeNull();
        }
        else if (car(tree)->type == SYMBOL_TYPE && strcmp(symb->value, "quote") == 0) {
            if (cdr(tree)->type == NULL_TYPE) return evaluationError();
            if (cdr(cdr(tree))->type != NULL_TYPE) return evaluationError();
            printResult(car(cdr(tree)));
            return makeNull();
        }
        else return evaluationError();
    }
    else return evaluationError();
}


// Input tree: A cons cell representing the root of the abstract syntax tree for 
// a Scheme program (which may contain multiple expressions).
// Evaluates the program, printing the result of each expression in it.
void interpret(Object *tree) {

    //Set up the global frame and add built-in functions to it (none yet)
    Frame *globalFrame = createFrame(NULL);

    while (tree->type != NULL_TYPE) {
        Object *result = eval(car(tree), globalFrame);
        printResult(result);
        printf("\n");
        tree = cdr(tree);
    }
    
    tfree();    
    return;
}
