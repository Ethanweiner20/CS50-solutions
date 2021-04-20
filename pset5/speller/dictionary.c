// Implements a dictionary's functionality

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>

#include "dictionary.h"

// Represents a node in a hash table
// May or may not be linked to other nodes
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

bool load_word(char *word);
void insert_node(int hash, node *n);
unsigned int list_size(node *n);
bool contains_word(node *list, const char *word);
bool unload_list(node *list);

// NOTES:
// - Ultimate enemy here is TIME
// - Focus less on space -- load is the only function that cares about it
// - Use the smaller dictionary and smaller texts when testing for less waiting

// Number of buckets in hash table
// Ideas: 26 (one letter), 26^2 (two letters) . . ., [26, 26^2] (one letters for some, two letters for others?)
const unsigned int N = 200000;

// Hash table
node *table[N];

// Loads dictionary into memory, returning true if successful, else false
// By "load into memory", does this mean LOAD INTO TABLE?
// Time to load indicates MEMORY USAGE -- less buckets = better (using many hashes requires many buckets)
bool load(const char *dictionary)
{
    // TODO
    FILE *dictionary_file = fopen(dictionary, "r");

    // File could not be found
    if (dictionary_file == NULL)
    {
        return false;
    }

    // Buffer word, assume length can't be longer than LENGTH + 1
    char buffer[LENGTH + 1];

    while (fscanf(dictionary_file, "%s", buffer) != EOF)
    {
        bool loaded = load_word(buffer);
        if (!loaded)
        {
            return false;
        }
    }

    fclose(dictionary_file);


    return true;
}

// load_word
// Loads _word_ (as a node) into the hash table, based on the hash function
bool load_word(char *word)
{
    // Allocate memory for a new node
    node *n = malloc(sizeof(node));

    if (n == NULL)
    {
        return false;
    }

    // Initialize the node
    strcpy(n->word, word);
    n->next = NULL;

    // Insert the node into the hash table
    insert_node(hash(word), n);

    return true;
}

// insert_node
// Inserts _node_ in the linked list of the hash table at the location _index_
void insert_node(int index, node *n)
{
    // Link _n_ to the head node at table[hash]
    n->next = table[index];
    // Set _n_ to be the new head node
    table[index] = n;
}

// OPPORTUNITY FOR FLEXIBILITY:

// Converts word to a "hash" (representing the bucket in the hash table)
// NOTE: Input Size = 143,091
// Ideas:
// - Search up most commonly used letters, & create an algorithm that determines number of letters to include for that string?
// - Determine hash function BASED ON INPUT SIZE OF DICTIONARY
// --- GOAL: All buckets contain as least words as possible
// --- Every bucket contains at least 1 word
// --- EMPTY BUCKETS ARE BAD
// Questions:
// - How close should N (# of possible hashes) be to the input size (143,091?)
// - Why can't the # of buckets be exactly equal to the input size, and then just find a hash function that is REALLY GOOD at creating unique values?
// - OH WAIT -- since the hash table is an array, randomly indexing to very large nunbers => empty space in the array
// - We don't want empty space, so let's do our best to create a relatively small array (perhaps just a bit larger than input size?)
// - Remember though, memory isn't too much of a problem

// HASHING CAN BE COMPLEX
// Do some research for the hash function, citing your sources

/*
RESEARCH:
- Remember, we aren't caring about memory as much as we are speed here
- Keep hash table filled roughly 80% or less -- don't want higher; probably more like 60% since we are using linked lists
- Buckets:Items > 1, but just a bit
- Summing the letters is not a great strategy <== permutations give same result, & English dictionary contains many permutations
- Potentials:
--- djb2
*/

// unsigned int hash_v2(const char *word)
// {
//     long sum = 0;
//     int c = tolower(*word);

//     while (c != '\0')
//     {
//         sum = 31 * sum + c;
//         c = tolower(*(word++));
//     }

//     return sum % N;

// }

// Settled on dbj2 -- "you are not likely to do better with one of the "well known" hash functions
unsigned int hash(const char *word)
{
    // TODO
    unsigned long long_hash = 5381;
    int c = tolower(*word);

    // Loops through _word_ (by incrementing its pointer), assigning its character value to _c_
    while (c != '\0')
    {
        // Shifts the hash 5 bits to left (multiply by 2^5 = 32) and adds itself ==>
        // Iteratively multiplies itself by 33, and adds the chracter value each time "c"
        long_hash = ((long_hash << 5) + long_hash) + c; /* hash * 33 + c */
        c = tolower(*(word++));
    }

    // Taking hash mod N should maintain the uniqueness of the hash value (intuitively, the remainder is basically as unique as the actual value itself)
    // Need to typecast to int?
    return (long_hash % N);
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    int size = 0;
    // TODO
    for (int i = 0; i < N; i++)
    {
        size += list_size(table[i]);
    }

    return size;
}

// list_size : [List-of String] -> Int
// Determines the size of _list_
// Use list template
unsigned int list_size(node *list)
{
    if (list == NULL)
    {
        return 0;
    }

    return list_size(list->next) + 1;
}

// Returns true if word is in dictionary, else false
// _word_ must be EXACTLY included in the dictionary, case-insensitive
// Note: all words in the dictionary are lowercase
// Represents SEARCH TIME -- more buckets = better
bool check(const char *word)
{
    // TODO
    // Lookup into array is constant time
    if (contains_word(table[hash(word)], word))
    {
        return true;
    }
    // Compares strings without caring about case
    // ...strcasecmp(...)...
    return false;
}


// contains_word : [List-of String] String -> Boolean
// Does _list_ contain _word_?
// Lookup into linked list is linear time, but hopefully small
bool contains_word(node *list, const char *word)
{
    // Base case
    if (list == NULL)
    {
        return false;
    }

    // Either the current node contains _word_, or the rest of the nodes contains _word_
    // Will short circuit if true ==> Less than O(n) running time possible
    return (strcasecmp(list->word, word) == 0) || contains_word(list->next, word);
}

// Unloads dictionary from memory, returning true if successful, else false
// Question: How would freeing memory fail?
// Use VALGRIND to test if this function properly frees memory
bool unload(void)
{
    // TODO
    for (int i = 0; i < N; i++)
    {
        // Try to
        if (!unload_list(table[i]))
        {
            return false;
        }
    }

    return true;
}

// unload_list : [List-of String] -> Boolean
// Unloads a given _list_ from memory, returning true if successful, else false
/*
Example: '(1 2 3)
unload_list(1 2 3) ==> unload_list(2 3), free(1) ==> unload_list(3), free(2) ==> unload_list(NULL), free(3) ==> true
*/
bool unload_list(node *list)
{
    // Base case
    if (list == NULL)
    {
        return true;
    }

    // Need to do this order so that we can access the rest of the list (avoid ophanage)
    // Temporary pointer to the rest of the list
    node *rest = list->next;
    // Free the current node
    free(list);
    // Free the other nodes
    return unload_list(rest);

    // Return false if list was improperly freed
    return false;

}

