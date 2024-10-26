

#include "linkedlist.h"
#include "talloc.c"
#include "object.h"
#include <stdio.h>

#ifndef _TOKENIZER
#define _TOKENIZER

// Return: A cons cell that is the head of a list. The list consists of the 
// tokens read from standard input (stdin).
Object *tokenize() {
    int ch;                       // int, not char; see fgetc documentation
    char buffer[300 + 1];         // based on 300-char limit plus terminating \0
    int index = 0;                // where in buffer to place the next char read
    objectType type = NULL_TYPE;  // type of token being built in buffer
    Object *list = makeNull();
    ch = fgetc(stdin);
    while (ch != EOF) {
        //For these three types, we know what they are based on the first character, and can just deal with them simply
        if (ch == '(') {
            Object *newCar = talloc(sizeof(Object));
            newCar->type = OPEN_TYPE;
            list = cons(newCar, list);
        }   
        else if (ch == ')') {
            Object *newCar = talloc(sizeof(Object));
            newCar->type = CLOSE_TYPE;
            list = cons(newCar, list);
        }
        else if (ch == '}') {
            Object *newCar = talloc(sizeof(Object));
            newCar->type = CLOSEBRACE_TYPE;
            list = cons(newCar, list);
        }
        else {
            String *currentThing = talloc(sizeof(STR_TYPE));
            currentThing->value = "";
            while (ch != (' ' || '(' || ')' || '}')) {
                strcat(currentThing->value, ch);
            }
            // Check to see if current is a double via first char being "."
            if (currentThing->value[0] = '.') {
                Double *newCar = talloc(sizeof(Double));
                char* end;
                newCar->value = strtod(currentThing, &end);
                list = cons(newCar, list);
            }
            // Check to see if it's a double or int by traversing ints until period (double) or end
            else if (currentThing->value[0] = ('1' || '2' || '3' || '4' || '5' || '6' || '7' || '8' || '9' || '0' || '-' || '+')) {
                if (strstr(currentThing->value, ".") != NULL) {
                    Double *newCar = talloc(sizeof(Double));
                    char* end;
                    newCar->value = strtod(currentThing, &end);
                    list = cons(newCar, list);
                }
                else {
                    Integer *newCar = talloc(sizeof(Integer));
                    char* end;
                    newCar->value = strtod(currentThing, &end);
                    list = cons(newCar, list);                    
                }
            }
        }
        buffer[index] = ch;
        index++;
        ch = fgetc(stdin);
    }
    return reverse(list);
}

// Input list: A list of tokens, as returned from the tokenize function.
// Prints the tokens, one per line with type annotation, as exemplified in the 
// assignment.
void displayTokens(Object *list) {

    while (list->type != NULL_TYPE) {
        printf("", list->type);
    }
}

#endif


