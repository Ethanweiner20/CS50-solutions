#include <ctype.h>
#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

void printGrade(float index);
float clIndex(string text);
float lettersPer100Words(string text);
float sentencesPer100Words(string text);
int numLetters(string text);
int numWords(string text);
int numSentences(string text);

int main(void)
{
    string text = get_string("Text: ");
    printGrade(clIndex(text));
}

// printGrade : Float -> _
// Determines the grade level associated with a particular Coleman-Liau index
void printGrade(float index)
{
    int gradeLevel = round(index);

    if (gradeLevel >= 16)
    {
        printf("Grade 16+\n");
    }
    else if (gradeLevel < 1)
    {
        printf("Before Grade 1\n");
    }
    else
    {
        printf("Grade %i\n", gradeLevel);
    }
}

float clIndex(string text)
{
    return 0.0588 * lettersPer100Words(text) - 0.296 * sentencesPer100Words(text) - 15.8;
}

// lettersPer100Words : String -> Float
// Computes the average number of letters per 100 words of _text_
float lettersPer100Words(string text)
{
    return ((float) numLetters(text) / (float) numWords(text)) * 100;
}

// sentencesPer100Words : String -> Float
// Comutes the avarage number of sentences per 100 words of _text_
float sentencesPer100Words(string text)
{
    return ((float) numSentences(text) / (float) numWords(text)) * 100;
}

// numLetters : String -> Int
// Computes the number of letters (a-z or A-Z) in _text_
int numLetters(string text)
{
    int total = 0;

    // Iterate over the characters in the string
    for (int i = 0, length = strlen(text); i < length; i++)
    {
        // If the character is a letter, add it to the total
        if (isupper(text[i]) || islower(text[i]))
        {
            total++;
        }
        // Else, continue to the next character
    }

    return total;
}

// numWords : String -> Int
// Computes the number of words (any sequence of characters separated by spaces) in _text_
int numWords(string text)
{
    int total = 0;

    // Idea: Count the # of spaces, plus 1? (using a loop, increment whenever the character is a space)
    for (int i = 0, length = strlen(text); i < length; i++)
    {
        // Count all words after the first (' ' indicates start of new word)
        if (text[i] == ' ')
        {
            total++;
        }
        // Count the first word
        else if (i == 0)
        {
            total++;
        }
    }

    return total;
}


// numSentences : String -> Int
// Computes the number of sentences (any sequence of characters ending with a period, exclamation point, or question mark) in _text
int numSentences(string text)
{
    int total = 0;

    // Idea: Count the # of ending punctuation (using a loop)
    for (int i = 0, length = strlen(text); i < length; i++)
    {
        if (text[i] == '.' || text[i] == '!' || text[i] == '?')
        {
            total++;
        }
    }

    return total;

}