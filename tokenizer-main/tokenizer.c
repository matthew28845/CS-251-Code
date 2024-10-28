#include "linkedlist.h"
#include "linkedlist.c"
#include "talloc.c"
#include "object.h"
#include <stdio.h>
#include <strings.h>

#ifndef _TOKENIZER
#define _TOKENIZER

bool isDigit(char chr) {
    if (chr == '1' || chr == '2' || chr == '3' || chr == '4' || chr == '5' || chr == '6' || chr == '7' || chr == '8' || chr == '9' || chr == '0' || chr == '-' || chr == '+') return true;
    return false; 
}

bool isWhitespace(char chr) {
    if (chr == ' ' || chr == '(' || chr == ')' || chr == '}' || chr == '\n' || chr == '\r' || chr == '\t') return true;
    return false;
}

bool isSpecial(char chr) {
    if (chr == '!' || chr == '$' || chr == '%' || chr == '&' || chr == '*' || chr == '/' || chr == ':' || chr == '<' || chr == '=' || chr == '>' || chr == '?' || chr == '~' || chr == '_' || chr == '^') return true;
    return false;
}

bool isLetter(char chr) {
    if ((chr > 100 && chr < 133) || (chr > 140 && chr < 173)) return true;
    return false;
}

bool isInteger(char token[]) {
    int index = 0;
    while (index < strlen(token)) {
        if (!isDigit(token[index])) {
            return false;
        }
        index++;
    }

    return true;
}

bool isDecimal(char token[]) {
    int index = 0;
    while ((token[index] != '.' || !(isWhitespace(token[index]))) && index != strlen(token)) {
        if (!isDigit(token[index])) {
            return false;
        }
        index++;
    }

    if ((strstr(".", token)) != NULL) {
        return true;
    }

    return false;
}

bool isString(char token[]) {
    int index = 0;
    while (index < strlen(token)) {
        char chr = token[index];
        if (chr == '"') return true;
        index++;
    }
    return false;
}

bool isInitial(char chr) {
    if (isLetter(chr) || isSpecial(chr)) return true;
    return false;
}

bool isSubsequent(char chr) {
    if (isInitial(chr) || isDigit(chr) || chr == '.' || chr == '+' || chr == '-') return true;
    return false;
}

