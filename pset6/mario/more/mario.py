from cs50 import get_int


def main():
    while True:
        height = get_int("Height: ")
        # Height given is valid
        if (height > 0 and height <= 8):
            break

    # Initialize at 0 spaces
    build_pyramid(height, 0)


# build_pyramid : Int -> _
# Prints a pyramid of height _height_
# Recursive idea: Print a pyramid of size _height_-1, then print the current row
# Accumulator : _spaces_ represents the number of spaces on one half-pyramid


def build_pyramid(height, spaces):

    # Base case
    if (height == 0):
        return

    # "height - 1" pyramid
    build_pyramid(height - 1, spaces + 1)

    # Current row (the bottom row will always be the size of the height)
    print_character(spaces, " ")
    print_character(height, "#")
    print_character(2, " ")
    print_character(height, "#")
    # Spaces needed at end of each row to compile?
    # New line to end row
    print()


# print_character : Int -> _
# Prints _num_ iterations of _char_


def print_character(num, char):
    for i in range(num):
        print(char, end="")


main()