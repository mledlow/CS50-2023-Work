#include <cs50.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <math.h>

int count_letters(string text);
int count_words(string text);
int count_sentences(string text);

int main(void)
{
    string text = get_string("Text: "); //gets input from user and counts letters, words, and sentences
    int letters = count_letters(text);
    int words = count_words(text);
    int sentences = count_sentences(text);

    double index = round((0.0588 * letters * 100 / words)  - (0.296 * sentences * 100 / words) - 15.8);
    if (index >= 16)
    {
        printf("Grade 16+\n"); //if greater than 16, this is higher than college level
    }
    else if (index < 1)
    {
        printf("Before Grade 1\n"); // if lower than 1, then this is pre grade school
    }
    else
    {
        printf("Grade %i\n", (int) index); //prints calculated index if neither of these
    }
}

int count_letters(string text)
{
    int letterCount = 0;
    for (int i = 0; i < strlen(text); i++)
    {
        if (isalpha(text[i]))
        {
            letterCount++; //if the current place in text is alphabet, increase letter count
        }
    }
    return letterCount;
}

int count_words(string text)
{
    int wordCount = 0;
    for (int i = 0; i < strlen(text); i++)
    {
        if (isblank(text[i]))
        {
            wordCount++; //if a blank space, then we can assume a word has ended, add to count
        }
    }
    return (wordCount + 1); //last word won't end in a space
}

int count_sentences(string text)
{
    int sentenceCount = 0;
    for (int i = 0; i < strlen(text); i++)
    {
        if (text[i] == '.' || text[i] == '?' || text[i] == '!')
        {
            sentenceCount++; //if the character is ending punctuation, then add to sentence count
        }
    }
    return (sentenceCount);
}