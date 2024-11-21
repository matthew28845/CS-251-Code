#include <stdio.h>
#include <string.h>
#include "object.h"
#include "talloc.h"
#include "linkedlist.h"

// Description: Prints evaluation error and performs texit()
Object *evaluationError(char* str) {
    printf("Evaluation Error: %s\n", str);
    texit(1);
    return makeNull(); // Appease the compiler
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
            printf("Matched symbol. Returning something of type %u", cdr(car(bindingsCopy))->type);
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
        case CLOSURE_TYPE: {
            printf("#<procedure>\n");
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
        case VOID_TYPE: {
            break;
        }
        default:
            break;
    }
}

Object *createPrimitive(char *name, Object*(*pf)()) {
    Primitive *myPrimitive = talloc(sizeof(Primitive));
    myPrimitive->type = PRIMITIVE_TYPE;
    Symbol *primitiveName = talloc(sizeof(Symbol));
    primitiveName->type = SYMBOL_TYPE;
    primitiveName->value = name;
    myPrimitive->pf = pf;

    return cons((Object *) primitiveName, (Object *)myPrimitive);
}

Object *primitiveNull(Object *args) {
    Boolean *myBool = talloc(sizeof(Boolean));
    if (car(args)->type == NULL_TYPE) {
        myBool->value = 0;
        return (Object *)myBool;
    }
    if (cdr(cdr(args))->type != NULL_TYPE) myBool->value = 0;
    myBool->value = 1;
    return (Object *)myBool;
}

Object *primitiveCar(Object *args) {
    if (args->type != CONS_TYPE) return evaluationError("Passing non-cons into car");
    else return car(car(args));
}

