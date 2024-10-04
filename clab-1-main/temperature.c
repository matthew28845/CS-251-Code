

#include <stdio.h>

int main() {
    printf("Enter a temperature in Fahrenheit:");
    double temp;
    scanf("%lf", &temp);
    if (temp < -459.67)
    {
        printf("Invalid temperature");
        return 0;
    }
    double tempc = (temp - 32) * 5 / 9;
    printf("%f degrees Fahrenheit is %f degrees Celsius.", temp, tempc);
    return 0;
}
