#include <cs50.h>
#include <stdio.h>

bool check_visa(long creditCardNumber, int numberOfDigits);
bool check_amex(long creditCardNumber);
bool check_mastercard(long creditCardNumber);
int get_credit_card_digit_count(long creditCardNumber);
bool luhn_algorithm(long creditCardNumber, int numberOfDigits);

int main(void)
{
    long creditCardNumber = get_long("Number: "); //gets the user's card input

    if (get_credit_card_digit_count(creditCardNumber) == 0)
    {
        printf("INVALID\n"); //if credit card number digits are "0", then it is invalid
    }
    else if (get_credit_card_digit_count(creditCardNumber) == 13) //is card 13 digits (check for visa)
    {
        if (check_visa(creditCardNumber, get_credit_card_digit_count(creditCardNumber)))
        {
            printf("VISA\n");
        }
        else
        {
            printf("INVALID\n");
        }
    }
    else if (get_credit_card_digit_count(creditCardNumber) == 15) //is card 15 digits (check for amex)
    {
        if (check_amex(creditCardNumber))
        {
            printf("AMEX\n");
        }
        else
        {
            printf("INVALID\n");
        }
    }
    else
    {
        if (check_mastercard(creditCardNumber)) //must be 16 digits at this stage, so check for visa and mastercard
        {
            printf("MASTERCARD\n");
        }
        else if (check_visa(creditCardNumber, get_credit_card_digit_count(creditCardNumber)))
        {
            printf("VISA\n");
        }
        else
        {
            printf("INVALID\n");
        }
    }

    //TODO:based on number of digits, run some checks to see what kind of card we have
    //visa checks for 13 and 16, amex checks for 15, and mastercard checks for 16
}

bool check_visa(long creditCardNumber, int numberOfDigits)
{
    long creditCardNumberDigit = creditCardNumber;
    if (numberOfDigits == 16)
    {
        creditCardNumberDigit /= 1000000000000000;
    } // what this does; take the credit card number, divide it by the number of digits so only the first is left
    else
    {
        creditCardNumberDigit /= 1000000000000;
    }
    if ((creditCardNumberDigit % 10) != 4)
    {
        return false;
    }
    if (luhn_algorithm(creditCardNumber, numberOfDigits))
    {
        return true;
    }
    return false;
}

bool check_amex(long creditCardNumber)
{
    if (((creditCardNumber / 100000000000000) % 10) != 3) //must start with 3
    {
        return false;
    }
    if ((creditCardNumber / 10000000000000) % 10 != 4 && (creditCardNumber / 10000000000000) % 10 != 7) //next digit must be 4 or 7
    {
        return false;
    }
    if (luhn_algorithm(creditCardNumber, 15))
    {
        return true;
    }
    return false;
}

bool check_mastercard(long creditCardNumber)
{
    if ((creditCardNumber / 1000000000000000) % 10 != 5)
    {
        return false;
    }
    if ((creditCardNumber / 100000000000000) % 10 > 5 || (creditCardNumber / 100000000000000) % 10 == 0)
    {
        return false;
    }
    if (luhn_algorithm(creditCardNumber, 16))
    {
        return true;
    }
    return false;
}

int get_credit_card_digit_count(long creditCardNumber)
{
    if (creditCardNumber >= 1000000000000 && creditCardNumber < 10000000000000)
    {
        return 13; //13 digit card
    }
    if (creditCardNumber >= 100000000000000 && creditCardNumber < 1000000000000000)
    {
        return 15; //15 digit card
    }
    if (creditCardNumber >= 1000000000000000 && creditCardNumber < 10000000000000000)
    {
        return 16; // 16 digit card
    }
    return 0;
}

bool luhn_algorithm(long creditCardNumber, int numberOfDigits)
{
    long luhnSum = 0; //what we will check in the end to see if the algo checks out
    long luhnModulus = 0; //the number at the digit place we are looking at; used to sum up
    for (int i = 1; i <= numberOfDigits; i++)
    {
        luhnModulus = creditCardNumber % 10; //getting the modulus at the digit we are at
        if (i % 2 == 0) //if i is even, do this (more complicated blegh)
        {
            luhnModulus *= 2; //multiple the modulus by 2
            if (luhnModulus >= 10)
            {
                luhnSum += (luhnModulus % 10);
                luhnModulus /= 10;
                luhnSum += (luhnModulus % 10); //adds the first and second digit (separately) to the sum
            }
            else
            {
                luhnSum += luhnModulus;
            }
        }
        else
        {
            luhnSum += luhnModulus; //if an odd number of the card, just add modulus to sum (nice and easy)
        }
        creditCardNumber /= 10; //slashes the last digit off of the number, so that we look at a new one
    }

    if ((luhnSum % 10) == 0)
    {
        return true;
    }
    return false;
}