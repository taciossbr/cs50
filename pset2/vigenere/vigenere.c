#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[])
{
    if (argc != 2 ) // exits if user not input a key
    {
        printf("Usage: ./vigenere k\n");
        return 1;
    }
    for (int i = 0, n = strlen(argv[1]); i < n; i++) // walk in the input and exits if find a number
    {
        if (isdigit(argv[1][i]))
        {
            printf("Usage: ./vigenere k\n");
            return 1;
        }
    }
    for (int i = 0, n = strlen(argv[1]); i < n;
         i++) // exits if the string inputed by user contains a space
    {
        if (argv[1][i] == ' ')
        {
            printf("Usage: ./vigenere k\n");
            return 1;
        }
    }

    printf("plaintext: ");
    string p = get_string();
    string key = argv[1];
    for (int i = 0, j = 0, n = strlen(p), l = strlen(key); i < n; i++, j++) // encrypt the string
    {
        int k;
        if (isupper(key[j])) // convert the actual key char in a integer
        {
            k = key[j] - 'A';
        }
        else
        {
            k = key[j] - 'a';
        }

        if (j == l - 1) // performs rotation in case of key overflow
        {
            j = - 1;
        }

        while (p[i] == ' ') // skip for the next char if the actual char is a space
        {
            i++;
        }
        if ('A' <= p[i] && p[i] <= 'Z' )
        {
            if (p[i] + k > 'Z')
            {
                p[i] = 'A' + (p[i] + k - 'Z') - 1; // performs rotation in case of alphabet overflow
            }
            else
            {
                p[i] += k;
            }
        }
        else if ('a' <= p[i] && p[i] <= 'z' )
        {
            if (p[i] + k > 'z')
            {
                p[i] = 'a' + (p[i] + k - 'z') - 1;
            }
            else
            {
                p[i] += k;
            }
        }
    }
    printf("ciphertext: %s\n", p);
}