#include <stdio.h>
//
// Created by Matthew Sigmond on 10/13/24.
//

int sum(int* a, int* b) {
    return *a + *b;
}

int main() {
    int x = 4;
    int y;
    long sumValue = sum(&x, &y);
    printf("%ld",sumValue);
    return 0;
}