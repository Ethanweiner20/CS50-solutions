from cs50 import get_string

# Not necessary, but definined a main function (more clean with everyone wrapped in a function)
def main():
    name = get_string("What is your name? \n")
    print("hello, " + name)


main()