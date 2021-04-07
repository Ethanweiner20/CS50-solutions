#include <cs50.h>
#include <stdio.h>

// Problem: Given start size & end size of a population, determine # years
// it takes to reach end size

int main(void)
{
    // TODO: Prompt for start size
    int startSize, endSize;

    do
    {
        startSize = get_int("Start size: ");
    }
    // Exit loop once startSize >= 9
    while (startSize < 9);

    // TODO: Prompt for end size

    do
    {
        endSize = get_int("End size: ");
    }
    // Exit loop once endSize >= startSize
    while (endSize < startSize);

    // TODO: Calculate number of years until we reach threshold

    // Method 1:
    // Performed some algebra on paper to retrieve # of years
    // endSize = startSize + (startSize / 3) * years - (startSize / 4) * years
    // int years = 12 * (endSize - startSize) / startSize;\

    // This method is incorrect, because it assumes that the population growth
    // is dependent on the STARTSIZE, not the size of the population each year
    // (i.e. it doesn't include a recurrence)

    // Method 2 (Better):
    // Use a loop to accumulate the number of years

    int currentPopulation = startSize;
    int years = 0;

    // Ends once currentPopulation >= (is at least) endSize
    while (currentPopulation < endSize)
    {
        currentPopulation = currentPopulation + currentPopulation / 3 - currentPopulation / 4;
        years++;
    }

    // TODO: Print number of years

    printf("Years: %i\n", years);
}