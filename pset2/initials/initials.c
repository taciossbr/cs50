#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>

int main(void)
{
    string nome = get_string();

    char iniciais[30];
    // set the first initial
    int len_iniciais = 1;
    iniciais[0] = toupper(nome[0]);
    // traverses the name in search of the remaining initials
    for (int i = 1, n = strlen(nome); i < n; i++)
    {
        if (nome[i] == ' ')
        {
            // set the initial in capital letter
            iniciais[len_iniciais++] = toupper(nome[++i]);
        }
    }
    iniciais[len_iniciais] = '\0';
    printf("%s\n", iniciais);
}