// Check that a password has at least one lowercase letter, uppercase letter, number and symbol
// Practice iterating through a string
// Practice using the ctype library

#include <cs50.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>

bool valid(string password);

int main(void)
{
    string password = get_string("Enter your password: ");
    if (valid(password))
    {
        printf("Your password is valid!\n");
    }
    else
    {
        printf("Your password needs at least one uppercase letter, lowercase letter, number and symbol\n");
    }
}

bool valid(string password)
{
    bool hasLowerCase = false;
    bool hasUpperCase = false;
    bool hasNumber = false;
    bool hasSymbol = false;
    for (int i = 0; i < strlen(password); i++)
    {
        if (islower(password[i]))
        {
            hasLowerCase = true; // if lowercase, set this bool to true
        }
        else if (isupper(password[i]))
        {
            hasUpperCase = true; // if uppercase, set this bool to true
        }
        else if (isalnum(password[i]))
        {
            hasNumber = true; // if not a letter but still alphanumeric, then we have number, set to true
        }
        else
        {
            hasSymbol = true;
        }
    }
    if (hasLowerCase && hasUpperCase && hasNumber && hasSymbol)
    {
        return true;
    }
    return false;
}
