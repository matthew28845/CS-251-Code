//
// Created by Matthew Sigmond on 10/7/24.
//

#include <stdio.h>

struct Complex {
    double real;
    double imaginary;

};

int main() {
    struct Complex c1;
    struct Complex c2;
    struct Complex c3;
    printf("Enter real part of c1: ");
    scanf("%lf",&c1.real);
    printf("Enter imaginary part of c1: ");
    scanf("%lf",&c1.imaginary);
    printf("Enter real part of c2: ");
    scanf("%lf",&c2.real);
    printf("Enter imaginary part of c2: ");
    scanf("%lf",&c2.imaginary);
    c3.real = ((c1.real*c2.real)-(c1.imaginary*c2.imaginary));
    c3.imaginary = ((c1.real*c2.imaginary)+(c1.imaginary*c2.real));
    printf("Answer = %10.2f + %10.2f i\n", c3.real, c3.imaginary);
    return 0;
}
