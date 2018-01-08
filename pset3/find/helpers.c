/**
 * helpers.c
 *
 * Helper functions for Problem Set 3.
 */

#include <cs50.h>

#include "helpers.h"

/**
 * Returns true if value is in array of n values, else false.
 */
bool search(int value, int values[], int n)
{
    if (n <= 0)
    {
        return false;
    }

    int mi = (n - 1) / 2; // look ate the middle of list
    eprintf("values[mi] = %d\n", values[mi]);
    if (values[mi] == value)
    {
        return true;
    }
    if (values[mi] > value)
    {
        return search(value, values, n / 2); // search in the left side of the list
    }
    return search(value, &values[mi + 1], mi + 1); // search int the right side of the list
}

/**
 * Sorts array of n values.
 */
void sort(int values[], int n)
{
    for (int i = 0; i < n - 1; i++) // walks in the list until the penultime element
    {
        int min = i;
        for (int j = i; j < n; j++) // seachs the minimal element
        {
            if (values[j] < values[min])
            {
                min = j;
            }
        }
        if (i != min) // swap the elements
        {
            int aux = values[min];
            values[min] = values[i];
            values[i] = aux;
        }
        eprintf("values[%d] = %d\n", i, values[i]);
    }
    eprintf("values[%d] = %d\n", n - 1, values[n - 1]);
}