Object *primitiveCdr(Object *args) {
    if (args->type != CONS_TYPE) return evaluationError("Passing non-cons into cdr");
    else return cdr(car(args));
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
        printf("Evaluating symbol");
        Object *binding = lookup(tree, frame, true);
        if (binding->type == NULL_TYPE) return evaluationError("Failed to find symbol binding");
        printf("Binding type: %u\n", binding->type); // Type of binding
        printf("Binding car type: %u\n", car(binding)->type); // Should be SYMBOL_TYPE
        printf("Binding cdr type: %u\n", cdr(binding)->type); // Should be PRIMITIVE_TYPE
        return binding;
    }
    else if (tree->type == CONS_TYPE) {
        printf("Evaluating cons, the car's type is %u", car(tree)->type);
        Symbol *symb = (Symbol *)(car(tree));
        // Handle if statements
        if (car(tree)->type == SYMBOL_TYPE && strcmp(symb->value, "if") == 0) {
            //Error out if we have less than 3 items
            if (cdr(cdr(tree))->type != CONS_TYPE) {
                return evaluationError("Less than three arguments in ConsCell");
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
                    if (condBool->value == 0) return createUnspecified();
                    else { // any value of the first argument other than the boolean #f is considered true
                        return eval(ifTrue, frame);
                    }       
                }
                //we have some weird thing happening
                else return evaluationError("Unsure");
            }
        }
        // Handle let statements
        else if (car(tree)->type == SYMBOL_TYPE && strcmp(symb->value, "let") == 0) {
            Frame *newFrame = createFrame(frame);

            Object *pairs = car(cdr(tree));
            Object *expressions = cdr(cdr(tree));

            //if this happens we have a let statement with no body expressions
            if (expressions->type == NULL_TYPE) return createUnspecified();

            //pairs must be a list of pairs, check for it not being a list and it not being of pairs
            if ((pairs->type != CONS_TYPE) && (pairs->type != NULL_TYPE)) {
                return evaluationError("Pairs must be a list of pairs, it's either not a list, or not a pair");
            }
            if (pairs->type == CONS_TYPE) {
                if (car(pairs)->type == CONS_TYPE) {
                    if (car(car(pairs))->type == NULL_TYPE) return evaluationError("The var of the pair is NULL");
                    if (cdr(car(pairs))->type == NULL_TYPE) return evaluationError("The val of the pair is NULL");
                }
                if (car(pairs)->type != NULL_TYPE) {
                    // Handle the (var value) pairs in a let statement
                    while (pairs->type != NULL_TYPE) {
                        Object *pair = car(pairs);
                        //Handle a pair with less than 2 items
                        if (pair->type != CONS_TYPE) {
                            return evaluationError("Pair with less than 2 items");
                        }
                        //Handle trying to pass in a non-symbol as var
                        if (car(pair)->type != SYMBOL_TYPE) {
                            return evaluationError("Can't pass non-symbol as var");
                        }
                        //Handle trying to redefine symbol already defined in let statement
                        if (lookup(car(pair), newFrame, false)->type != NULL_TYPE) {
                            return evaluationError("Can't redefine symbol already defined in let");
                        }

                        // Get value using recursive call that evaluates the var
                        Object *evaluatedValue = eval(car(cdr(pair)), frame);
                        Object *newBinding = cons(car(pair), evaluatedValue);
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
            else return createUnspecified();
            return makeNull();
        }
        //Handle quote
        else if (car(tree)->type == SYMBOL_TYPE && strcmp(symb->value, "quote") == 0) {
            if (cdr(tree)->type == NULL_TYPE) return evaluationError("Second arg for quote is NULL");
            if (cdr(cdr(tree))->type != NULL_TYPE) return evaluationError("Third arg for quote is NULL");
            return car(cdr(tree));
        }
        //Handle define
        else if (car(tree)->type == SYMBOL_TYPE && strcmp(symb->value, "define") == 0) {
            //Handle != 2 args
            if (cdr(tree)->type != CONS_TYPE) return evaluationError("Not enough args");
            if (cdr(cdr(tree))->type == NULL_TYPE) return evaluationError("Second arg for define is NULL");
            if (cdr(cdr(cdr(tree)))->type != NULL_TYPE) return evaluationError("Third arg for define is NULL");
            //handle first arg not being symbol
            if (car(cdr(tree))->type != SYMBOL_TYPE) return evaluationError("First arg for define is not symbol type");

            //make sure we are not defining an object that is already defined
            Object *existingSymbValue = talloc(sizeof(Object));
            existingSymbValue = lookup(car(cdr(tree)), frame, false);
            if (existingSymbValue->type != NULL_TYPE) return evaluationError("Can't redefine an object that is already defined");

            Object *newBinding = cons(car(cdr(tree)), eval(car(cdr(cdr(tree))), frame));
            frame->bindings = cons(newBinding, frame->bindings);
            Object *newVoid = talloc(sizeof(Object));
            newVoid->type = VOID_TYPE;
            return newVoid;
        }
        // Handle lambda
        else if (car(tree)->type == SYMBOL_TYPE && strcmp(((Symbol *)car(tree))->value, "lambda") == 0) {
            if (cdr(tree)->type == NULL_TYPE) return evaluationError("No args following lambda");
            if (car(cdr(tree))->type == NULL_TYPE) return evaluationError("No argument list following parameters");
            if (cdr(cdr(tree))->type == NULL_TYPE) return evaluationError("No code in lambda following parameters");

            Closure *newClosure = talloc(sizeof(Closure));
            newClosure->type = CLOSURE_TYPE;
            newClosure->paramNames = car(cdr(tree)); // arg list
            newClosure->functionCode = cdr(cdr(tree)); // function body
            newClosure->frame = frame;

            // Check for duplicate identifiers in lambda's parameter list
            Object *argList = car(cdr(tree));
            Object *checkedArgs = makeNull();

            while (argList->type == CONS_TYPE && car(argList)->type != NULL_TYPE) {
                Object *param = car(argList);
                // Ensure each parameter is a symbol
                if (param->type != SYMBOL_TYPE) {
                    return evaluationError("Formal parameters for lambda must be symbols");
                }

                // Check for duplicates in the already-processed parameter list
                Object *temp = checkedArgs;
                while (temp->type == CONS_TYPE) {
                    if (strcmp(((Symbol *)car(temp))->value, ((Symbol *)param)->value) == 0) {
                        return evaluationError("Duplicate parameter name in lambda argument list");
                    }
                    temp = cdr(temp);
                }

                // Add parameter to checked list
                checkedArgs = cons(param, checkedArgs);
                argList = cdr(argList);
            }

                return (Object *)newClosure;
        }
        else if (car(tree)->type == CLOSURE_TYPE){
            // Handle closure evaluation
            Object *result = eval(car(tree), frame);
            if (result->type == CLOSURE_TYPE) {
                Closure *inputClosure = (Closure *)result;
                Frame *newFrame = createFrame(inputClosure->frame);

                Object *argList = inputClosure->paramNames;
                Object *argValues = cdr(tree); // Arguments passed to the closure
                Object *expressionList = inputClosure->functionCode;
                Object *evaledExpression = makeNull();

                while (argList->type == CONS_TYPE && argValues->type == CONS_TYPE) {
                    Object *evaledArg = eval(car(argValues), frame);
                    Object *paramVar = car(argList);
                    Object *newBinding = cons(paramVar, evaledArg);

                    newFrame->bindings = cons(newBinding, newFrame->bindings);
                    argList = cdr(argList);
                    argValues = cdr(argValues);
                }

                // Evaluate the closure body in the new frame
                while (expressionList->type == CONS_TYPE) {
                    evaledExpression = eval(car(expressionList), newFrame);
                    expressionList = cdr(expressionList);
                }

                return evaledExpression;
            }
        }
        //Handle primitives
        else {
            Object *evaluatedCar = eval(car(tree), frame);
            Symbol *carName = (Symbol *)car(evaluatedCar);
            printf("%s\n", carName->value);
            Primitive *actualPrimitive = (Primitive *)cdr(evaluatedCar);
            if (actualPrimitive->type != PRIMITIVE_TYPE) {
                return evaluationError("Expected a primitive function");
            }
            Object *result = actualPrimitive->pf(tree);
            return result;
        }
    }
    return evaluationError("Nothing is going to plan :(");
}


// Input tree: A cons cell representing the root of the abstract syntax tree for 
// a Scheme program (which may contain multiple expressions).
// Evaluates the program, printing the result of each expression in it.
void interpret(Object *tree) {

    //Set up the global frame and add built-in functions to it (none yet)
    Frame *globalFrame = createFrame(NULL);

    //Create primitive null?
    Object *null = createPrimitive("null?", primitiveNull);
    globalFrame->bindings = cons(null, globalFrame->bindings);

    //Create primitive car
    Object *primCar = createPrimitive("car", primitiveCar);
    globalFrame->bindings = cons(primCar, globalFrame->bindings);

    //Create primitive cdr
    Object *primCdr = createPrimitive("cdr", primitiveCdr);
    globalFrame->bindings = cons(primCdr, globalFrame->bindings);


    while (tree->type != NULL_TYPE) {
        Object *result = eval(car(tree), globalFrame);
        printResult(result);
        printf("\n");
        tree = cdr(tree);
    }

    tfree();
}
