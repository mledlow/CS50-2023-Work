#include <cs50.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

// each of our text files contains 1000 words
#define LISTSIZE 1000

// values for colors and score (EXACT == right letter, right place; CLOSE == right letter, wrong place; WRONG == wrong letter)
#define EXACT 2
#define CLOSE 1
#define WRONG 0

// ANSI color codes for boxed in letters
#define GREEN   "\e[38;2;255;255;255;1m\e[48;2;106;170;100;1m"
#define YELLOW  "\e[38;2;255;255;255;1m\e[48;2;201;180;88;1m"
#define RED     "\e[38;2;255;255;255;1m\e[48;2;220;20;60;1m"
#define RESET   "\e[0;39m"

// user-defined function prototypes
string get_guess(int wordsize);
int check_word(string guess, int wordsize, int status[], string choice);
void print_word(string guess, int wordsize, int status[]);

int main(int argc, string argv[])
{
    if (argc != 2)
    {
        printf("Usage: ./wordle wordsize\n"); //makes sure correct number of args is inputted
        return 1;
    }

    if ((argv[1][0] != '5' && argv[1][0] != '6' && argv[1][0] != '7' && argv[1][0] != '8') || strlen(argv[1]) > 1)
    {
        printf("Error: wordsize must be either 5, 6, 7, or 8\n"); //makes sure correct word size is inputted
        return 1;
    }

    int wordsize = argv[1][0] - 48; // shifts it by 48 to get to the number value for the character

    // open correct file, each file has exactly LISTSIZE words
    char wl_filename[6];
    sprintf(wl_filename, "%i.txt", wordsize);
    FILE *wordlist = fopen(wl_filename, "r");
    if (wordlist == NULL)
    {
        printf("Error opening file %s.\n", wl_filename);
        return 1;
    }

    // load word file into an array of size LISTSIZE
    char options[LISTSIZE][wordsize + 1];

    for (int i = 0; i < LISTSIZE; i++)
    {
        fscanf(wordlist, "%s", options[i]);
    }

    // pseudorandomly select a word for this game
    srand(time(NULL));
    string choice = options[rand() % LISTSIZE];

    // allow one more guess than the length of the word
    int guesses = wordsize + 1;
    bool won = false;

    // print greeting, using ANSI color codes to demonstrate
    printf(GREEN"This is WORDLE50"RESET"\n");
    printf("You have %i tries to guess the %i-letter word I'm thinking of\n", guesses, wordsize);

    // main game loop, one iteration for each guess
    for (int i = 0; i < guesses; i++)
    {
        // obtain user's guess
        string guess = get_guess(wordsize);

        // array to hold guess status, initially set to zero
        int status[wordsize];

        for (int j = 0; j < wordsize; j++)
        {
            status[j] = 0; //initializes all of status to be 0 initially
        }

        // Calculate score for the guess
        int score = check_word(guess, wordsize, status, choice);

        printf("Guess %i: ", i + 1);

        // Print the guess
        print_word(guess, wordsize, status);

        // if they guessed it exactly right, set terminate loop
        if (score == EXACT * wordsize)
        {
            won = true;
            break;
        }
    }

    if (won == true)
    {
        printf("You won!\n");
    }
    else
    {
        printf("The word was: %s\n", choice);
    }

    // that's all folks!
    return 0;
}

string get_guess(int wordsize)
{
    string guess = "";

    do
    {
        guess = get_string("Input a %i-letter word: ", wordsize); //prompts user for a guess until it matches the wordsize
    }
    while (strlen(guess) != wordsize);

    return guess;
}

int check_word(string guess, int wordsize, int status[], string choice)
{
    int score = 0;

    for (int i = 0; i < wordsize; i++) // for loop to iterate through guess
    {
        for (int j = 0; j < wordsize; j++) // for loop to iterate through choice
        {
            if (guess[i] == choice[j]) //if the char at guess[i] equals the char at choice[j]
            {
                if (i == j) //if they are at the same place in the word
                {
                    status[j] = 2;
                    score += 2; //sets the place in the choice to be correct and adds two to score
                    break; //breaks so as not to check
                }
                else
                {
                    status[j] = 1;
                    score += 1; //sets the letter to be "close" and adds one to score
                }
            }
        }
    }

    return score;
}

void print_word(string guess, int wordsize, int status[])
{

    for (int i = 0; i < wordsize; i++)
    {
        if (status[i] == 2)
        {
            printf(GREEN"%c", guess[i]); //prints it green if correct choice
        }
        else if (status[i] == 1)
        {
            printf(YELLOW"%c", guess[i]); //prints it yellow if somewhere in word
        }
        else
        {
            printf(RED"%c", guess[i]); // prints it red if incorrect
        }
    }

    printf(RESET"\n");
    return;
}
