


/* Program demonstrating pointers in C. */

#include <stdio.h>

int main() {
    int a = 5;
    int b = 6;
    if (&a < &b) {
        printf("higher\n");
    }
    else {
        printf("lower\n");
    }
    // p and q are pointers to a and b, respectively
    int *p = &a;
    int *q = &b;
    // pp is a pointer to p
    int **pp = &p;
    // Adds a's value to b (b=11)
    *q += *p;
    // Increments a's value (a=6)
    (**pp) += 1;
    // Now q will point to a and p will point to b
    q = p;
    *pp = &b;
    // Adds 3 to b (b=14)
    *p += 3;
    // Doubles a (a=12)
    *q *= 2;
    // Should print 12 and 14
    printf("Final values of a and b: %i, %i\n", a, b);
}


