#include <cs50.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[])
{
    if (argc != 2) // exits if user not input a key
    {
        printf("Usage: ./caesar k\n");
        return 1;
    }
    int k = atoi(argv[1]) % 26; // convert the key to integet
    printf("plaintext: ");
    string p = get_string();
    for (int i = 0, n = strlen(p); i < n; i++) // encrypt the string
    {
        if ('A' <= p[i] && p[i] <= 'Z' )
        {
            if (p[i] + k > 'Z')
            {
                p[i] = 'A' + (p[i] + k - 'Z') - 1; // performs rotation in case of overflow
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