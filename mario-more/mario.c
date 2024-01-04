#include <cs50.h>
#include <stdio.h>

int main(void)
{
    int height = 0;
    do
    {
        height = get_int("Height: ");
    }
    while (height < 1 || height > 8);

    for (int y = 1; y <= height; y++)
    {
        for (int x = 1; x <= height; x++) //first for loop handles the left side of the pyramid
        {
            if ((x + y) > height) //if x + y > height, then blocks start getting placed all the way to the right; more placed as y increases
            {
                printf("#");
            }
            else                         //else, put a space
            {
                printf(" ");
            }
        }
        printf(" ");
        printf(" ");
        for (int z = 1; z <= height; z++)
        {
            if (y >= z) //just a simple way to make a linear downward slope (as y increases, so does number of hashes)
            {
                printf("#");
            }
        }
        printf("\n");
    }
}