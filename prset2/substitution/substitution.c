#include <ctype.h>
#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Plan:
// 1. Accept key (26 letters) as command line argument
// - Only accept valid keys (26 characters):
// --- If wrong # of arguments exists, or non-alphabetical characters are provided, provide a "Usage" message: "Usage: ./substitution key"
// --- If an argument exists of the wrong length, provide message "Key must contain 26 characters"
// 2. Convert plaintext->ciphertext using key
// - Create "ciphertext" (accumulatively) of the same size as the original
// - Only substitue alphabetical cases: For ASCII characters in the "plaintext" that are not uppercase (isupper()) or lowercase (islower()), use the same character as in the "ciphertext"

bool invalidKey(string key);
bool invalidCharacters(string key);
bool isRepeat(char character, string str, int endIndex);
bool isAlphabetical(char character);
string cipher(string plaintext, string key);
char encryptChar(char character, string key);

// Constants
const int UPPERCASE_CHARACTER_START = 65;
const int LOWERCASE_CHARACTER_START = 97;

int main(int argc, string argv[])
{
    if (argc != 2)
    {
        printf("Usage: ./substitution key");
        return 1;
    }
    else if (invalidKey(argv[1]))
    {
        printf("Invalid key: Must contain 26 alphabetical characters without any repeat characters");
        return 1;
    }
    else
    {
        string plaintext = get_string("plaintext: ");
        string ciphertext = cipher(plaintext, argv[1]);
        printf("ciphertext: %s\n", ciphertext);
        return 0;
    }
}

// invalidKey : String -> Boolean
// Determines whether _key_ is valid: Contains 26 alphabetical characters without repeats
bool invalidKey(string key)
{
    return (strlen(key) != 26 || invalidCharacters(key));
}

// invalidCharacters : String -> Boolean
// Determines whether there are any non-alphabetical characters in _key_
// Idea: Each character in the key must either be upper or lowercase (isupper or islower)
bool invalidCharacters(string key)
{
    // If at least one invalid character is found, return true
    for (int i = 0, length = strlen(key); i < length; i++)
    {

        if (!isAlphabetical(key[i]))
        {
            return true;
        }

        if (isRepeat(key[i], key, i))
        {
            return true;
        }

    }

    // If not invalid characters are found, return false
    return false;
}

// isRepeat : Char String Int -> Boolean
// Does _character_ occur somewhere in _str_ up until _endIndex_?
bool isRepeat(char character, string str, int endIndex)
{
    for (int i = 0; i < endIndex; i++)
    {
        if (str[i] == character)
        {
            return true;
        }
    }

    return false;
}

// isAlphabetical : Char -> Boolean
// Is _character_ alphabetical?
bool isAlphabetical(char character)
{
    return isupper(character) || islower(character);
}

// cipher : String String -> String
// Using a valid, 26-character _key_, convert _plaintext_ to an encrypted form
// Idea:
// - Create an accumulator "ciphertext" string of length strlen(plaintext)
// - Loop through plaintext's characters
// - If the character is alphabetical, use the key to convert it
// - Else, make no modifications to the character

string cipher(string plaintext, string key)
{
    int length = strlen(plaintext);
    string ciphertext = plaintext;

    for (int i = 0; i < length; i++)
    {
        char curr = plaintext[i];
        if (isAlphabetical(curr))
        {
            ciphertext[i] = encryptChar(curr, key);
        }
        else
        {
            ciphertext[i] = curr;
        }
    }

    return ciphertext;
}

// encryptChar : Char String -> Char
// Converts _letter_ to its encrypted form based on _key_
char encryptChar(char letter, string key)
{
    // Plan:
    // Find corresponding position in key
    // Create a new character = key[position]
    // Convert key[position] to the same case as character
    // Return the new character

    char encryptedChar;

    if (isupper(letter))
    {
        int keyPosition = letter - UPPERCASE_CHARACTER_START;
        encryptedChar = toupper(key[keyPosition]);
    }
    else
    {
        int keyPosition = letter - LOWERCASE_CHARACTER_START;
        encryptedChar = tolower(key[keyPosition]);
    }

    return encryptedChar;

}