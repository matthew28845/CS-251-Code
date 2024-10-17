
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "object.h"
#include <assert.h>

#ifndef _LINKEDLIST
#define _LINKEDLIST

// Return: A newly allocated Object of NULL_TYPE.
Object *makeNull() {
    Object *newObject = malloc(sizeof(Object));
    assert(newObject != NULL);
    newObject->type = NULL_TYPE;
    return newObject;
}

// Input: Value to insert into newly created integer object
// Return: A newly allocated Integer of INT_TYPE.
Object *makeInt(int *newVal) {
    Integer *newObject = malloc(sizeof(Integer));
    assert(newObject != NULL);
    newObject->type = INT_TYPE;
    newObject->value = *newVal;
    return (Object *)newObject;
}

// Input: Value to insert into newly created double object
// Return: A newly allocated Double of DOUBLE_TYPE.
Object *makeDbl(double *newVal) {
    Double *newObject = malloc(sizeof(Double));
    assert(newObject != NULL);
    newObject->type = DOUBLE_TYPE;
    newObject->value = *newVal;
    return (Object *)newObject;
}

// Input: Value to insert into newly created string object
// Return: A newly allocated Double of STR_TYPE.
Object *makeStr(char *newVal) {
    Integer *newObject = malloc(sizeof(String));
    assert(newObject != NULL);
    newObject->type = STR_TYPE;
    newObject->value = *newVal;
    return (Object *)newObject;
}

// Input newCar: An instance of Object or one of its subclasses.
// Input newCdr: An instance of Object or one of its subclasses.
// Return: A newly allocated ConsCell object with that car and cdr.
Object *cons(Object *newCar, Object *newCdr) {
    ConsCell *newCons = malloc(sizeof(ConsCell));
    assert(newCons != NULL);
    newCons->car = newCar;
    newCons->cdr = newCdr;
    newCons->type = CONS_TYPE;
    return (Object *)newCons;
}

// Input list: A ConsCell that is the head of a list.
// Prints the entire list in a human-readable way.
void display(Object *list) {
    if (list->type == CONS_TYPE) {
        ConsCell *cons = (ConsCell *)list;
        while (cons->car != NULL) {
            if (cons->car->type == INT_TYPE) {
                Integer *integer = (Integer *)cons->car;
                printf("%i\n",integer->value);
            } else if (cons->car->type == DOUBLE_TYPE) {
                Double *doub = (Double *)cons->car;
                printf("%f\n",doub->value);
            } else if (cons->car->type == STR_TYPE) {
                String *str = (String *)cons->car;
                printf("%s\n",str->value);
            }
            cons = (ConsCell *)cons->cdr;
        }
        printf("\n");
    }else {
        printf("You didn't give me a cons cell. Now die.\n");
    }
}

// Input list: A ConsCell that is the head of a list.
// Return: A ConsCell that is the head of a list. The new list is the reverse of 
// the given list. All content within the list is duplicated; there is no shared 
// memory whatsoever between the given list and the new one.
Object *reverse(Object *list) {
    if (list->type == CONS_TYPE) {
        ConsCell *headptr = (ConsCell *)list;
        Object *revptr = makeNull();
        while (headptr->type != NULL_TYPE) {
            if (headptr->car->type == INT_TYPE) {
                Integer *currentCar = (Integer *)headptr->car;
                Integer *newCar = (Integer *)makeInt(&currentCar->value);
                ConsCell *reverseItem = (ConsCell *)cons((Object *)newCar, revptr);
                revptr = (Object *)reverseItem;
                headptr = (ConsCell *)headptr->cdr;
            }            
            else if (headptr->car->type == DOUBLE_TYPE) {
                Double *currentCar = (Double *)headptr->car;
                Double *newCar = (Double *)makeDbl(&currentCar->value);
                ConsCell *reverseItem = (ConsCell *)cons((Object *)newCar, revptr);
                revptr = (Object *)reverseItem;
                headptr = (ConsCell *)headptr->cdr;
            }
            else if (headptr->car->type == STR_TYPE) {
                String *currentCar = (String *)headptr->car;
                String *newCar = (String *)makeStr(currentCar->value);
                ConsCell *reverseItem = (ConsCell *)cons((Object *)newCar, revptr);
                revptr = (Object *)reverseItem;
                headptr = (ConsCell *)headptr->cdr;
            }
            else {
                Object *newCar = makeNull();
                ConsCell *reverseItem = (ConsCell *)cons((Object *)newCar, revptr);
                revptr = (Object *)reverseItem;
                headptr = (ConsCell *)headptr->cdr;
            }
        }
        return (Object *)revptr;
    }
    else {
        return makeNull();
    }
}

// Input list: A ConsCell that is the head of a list.
// Frees all memory directly or indirectly referred to by the given list.
void cleanup(Object *list) {
    while (list->type == CONS_TYPE) {
        ConsCell *cons = (ConsCell *)list;
        if (cons->car->type == STR_TYPE) {
            String *str = (String *)cons->car;
            free(str->value);
        }
        free(cons->car);
        Object *nextCons = cons->cdr;
        free(cons);
        list = nextCons;
    }
}

// Input list: A ConsCell.
// Return: The car of that ConsCell.
// This is a convenience function to slightly accelerate taking cars of objects 
// known to be cons cells.
Object *car(Object *list) {
    assert(list->type == CONS_TYPE);
    ConsCell *cons = (ConsCell *)list;
    return cons->car;
}

// Input list: A ConsCell.
// Return: The cdr of that ConsCell.
// This is a convenience function to slightly accelerate taking cars of objects 
// known to be cons cells 
Object *cdr(Object *list) {
    assert(list->type == CONS_TYPE);
    ConsCell *cons = (ConsCell *)list;
    return cons->cdr;
}

// Input list: Any object.
// Return: A Boolean indicating whether that object is of NULL_TYPE.
bool isNull(Object *value) {
    return (value->type == NULL_TYPE);
}

// Input value: A ConsCell that is the head of a list.
// Return: The length of the list. For example, the Scheme list () has length 0, 
// and the Scheme list (7) has length 1.
int length(Object *value) {
    if (value->type == CONS_TYPE) {
        ConsCell *cons = (ConsCell *)value;
        int length = 0;
        while (cons->type != NULL_TYPE) {
            length++;
            cons = (ConsCell *)cons->cdr;
        }
        return length;
    }else {
        return 0;
    }
}

#endif


