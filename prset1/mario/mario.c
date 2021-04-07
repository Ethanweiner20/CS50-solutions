#include <cs50.h>
#include <stdio.h>

// Prototype
void printCharacters(int numCharacters, string character);

int main(void)
{
    int HASH_SPACE = 2;
    int height;

    // Get the height of the pyramid between 1 & 8, inclusive
    do
    {
        height = get_int("Height: ");
    }
    // Retrieve height until inverse is true: height >= 1 && height <= 8 (our condition)
    while (height < 1 || height > 8);

    // Use for calculation purposes
    int TOTAL_WIDTH = height * 2 + 2;

    // For each row:
    for (int row = 0; row < height; row++)
    {
        // Number of hashes per side of row
        int numHashes = row + 1;
        // Blank space on left & right of hashes
        int margins = TOTAL_WIDTH - (numHashes * 2 + 2);

        // Left side margins
        printCharacters(margins / 2, " ");
        // Left side hashes
        printCharacters(numHashes, "#");
        // Space between hashes
        printf("  ");
        // Right side hashes
        printCharacters(numHashes, "#");

        // New line after row
        printf("\n");
    }
}

void printCharacters(int numCharacters, string character)
{
    for (int i = 0; i < numCharacters; i++)
    {
        printf("%s", character);
    }
}