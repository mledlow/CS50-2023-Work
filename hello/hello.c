#include <stdio.h>
#include <cs50.h>

int main(void)
{
    string name = get_string("What's your name? "); // gets the string from the user
    printf("hello, %s\n", name);
}