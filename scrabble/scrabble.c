#include <ctype.h>
#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Points assigned to each letter of the alphabet
int POINTS[] = {1, 3, 3, 2, 1, 4, 2, 4, 1, 8, 5, 1, 3, 1, 1, 3, 10, 1, 1, 1, 1, 4, 4, 8, 4, 10};

int compute_score(string word);

int main(void)
{
    // Get input words from both players
    string word1 = get_string("Player 1: ");
    string word2 = get_string("Player 2: ");

    // Score both words
    int score1 = compute_score(word1);
    int score2 = compute_score(word2);

    if (score1 > score2) // prints the winner of the game, or a tie if there is one
    {
        printf("Player 1 wins!\n");
    }
    else if (score1 < score2)
    {
        printf("Player 2 wins!\n");
    }
    else
    {
        printf("Tie!\n");
    }
}

int compute_score(string word)
{
    char currentChar = 0;
    int wordScore = 0;
    for (int i = 0; i < strlen(word); i++)
    {
        currentChar = word[i]; //gets the character at the current place in the word
        if (islower(currentChar))
        {
            currentChar -= 32; //if lowercase, convert to uppercase first
        }
        if (isupper(currentChar))
        {
            currentChar -= 65; //if uppercase, then shift down to 65 to match the array's inputs for A, B, C, etc
            wordScore += POINTS[(int) currentChar]; // add the value at that position to the word score
        }
    }

    return wordScore; //return the word's score
}
