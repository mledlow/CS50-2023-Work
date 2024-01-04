#include <cs50.h>
#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

int convert(string input);

int main(void)
{
    string input = get_string("Enter a positive integer: ");

    for (int i = 0, n = strlen(input); i < n; i++)
    {
        if (!isdigit(input[i]))
        {
            printf("Invalid Input!\n");
            return 1;
        }
    }

    // Convert string to int
    int someChar = convert(input);
    printf("\n");
}

int convert(string input)
{
    if (strlen(input) < 1)
    {
        return input[0];
    }
    char lastChar = input[strlen(input) - 1] - 48; //gets the last character of the string as its number
    input[strlen(input) - 1] = '\0';
    int nextChar = convert(input);
    printf("%i", (int) lastChar);

    return lastChar;
}