bool isSymbol(char token[]) {
    int index = 0;
    if (strlen(token) < 2) {
        if (isInitial(token[0])) return true;
        return false;
    }
    if ((token[index] == '+' || token[index] == '-') && isWhitespace(token[index+1])) return true;
    if (isInitial(token[index])) {
        while (!(isWhitespace(token[index])) && index != strlen(token)) {
            if (!isSubsequent(token[index])) return false;
        }
        return true;
    }
    return false;
}

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
            assert(newCar != NULL);
            newCar->type = OPEN_TYPE;
            list = cons(newCar, list);
        }   
        else if (ch == ')') {
            Object *newCar = talloc(sizeof(Object));
            assert(newCar != NULL);
            newCar->type = CLOSE_TYPE;
            list = cons(newCar, list);
        }
        else if (ch == '}') {
            Object *newCar = talloc(sizeof(Object));
            assert(newCar != NULL);
            newCar->type = CLOSEBRACE_TYPE;
            list = cons(newCar, list);
        }
        else if (!(ch == '\n' || ch == '\r' || ch == '\t' || ch == ';' || ch == ' ')) {
            char currentToken[301];
            int tokenIndex = 0;

            // Get token segment
            while (!(isWhitespace(ch))) {
                currentToken[tokenIndex] = ch;
                tokenIndex++;
                buffer[index] = ch;
                index++;
                ch = fgetc(stdin);
            }
            currentToken[tokenIndex] = '\0';
            if (currentToken[0] == '+' || currentToken[0] == '-') {
                    if (currentToken[1] == '(' || currentToken[1] == ')' || currentToken[1] == '}' ) type = SYMBOL_TYPE;
                    
                    if (isDigit(currentToken[1]) || currentToken[1] == '.') {
                        if (isInteger(currentToken)) type = INT_TYPE;
                        else type = DOUBLE_TYPE;
                    }
            }
            // If token starts with digit then check if Int type / Double type
            else if (isDigit(currentToken[0]) || currentToken[0] == '.') {
                if (isInteger(currentToken)) type = INT_TYPE;
                else type = DOUBLE_TYPE;
            }

            // Check if Boolean Type
            else if (currentToken[0] == '#') {
                if (currentToken[1] == 't' || currentToken[1] == 'f') type = BOOL_TYPE;
                else {
                    printf("Syntax error checking if boolean: Invalid token '%s'\n", currentToken);
                    texit(4);
                }
            }

            // Check if string
            else if (currentToken[0] == '"') {
                if (isString(currentToken)) type = STR_TYPE;
                else {
                    printf("Syntax error checking if string: Invalid token '%s'\n", currentToken);
                    texit(2);
                }
            }
            
            //Assume it is a symbol, but still do error checking
            else if (isSymbol(currentToken)) {
                type = SYMBOL_TYPE;   
            }

            // If unparsed via specified types check if whitespace character, else invalid syntax
            else {
                if (!isWhitespace(currentToken[0])) {
                    printf("Syntax error checking if whitespace: Invalid token '%s'\n", currentToken);
                    texit(3);            
                }
            }
            
            // Convert current token to its appropriate type, cons onto list
            if (type == INT_TYPE) {
                int tmp = atoi(currentToken);
                Integer *newCar = talloc(sizeof(Integer));
                assert(newCar != NULL);
                newCar->type = INT_TYPE;
                newCar->value = tmp;
                
                list = cons((Object *)newCar, list);
            }
            else if (type == DOUBLE_TYPE) {
                double tmp = (atof(currentToken));
                Double *newCar = talloc(sizeof(Double));
                assert(newCar != NULL);
                newCar->type = DOUBLE_TYPE;
                newCar->value = tmp;

                list = cons((Object *)newCar, list);
            }
            else if (type == STR_TYPE) {
                String *newCar = talloc(sizeof(String));
                assert(newCar != NULL);
                newCar->type = STR_TYPE;
                char *tmp = talloc(301);
                strcpy(tmp, currentToken);
                newCar->value = tmp;

                list = cons((Object *)newCar, list);
            }
            else if (type == SYMBOL_TYPE) {
                Symbol *newCar = talloc(sizeof(Symbol));
                assert(newCar != NULL);
                newCar->type = SYMBOL_TYPE;
                char *tmp = talloc(301);
                strcpy(tmp, currentToken);
                newCar->value = tmp;

                list = cons((Object *)newCar, list);
            }
            else if (type == BOOL_TYPE) {
                Boolean *newCar = talloc(sizeof(Boolean));
                assert(newCar != NULL);
                newCar->type = BOOL_TYPE;
                if (strcmp(currentToken, "#f") == 0) newCar->value = false;
                else newCar->value = true;
                list = cons((Object *)newCar, list);  
            }
        }
        else if (ch == ';') {
            while (ch != '\n') {
                buffer[index] = ch;
                index++;
                ch = fgetc(stdin);                
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
        ConsCell *listCons = (ConsCell *)list;
        
        Object *car = listCons->car;

        if (car->type == OPEN_TYPE) {
            Object *tmp = (Object *)car;
            printf("(:open\n");

        } else if (car->type == CLOSE_TYPE) {
            Object *tmp = (Object *)car;
            printf("):close\n");

        } else if (car->type == CLOSEBRACE_TYPE) {
            Object *tmp = (Object *)car;
            printf("}:closebrace\n");

        } else if (car->type == INT_TYPE) {
            Integer *tmp = (Integer *)car;
            printf("%d:integer\n", tmp->value);
            
        } else if (car->type == DOUBLE_TYPE) {
            Double *tmp = (Double *)car;
            printf("%f:double\n", tmp->value);

        } else if (car->type == STR_TYPE) {
            String *tmp = (String *)car;
            printf("%s:string\n", tmp->value);
            
        } else if (car->type == SYMBOL_TYPE) {
            Symbol *tmp = (Symbol *)car;
            printf("%s:symbol\n", tmp->value);
            
        } else if (car->type == BOOL_TYPE) {
            Boolean *tmp = (Boolean *)car;
            if (tmp->value == 1) printf("#t:boolean\n");
            else printf("#f:boolean\n");
        }

        list = (Object *)listCons->cdr;
    }
}

#endif