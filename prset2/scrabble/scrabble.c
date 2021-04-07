#include <ctype.h>
#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Ideas
// Use toupper for any inputted letter for consistency OR just use points for that index
// To access the particular scores given a character, subtract a particular number from that character (65?) to determine the proper index for that character

// Points assigned to each letter of the alphabet
int POINTS[] = {1, 3, 3, 2, 1, 4, 2, 4, 1, 8, 5, 1, 3, 1, 1, 3, 10, 1, 1, 1, 1, 4, 4, 8, 4, 10};

int compute_score(string word);
int points(char character);
int points_v2(char character);

int main(void)
{
    // Get input words from both players
    string word1 = get_string("Player 1: ");
    string word2 = get_string("Player 2: ");

    // Score both words
    int score1 = compute_score(word1);
    int score2 = compute_score(word2);

    // TODO: Print the winner
    if (score1 > score2)
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
    // TODO: Compute and return score for string

    // Plan:
    // 1. Iterate through the word
    // 2. Compute the score for each character, & add to the overall score
    // 3. Return the score

    int score = 0;

    for (int i = 0, n = strlen(word); i < n; i++)
    {
        score += points_v2(word[i]);
    }

    return score;
}

// Return the points represented by a particular _character_, as mapped in scrabble in the array POINTS[]

// Version 1:
int points(char character)
{
    // Plan:
    // 1. Determine the integer value of that character
    // 2. Branch/conditional: If the integer value indicates it as a letter, then search in scores array, else return 0
    // 3. Determine the index of that value in the alphabet (with A starting at 0):
    // This properly maps the index of the character to the index of the points array
    // 4. Return the value associated with the points array

    // Will automatically cast char to int because of context within operator
    // Searching between various OR conditions

    if (character >= 65 && character <= 90)  // Character is an uppercase letter
    {
        return POINTS[character - 65];
    }
    else if (character >= 97 && character <= 122) // Character is a lowercase letter
    {
        return POINTS[character - 97];
    }
    else // Character is not a letter
    {
        return 0;
    }
}

// Version 2:
int points_v2(char character)
{
    // Plan:
    // 1. Determine the integer value of that character
    // 2. Branch/conditional: If the integer value indicates it as a letter, then search in scores array, else return 0
    // 3. Determine the index of that value in the alphabet (with A starting at 0):
    // This properly maps the index of the character to the index of the points array
    // 4. Return the value associated with the points array

    // Will automatically cast char to int because of context within operator
    // Searching between various OR conditions

    if (isupper(character))  // Character is an uppercase letter
    {
        return POINTS[character - 65];
    }
    else if (islower(character)) // Character is a lowercase letter
    {
        return POINTS[character - 97];
    }
    else // Character is not a letter
    {
        return 0;
    }
}