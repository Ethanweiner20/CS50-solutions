#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <cs50.h>
#include <math.h>

// Use this type to retrieve byte-sized "chunks" from the file
typedef uint8_t BYTE;
const int BLOCK_SIZE = 256;

// Tips:
// - Use "sprintf" to assist with numbering the files as three-digit decimal numbers
// - To remove jpgs from rough drafts, type "rm -f *.jpg" in CLI

// Pseudocode:

/*
// Continue to iterate until no more chunks
// No need to worry about "zeroed" chunks, jpegs can deal with those
while (chunk (512 bytes) in file exists)
    FILE * file_writing; // Current file we are writing
    if (chunk contains signature)
        close the current file file_writing points to
        open a new file for writing and store pointer in file_writing
    for (byte in chunk)
       write byte to file_writing
}
*/

bool contains_signature(BYTE block[BLOCK_SIZE]);

int main(int argc, char *argv[])
{
    // ERROR HANDLING

    // Incorrect number of command-line arguments
    if (argc != 2)
    {
        printf("Usage: ./recover image");
        return 1;
    }
    char *raw_file_path = argv[1];
    FILE *raw_file = fopen(raw_file_path, "r");

    // If the raw file could be found and memory was properly allocated
    if (raw_file == NULL)
    {
        printf("The forensic image cannot be opened for reading. Check the file path you inputted to make sure it's correct.");
        return 1;
    }

    // PROGRAM

    BYTE buffer[BLOCK_SIZE];

    int file_count = 0;

    // Declare a new file pointer that we can write to
    FILE *current_image = NULL;

    // As long as fread returns a number > 0, we have not reached the end of "raw_file"
    // Replace _buffer_ with next 512 bytes
    while (fread(buffer, sizeof(BYTE), BLOCK_SIZE, raw_file) != 0)
    {
        // If the block contains a signature, we have reached a new .jpeg files
        // Close old file and open new file
        if (contains_signature(buffer))
        {
            if (current_image != NULL)
            {
                fclose(current_image);
            }

            // Open a new file with the proper name
            char name[7];
            sprintf(name, "%.3d.jpg", file_count);
            current_image = fopen(name, "w");
            file_count++;

        }
        // Write the 512 bytes from the current _buffer_ to the _current_image_
        if (current_image != NULL)
        {
            fwrite(buffer, sizeof(BYTE), BLOCK_SIZE, current_image);
        }
    }

    fclose(raw_file);
    fclose(current_image);
}

// contains_signature : Buffer -> Boolean
// Does _block_ contain a jpeg signature?
bool contains_signature(BYTE block[BLOCK_SIZE])
{
    bool first_bytes = (block[0] == 0xff && block[1] == 0xd8 && block[2] == 0xff);
    // Checks if the first four bits are 1110 (0xe)
    bool fourth_byte = (block[3] / pow(2, 4) == 0xe);

    return (first_bytes && fourth_byte);
}