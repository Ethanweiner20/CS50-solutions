import sys
import csv

# Notes:

# - An STR count will be defined as the longest run pattern of a particular sequence that can be found
# For a match to be present, all STR counts in the given _sequence_ must be equal to the STR counts of some row in the .csv file

# Plan
# 1. Determine the particular STRs to look for, using the first row of the .csv file X
# 2. For each STR, determine its maximum consecutive count in "sequence" (by searching for the LONGEST consecutive sequence in "sequence") X
# 3. Iterate through every row (person) of the .csv file X
# 4. For each row (person), check if all STR counts match those found in sequence X
# 5. If a match is found, print the name of the matching individual and break from the loop X
# 6. Else print "No match" X

# Data Definitions

# A Person is a dictionary with fields:
# - Name
# - [Multiple fields] STR counts

# An STR Count is the longest run pattern of a particular STR name in a given dna sequence


def main():

    if (len(sys.argv) != 3):
        print("Usage: python dna.py <database> <sequence>")
        return 1

    database_path = sys.argv[1]
    sequence_path = sys.argv[2]

    # Load the sequence into memory
    dna_sequence = ""

    with open(sequence_path, 'r') as sequence_file:
        dna_sequence = sequence_file.read()

    # Load the database into memory
    with open(database_path, 'r') as database_file:
        reader = csv.DictReader(database_file)
        # For each person, try to find a match
        for person in reader:

            if (is_match(person, dna_sequence)):
                print(person['name'])
                return 0

    # If no person is found as a match
    print("No match")
    return 0


# is_match : Person String -> Boolean
# Do all STR counts in _person_ match the corresponding counts in _dna_sequence_?


def is_match(person, dna_sequence):

    for key, value in person.items():

        # Skip the iteration if the item is the name (rather than an STR count)
        if key == 'name':
            continue

        # The STR count in the provided person
        person_str_count = int(value)
        # The STR count found in the DNA sequence
        dna_sequence_str_count = longest_sequence(key, dna_sequence)

        if (person_str_count != dna_sequence_str_count):
            return False

    # All STR counts matched, so the person indeed matches the DNA sequence
    return True


# longest_sequence : String String -> NN
# Determines the maximum number of times that _STR_ repeats in _dna_sequence_


def longest_sequence(STR, dna_sequence):
    # Plan:
    # Iterate over every character in _dna_sequence_
    # For each character, create a substring from that character of length len(STR)
    # If that substring is equal to the given STR, store num_repeats(STR) in variable max (longest = max(longest, num_repeats(STR, i)))

    longest = 0

    # Iterate over index
    for i in range(0, len(dna_sequence)):
        substr = dna_sequence[i:i + len(STR)]
        if (substr == STR):
            longest = max(longest, count_repeats(STR, i, dna_sequence))

    return longest


# count_repeats : String Number String -> NN
# Count the number of times _STR_ repeats in _dna_sequence_ starting at index _starting_index_


def count_repeats(STR, starting_index, dna_sequence):

    count = 0
    index = starting_index

    # Continue incrementing while the current substring is equal to the STR
    while (dna_sequence[index:index + len(STR)] == STR):
        count += 1
        index += len(STR)

    return count


main()