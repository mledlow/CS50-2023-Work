#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max number of candidates
#define MAX 9

// Candidates have name and vote count
typedef struct
{
    string name;
    int votes;
}
candidate;

// Array of candidates
candidate candidates[MAX];

// Number of candidates
int candidate_count;

// Function prototypes
bool vote(string name);
void print_winner(void);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: plurality [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX)
    {
        printf("Maximum number of candidates is %i\n", MAX);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i].name = argv[i + 1];
        candidates[i].votes = 0;
    }

    int voter_count = get_int("Number of voters: ");

    // Loop over all voters
    for (int i = 0; i < voter_count; i++)
    {
        string name = get_string("Vote: ");

        // Check for invalid vote
        if (!vote(name))
        {
            printf("Invalid vote.\n");
        }
    }

    // Display winner of election
    print_winner();
}

// Update vote totals given a new vote
bool vote(string name)
{
    for (int i = 0; i < candidate_count; i++)
    {
        if (strcmp(candidates[i].name, name) == 0) //if the two names are the same
        {
            candidates[i].votes++;
            return true; //increments their vote and returns true, otherwise loops
        }
    }
    return false;
}

// Print the winner (or winners) of the election
void print_winner(void)
{
    int highestVote = candidates[0].votes; //initialized to first candidate's votes
    for (int i = 1; i < candidate_count; i++)
    {
        if (candidates[i].votes > highestVote) //if current candidate's votes are higher than current winner's
        {
            highestVote = candidates[i].votes; //sets new highest vote count to theirs
        }
    }
    for (int j = 0; j < candidate_count; j++)
    {
        if (candidates[j].votes == highestVote) //if the candidate at j place has votes equal to the highest number of votes
        {
            printf("%s\n", candidates[j].name); //print their name and a new line (allows for ties)
        }
    }
    return;
}