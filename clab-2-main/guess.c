#include <stdlib.h>
#include <stdio.h>

int main(void) {
    printf("Enter a random seed: ");
    int seed;
    scanf("%d", &seed);
    srand(seed);
    int numToGuess = (rand() % 100 + 1);
    int myGuess = -1;
    int guesses = 0;
    while (myGuess != numToGuess) {
        if (guesses > 0) {
            if (myGuess < numToGuess) {
                printf("Too low!\n");
            }
            else {
                printf("Too high!\n");
            }
        }
        printf("Guess a number: ");
        scanf("%d", &myGuess);
        guesses++;
    }
    printf("Correct! Total guesses = %d\n", guesses);
    return 0;
}