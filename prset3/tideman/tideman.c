#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max number of candidates
#define MAX 9

// preferences[i][j] is number of voters who prefer i over j
int preferences[MAX][MAX];

// locked[i][j] means i is locked in over j
// INTERPRETATION: A "true" entry represents an edge from i to j, a "false" entry represents NO EDGE
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

// Contains pairs of candidates where one candidate is preferred
// * Does not include ties
// Can be unordered, or ordered from highest to lowest margin
pair pairs[MAX * (MAX - 1) / 2];

int pair_count;
int candidate_count;

// Function prototypes
bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void record_preferences_helper(int ranks[], int winner_index);
void add_pairs(void);
void sort_pairs(void);
void lock_pairs(void);
void print_winner(void);
int get_candidate_index(string name);
void selection_sort(int array[]);
void print_candidate(int candidate_index);
int margin(pair pair);
bool find_path(int origin, int destination);

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

    // Clear graph of locked in pairs: Initialize the graph to have NO EDGES
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

            // Update the voter's _ranks_ with _name_ in index _j_ of rank
            bool voted = vote(j, name, ranks);

            if (!voted)
            {
                printf("Invalid vote.\n");
                return 3;
            }
        }

        // Using the new voter's ranks, update the preferences
        record_preferences(ranks);

        printf("\n");
    }

    add_pairs();
    sort_pairs();
    lock_pairs();
    print_winner();
    return 0;
}

// vote : Number String [List-of Int] -> Boolean
// Update a voter's _ranks_ with candidate _name_'s index as _rank_
bool vote(int rank, string name, int ranks[])
{
    int candidateIndex = get_candidate_index(name);
    // If the index of the candidate is found
    if (candidateIndex != -1)
    {
        ranks[rank] = candidateIndex;
        return true;
    }
    return false;
}

// get_candidate_index : Retrieve the index of the candidate associated with _name_
int get_candidate_index(string name)
{
    for (int index = 0; index < candidate_count; index++)
    {
        if (strcmp(candidates[index], name) == 0)
        {
            return index;
        }
    }
    return -1;
}

// record_preferences : [List-of Index] -> _
// Update the pairings in "preferences" based on a candidates ranking

/*
Example:
Input: [1, 0, 2]
Output: preferences[1][0]++, preferences[1][2]++, preferences[0][2]++
*/


void record_preferences(int ranks[])
{
    // Method 1: Iterative

    // Loop through all possible "winners" in _ranks_
    for (int winner_ranking = 0; winner_ranking < candidate_count - 1; winner_ranking++)
    {
        // For each winner, update their pairings with every "loser"
        for (int loser_ranking = winner_ranking + 1; loser_ranking < candidate_count; loser_ranking++)
        {
            int winner_index = ranks[winner_ranking];
            int loser_index = ranks[loser_ranking];
            preferences[winner_index][loser_index]++;
        }
    }

    // Method 2: Recursive
    // record_preferences_helper(ranks, 0);

    return;
}

// record_preferences_helper : [List-of Int] Int -> _
void record_preferences_helper(int ranks[], int winner_ranking)
{
    // We have run out of pairings to update
    if (winner_ranking >= candidate_count - 1)
    {
        return;
    }

    for (int loser_ranking = winner_ranking + 1; loser_ranking < candidate_count; loser_ranking++)
    {
        int winner_index = ranks[winner_ranking];
        int loser_index = ranks[loser_ranking];
        preferences[winner_index][loser_index]++;
    }

    record_preferences_helper(ranks, winner_ranking + 1);
}

// Record pairs of candidates where one is preferred over the other

void add_pairs(void)
{
    // Initialize pair count
    pair_count = 0;

    for (int i = 0; i < candidate_count; i++)
    {
        // For each candidate (i), add all pairings in which this candidate is preferred
        for (int j = 0; j < candidate_count; j++)
        {
            // If there are more voters prefer i over j than voters who prefer j over i
            if (preferences[i][j] > preferences[j][i])
            {
                pair newPair;
                newPair.winner = i;
                newPair.loser = j;

                // Add the new pair to the pairs array
                pairs[pair_count] = newPair;

                // Increment the pair count
                pair_count++;
            }
        }
    }

    return;
}

// sort_pairs
// Sorts pairs according to selection sort, in order of decreasing strength
// Why selection sort? We are only dealing with small array sizes and it has the easiest implementation
void sort_pairs(void)
{

    for (int i = 0; i < pair_count; i++)
    {
        int max_index = i;

        for (int j = i + 1; j < pair_count; j++)
        {
            // If the margin of the pair is greater than the margin of the maximum pair so far,
            // move to the front of the array
            if (margin(pairs[j]) > margin(pairs[max_index]))
            {
                max_index = j;
            }
        }

        // Only necessary to sort if max_index actually changed
        if (max_index != i)
        {
            pair temp = pairs[i];
            pairs[i] = pairs[max_index];
            pairs[max_index] = temp;
        }

    }

}

// margin : Pair -> Int
// Determine the margin of win in pair
int margin(pair pair)
{
    return preferences[pair.winner][pair.loser];
}

// Lock pairs into the candidate graph in order, without creating cycles
// Accumulator: The locked graph so far

void lock_pairs(void)
{

    for (int i = 0; i < pair_count; i++)
    {
        bool creates_cycle = find_path(pairs[i].loser, pairs[i].winner);

        if (!creates_cycle)
        {
            locked[pairs[i].winner][pairs[i].loser] = true;
        }

    }

    return;
}

// find_path: Int Int -> Boolean
// Does a path exist from _origin_ to _destination_ in _locked_?
bool find_path(int origin, int destination)
{
    // Is _origin_ directly connected to _destination_?
    bool directly_connected = locked[origin][destination];

    // Does one of origin's neighbors have a path to destination?
    bool indirectly_connected = false;

    // Exhaust all the neighbors when checking for cycles
    for (int potential_neighbor = 0; potential_neighbor < candidate_count; potential_neighbor++)
    {
        bool is_neighbor = locked[origin][potential_neighbor];
        if (is_neighbor && find_path(potential_neighbor, destination))
        {
            indirectly_connected = true;
        }
    }

    return (directly_connected || indirectly_connected);
}

// Print the winner of the election
void print_winner(void)
{
    // For each candidate, determine if they are a "source"
    for (int potential_source = 0; potential_source < candidate_count; potential_source++)
    {
        bool is_source = true;

        for (int i = 0; i < candidate_count; i++)
        {
            // If the potential source is ever a loser
            if (locked[i][potential_source])
            {
                is_source = false;
            }
        }

        if (is_source)
        {
            printf("%s\n", candidates[potential_source]);
        }
    }

    return;
}