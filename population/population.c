#include <cs50.h>
#include <stdio.h>

int getStartSize(void);
int getEndSize(int startSize);
int getYearsToReturn(int startSize, int endSize);

int main(void)
{
    int startSize = getStartSize();

    int endSize = getEndSize(startSize);

    int yearsToReturn = getYearsToReturn(startSize, endSize);

    printf("Years: %i\n", yearsToReturn);
}

int getStartSize(void)
{
    int n; //gets a target start size that must be greater than 8
    do
    {
        n = get_int("Start size: ");
    }
    while (n < 9);

    return n;
}

int getEndSize(int startSize)
{
    int n; //gets a target ending size that must be less than the start size
    do
    {
        n = get_int("End size: ");
    }
    while (n < startSize);

    return n;
}

int getYearsToReturn(int startSize, int endSize)
{
    for (int y = 0; true; y++) //for loop that increments the year with each loop
    {
        if (startSize >= endSize) //startSize repurposed into currentSize since it's only needed for one loop
        {
            return y; //returns this first so that 0 years case is handled properly
        }
        startSize = startSize + startSize / 3 - startSize / 4; //modifies startSize by adding (it / 3) and subtracting (it / 4)
    }
}