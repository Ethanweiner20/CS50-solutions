#include <cs50.h>
#include <stdio.h>

// Use abstraction on all methods that sum the digit?
// - Differences: Starting point (from right), skip-interval, factor to multiply digits?

int getFirstSum(long cardNumber);
int getSecondSum(long cardNumber);
int sumDigits(int number);
int cardLength(long cardNumber);
void printCardType(long cardNumber, int cardLength);
int digit(long number, long position);
int digit_v2(long number, long position);

int main(void)
{
    long cardNumber = get_long("Number: ");
    // 1. Determine sum of the digtis of [every other digit (starting at second to rightmost) x 2]
    int firstSum = getFirstSum(cardNumber);

    // 2. Determine sum of every other digit (starting at rightmost) x 1
    int secondSum = getSecondSum(cardNumber);

    // 3. Sum "firstSum" and "secondSum"
    int checkSum = firstSum + secondSum;

    // Can we use modulo operator here?
    bool validCheckSum = (checkSum % 10 == 0);

    if (validCheckSum)
    {
        // Since the number is valid, we can determine its type
        printCardType(cardNumber, cardLength(cardNumber));
    }
    else
    {
        printf("INVALID\n");
    }
}

// getFirstSum
// Determines the sum of every other digit, starting at the second to rightmost
int getFirstSum(long cardNumber)
{

    // Current position of the digit we are searching for
    long place = 10;
    int sum = 0;

    while (place <= cardNumber)
    {
        sum += sumDigits(digit_v2(cardNumber, place) * 2);
        place *= 100;
    }

    return sum;


}

// getSecondSum
// Determines the sum of every other digit, starting at the rightmost
int getSecondSum(long cardNumber)
{
    // Current position of the digit we are searching for
    long place = 1;
    int sum = 0;

    while (place <= cardNumber)
    {
        sum += digit_v2(cardNumber, place);
        place *= 100;
    }

    return sum;
}

// sumDigits
int sumDigits(int number)
{
    int place = 1;
    int sum = 0;

    while (place <= number)
    {
        sum += digit_v2(number, place);
        place *= 10;
    }
    return sum;
}

// cardLength
// Determines the number of digits present in _cardNumber_
int cardLength(long cardNumber)
{
    long place = 1;
    int length = 0;
    while (place <= cardNumber)
    {
        place *= 10;
        length++;
    }
    return length;
}

// printCardType
// Determines the card type of a  valid _cardNumber_ with length _cardLength_
void printCardType(long cardNumber, int cardLength)
{
    int digit_16 = digit_v2(cardNumber, 1000000000000000);
    int digit_15 = digit_v2(cardNumber, 100000000000000);
    int digit_14 = digit_v2(cardNumber, 10000000000000);
    int digit_13 = digit_v2(cardNumber, 1000000000000);

    // American Express
    bool amex = (cardLength == 15 && digit_15 == 3 && (digit_14 == 4 || digit_14 == 7));
    // Put this into another variable for styling purposes
    bool mastercardSecondDigit = (digit_15 == 1 || digit_15 == 2 || digit_15 == 3 || digit_15 == 4 || digit_15 == 5);
    bool mastercard = cardLength == 16 && digit_16 == 5 && mastercardSecondDigit;
    bool visa = (cardLength == 13 && digit_13 == 4) || (cardLength == 16 && digit_16 == 4);

    if (amex)
    {
        printf("AMEX\n");
    }
    else if (mastercard)
    {
        printf("MASTERCARD\n");
    }
    else if (visa)
    {
        printf("VISA\n");
    }
    else
    {
        printf("INVALID\n");
    }
}

// digit
// Determines the digit in _place_ of _number_ without using the modulo operator

int digit(long number, long place)
{
    // The card number rounded to the digits place we are searching for
    long rounded = (number / (place * 10)) * (place * 10);

    long extra = number - rounded;

    // The digit we are looking for is the FIRST DIGIT of the difference of the extra discarded from rounding
    int digit = extra / place;

    return digit;
}

// digit_v2
// Determines the digit in _place_ of _number_ using the modulo operator

int digit_v2(long number, long place)
{
    // Plan: Determine the remainder when divided by the right place
    long extra = number % (place * 10);
    int digit = extra / place;
    return digit;
}