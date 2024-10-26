#include "linkedlist.h"
#include "linkedlist.c"
#include "talloc.c"
#include "object.h"
#include <stdio.h>

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

bool isInteger(char buffer[], int index) {
    while (!(isWhitespace(buffer[index])) && index != strlen(buffer)) {
        if (!isDigit(buffer[index])) {
            return false;
        }

        index++;
    }

    return true;
}

bool isDecimal(char buffer[], int index) {
    int periodCounter = 0;
    while (buffer[index] != '.' || !(isWhitespace(buffer[index])) && index != strlen(buffer)) {
        if (!isDigit(buffer[index])) {
            return false;
        }
        else if (buffer[index] == '.') {
            periodCounter++;
        }

        index++;
    }

    if (periodCounter > 1) {
        return false;
    }

    return true;
}

bool isString(char buffer[], int index) {
    while (index < strlen(buffer)) {
        char chr = buffer[index];
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

bool isSymbol(char buffer[], int index) {
    if ((buffer[index] == '+' || buffer[index] == '-') && isWhitespace(buffer[index+1])) return true;
    if (isInitial(buffer[index])) {
        while (!(isWhitespace(buffer[index])) && index != strlen(buffer)) {
            if (!isSubsequent(buffer[index])) return false;
        }
        return true;
    }
    return false;
}

Object *getCurrentToken(char buffer[], int* tokenStart) {
    char chr = buffer[*tokenStart];
    Object *token = talloc(sizeof(Object));
    
    // Check if Symbol type
    if (buffer[*tokenStart] == '+' || buffer[*tokenStart] == '-') {
        if (buffer[*tokenStart+1] == '(' || buffer[*tokenStart+1] == ')' || buffer[*tokenStart+1] == '}' ) token->type = SYMBOL_TYPE;
        if (isDigit(buffer[*tokenStart+1]) || buffer[*tokenStart+1] == '.') {
            if (isInteger(buffer, *tokenStart+1)) {
            token->type = INT_TYPE;
        }
            token->type = DOUBLE_TYPE;
        }
    }
    // If token starts with digit then check if Int type / Double type
    else if (isDigit(buffer[*tokenStart]) || buffer[*tokenStart] == '.') {
        if (isInteger(buffer, *tokenStart)) token->type = INT_TYPE;
        token->type = DOUBLE_TYPE;
    }

    // Check if Boolean Type
    else if (buffer[*tokenStart] == '#') {
        if (buffer[*tokenStart+1] == 't' || buffer[*tokenStart+1] == 'f') token->type = BOOL_TYPE;
        printf("Syntax error: Invalid token '%c'", buffer[*tokenStart]);
        texit(1);
    }

    // Check if string
    else if (buffer[*tokenStart] == '"') {
        if (isString(buffer, *tokenStart)) token->type = STR_TYPE;
        printf("Syntax error: Invalid token '%c'", buffer[*tokenStart]);
        texit(1);
    }
    
    //Assume it is a symbol, but still do error checking
    else if (isSymbol(buffer, *tokenStart)) {
        token->type = SYMBOL_TYPE;   
    }

    // If unparsed via specified types check if whitespace character, else invalid syntax
    else {
        if (!isWhitespace(buffer[*tokenStart])) {
            printf("Syntax error: Invalid token '%c'", buffer[*tokenStart]);
            texit(1);            
        }
    }

    // Iterate until whitespace character to get token segment
    char tokenSegment[300];
    
    while (!(isWhitespace(buffer[*tokenStart])) && *tokenStart != strlen(buffer)) {
        *tokenStart+=1;
        chr = buffer[*tokenStart];
        char cToStr[2];
        cToStr[0] = chr;
        cToStr[1] = '\0';
        strcat(tokenSegment, cToStr);
    }
    
    // Convert token segment to its appropriate type
    if (token->type == INT_TYPE) {
        int tmp = atoi(tokenSegment);
        Integer *token = (Integer *)token;
        token->value = tmp;
    }
    else if (token->type == DOUBLE_TYPE) {
        double tmp = (atof(tokenSegment));
        Double *token = (Double *)token;
        token->value = tmp;
    }
    else if (token->type == STR_TYPE) {
        
    }
    return (Object *)token;
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
        // TODO keep track of open parens vs closed
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
        else if (ch == '\n' || ch == '\r' || ch == '\t') {
            index++;
        }
        else if (ch == ';') {
            while (ch != '\n') {
                index++;
            }
        }
        else {
            int* tokenStart = &index;
            getCurrentToken(buffer, tokenStart);
        }
        ch = buffer[index];
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
        printf("%u\n", list->type);
    }
}

#endif


