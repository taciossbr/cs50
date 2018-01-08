#include <cs50.h>
#include <stdio.h>

int main(void)
{
    printf("Height: ");
    int n = get_int();
    while (n < 0 || n > 23) // verifies that the input is a value between 0 and 23
    {
        printf("Height: ");
        n = get_int();
    }

    for (int i = 0, l = n - 1; i < n; i++, l--)
    {
        for (int j = 0; j < l; j++) // print spaces before "blocks" #
        {
            printf(" ");
        }
        for (int j = 0; j < n - l; j++)
        {
            printf("#"); // prints the first blocks
        }
        printf("  ");
        for (int j = 0; j < n - l; j++)
        {
            printf("#"); // prints the last blocks
        }

        printf("\n");
    }

}