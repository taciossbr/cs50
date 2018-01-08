#include <cs50.h>
#include <stdio.h>

int main(void)
{
    int m, b;
    printf("Minutes: "); // read the minutes
    m = get_int();
    b = m * 12; // calcule the bottles
    printf("Bottles: %d\n", b); // print the result
}