#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max number of candidates
#define MAX 9

// preferences[i][j] is number of voters who prefer i over j
int preferences[MAX][MAX];

// locked[i][j] means i is locked in over j
bool locked[MAX][MAX];

// Each pair has a winner, loser
typedef struct
{
    int winner;
    int loser;
}
pair;

// Array of candidates
string candidates[MAX];
pair pairs[MAX * (MAX - 1) / 2];

int pair_count;
int candidate_count;

// Function prototypes
bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
void lock_pairs(void);
void print_winner(void);
bool pair_cycles(int initialPos, int currentPos);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: tideman [candidate ...]\n");
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
        candidates[i] = argv[i + 1];
    }

    // Clear graph of locked in pairs
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            locked[i][j] = false;
        }
    }

    pair_count = 0;
    int voter_count = get_int("Number of voters: ");

    // Query for votes
    for (int i = 0; i < voter_count; i++)
    {
        // ranks[i] is voter's ith preference
        int ranks[candidate_count];

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            if (!vote(j, name, ranks))
            {
                printf("Invalid vote.\n");
                return 3;
            }
        }

        record_preferences(ranks);

        printf("\n");
    }

    add_pairs();
    sort_pairs();
    lock_pairs();
    print_winner();
    return 0;
}

// Update ranks given a new vote
bool vote(int rank, string name, int ranks[])
{
    for (int i = 0; i < candidate_count; i++)
    {
        if (strcmp(name, candidates[i]) == 0) //is the name of the choice the same as the name of the candidate in i-th place
        {
            ranks[rank] = i;
            return true;
        }
    }
    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = i + 1; j < candidate_count; j++)
        {
            preferences[ranks[i]][ranks[j]]++; //increments the number of voters who prefer candidate i over candidate j
        }
    }
    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    for (int j = 0; j < candidate_count; j++)
    {
        for (int k = j + 1; k < candidate_count; k++) //double for loop for preferences referencing
        {
            if (preferences[j][k] > preferences[k][j]) //if more people prefer candidate j over candidate k
            {
                pairs[pair_count].winner = j; //then j is the winner, and the loser is k
                pairs[pair_count].loser = k;
                pair_count++;
            }
            else if (preferences[j][k] < preferences[k][j]) //if more people prefer candidate k over candidate j
            {
                pairs[pair_count].winner = k; //then the opposite, where k is the winner
                pairs[pair_count].loser = j;
                pair_count++; //pair count incremented
            }
            else
            {

            }
        }
    }
    return;
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    pair currentPair = pairs[0]; //initializes a pair to compair against at the start of the array
    for (int i = 0; i < pair_count; i++)
    {
        for (int j = i + 1; j < pair_count; j++)
        {
            if (preferences[pairs[j].winner][pairs[j].loser] > preferences[pairs[i].winner][pairs[i].loser])
            {
                currentPair = pairs[j]; //if the amount of people who prefer the j-th pair is higher than the i-th pair
                pairs[j] = pairs[i]; //store j-th pair in variable, then swap the j-th and i-th pairs
                pairs[i] = currentPair;
            }
        }
    } //this should ensure that greatest pairs are moved to the left
    return;
}

// locked[i][j] means i is locked in over j
// bool locked[MAX][MAX];

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    for (int i = 0; i < pair_count; i++)
    {
        locked[pairs[i].winner][pairs[i].loser] = true;
        if (pair_cycles(pairs[i].winner, pairs[i].winner) || pair_cycles(pairs[i].loser, pairs[i].loser))
        {
            locked[pairs[i].winner][pairs[i].loser] = false;
        }
    }
}

bool pair_cycles(int initialCandidate, int currentCandidate)
{
    if (locked[currentCandidate][initialCandidate]) //if we are right back to where we started
    {
        return true; //then this is definitely a cycle
    }
    bool cycles = false;
    for (int j = 0; j < candidate_count; j++) //loop through each of the candidates
    {
        if (locked[currentCandidate][j])
        {
            cycles = pair_cycles(initialCandidate, j); //if j-th place beats or loses to a currentPos in opposite situation, check cycling
        }
        else //if this is not happening, then don't check pair_cycles for that
        {

        }
    }
    return cycles;
}

// Print the winner of the election
void print_winner(void)
{
    int winner = 0;
    for (int i = 1; i < candidate_count; i++)
    {
        if (locked[i][winner]) //if the i beats winner
        {
            winner = i; //assigns new winner
        }
    }
    printf("%s\n", candidates[winner]);
    return;
}