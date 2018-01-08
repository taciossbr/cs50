#include <cs50.h>
#include <stdio.h>

int main(void)
{
    printf("O hai! How much change is owed?\n");
    double n = get_double();
    while (n < 0) // ensures that the input will be greater than 0
    {
        printf("O hai! How much change is owed?\n");
        double n = get_double();
    }


    int m = 0;
    int in = n * 100; // multiplies by 100 and converts to integer to be able to use the modulo operator

    m += in / 25;
    in %= 25;

    m += in / 10;
    in %= 10;

    m += in / 5;
    in %= 5;

    m += in;

    printf("%d\n", m); // print the result



}