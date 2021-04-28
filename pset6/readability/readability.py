import re
from cs50 import get_string


def main():
    text = get_string("Text: ")
    print_grade(cl_index(text))


# print_grade : Float -> _
# Determines the grade level associated with a particular Coleman-Liau _index_


def print_grade(index):

    grade_level = round(index)

    if (grade_level >= 16):
        print("Grade 16+")
    elif (grade_level < 1):
        print("Before Grade 1")
    else:
        print(f"Grade {grade_level}")


# cl_index : String -> Float
# Determines the cl index for some _text_


def cl_index(text):
    return 0.0588 * letter_avg(text) - 0.296 * sentence_avg(text) - 15.8


# letter_avg : String -> Float
# Computes the avareage number of letters per every 100 words in _text_


def letter_avg(text):
    return 100 * num_letters(text) / num_words(text)


# sentence_avg : String -> Float
# Computers the average number of sentences per every 100 words in _text_


def sentence_avg(text):
    return 100 * num_sentences(text) / num_words(text)


# num_letters : String -> NN
# Computes the total number of letters in _text_


def num_letters(text):
    letters = re.findall("[A-Za-z]", text)
    return len(letters)


# num_words : String -> NN
# Computes the total number of words in _text_


def num_words(text):
    spaces = re.findall(" ", text)

    # Count the first word
    if (len(text)):
        return len(spaces) + 1

    return 0


# num_sentences : String -> NN
# Computes the total number of sentences in _text_


def num_sentences(text):
    sentences = re.findall("[.!?]", text)
    return len(sentences)


main()