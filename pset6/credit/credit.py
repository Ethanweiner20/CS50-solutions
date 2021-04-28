from cs50 import get_string
import re

# Ideas: Use regex to validate user input (correct length, only digits?)
# Criteria for valid card number:
# - Only includes numbers
# - Length is 13, 15, or 16
# - Proper checksum
# Easier to work with strings than numbers in this scenario (perform less arithmetic)


def main():

    user_input = get_string("Number: ")

    # Check for correct length and only diigts
    if (not(possible_card(user_input))):
        print("INVALID")
        return 1

    # Check if the check sums are valid
    if (not(valid_check_sum(user_input))):
        print("INVALID")
        return 2

    # Print the particular type of card
    print_card_type(user_input)

    return 0


# valid : String -> Boolean
# Is the provided user_input a possible card number?


def possible_card(user_input):
    match = re.fullmatch('[0-9]{13}|[0-9]{15}|[0-9]{16}', user_input)
    if match:
        return True
    return False


# valid_check_sum : String -> Boolean
# Do the digits in _card_number_ form a valid checksum?


def valid_check_sum(card_number):
    check_sum = first_sum(card_number) + second_sum(card_number)
    valid = check_sum % 10 == 0
    return valid


# first_sum : String -> Number
# Determines the sum of every other digit in _card_number_ times 2, starting at the second to rightmost


def first_sum(card_number):

    # Accumulator
    total = 0
    card_length = len(card_number)

    for i in range(1, card_length, 2):
        index = (card_length - 1) - i
        total += sum_digits(int(card_number[index]) * 2)

    return total


# sum_digits : Number -> Number
# Determines the sum of all digits in _num_


def sum_digits(num):

    # Accumulator
    total = 0
    digits = str(num)

    for i in range(0, len(digits)):
        total += int(digits[i])

    return total

# second_sum : String -> Number
# Determines the sum of every other digit in _card_number_, starting at the rightmost digit


def second_sum(card_number):

    # Accumulator
    total = 0
    card_length = len(card_number)

    for i in range(0, card_length, 2):
        index = (card_length - 1) - i
        total += int(card_number[index])

    return total


# print_card_type : String -> _
# Prints the type of card that _card_number_ represents, or "invalid"
# N.B. Card number consists of only digits


def print_card_type(card_number):
    # Use regex to determine if digits are in correct positions

    # AMEX: Contains 15 digits; first 2 are 34 or 37
    amex = re.fullmatch("34\d{13}|37\d{13}", card_number)
    mastercard = re.fullmatch("51\d{14}|52\d{14}|53\d{14}|54\d{14}|55\d{14}|", card_number)
    visa = re.fullmatch("4\d{12}|4\d{15}", card_number)

    if amex:
        print("AMEX")
    elif mastercard:
        print("MASTERCARD")
    elif visa:
        print("VISA")
    else:
        print("INVALID")


main()