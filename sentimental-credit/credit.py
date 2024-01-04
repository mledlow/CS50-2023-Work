import cs50


def main():
    num = cs50.get_int('Number: ')
    digits = number_of_digits(num)
    isVisa = check_visa(num, digits)
    isAmex = check_amex(num, digits)
    isMastercard = check_mastercard(num, digits)  # checks all types of cards to see which is valid

    if isVisa:
        print('VISA')
    elif isAmex:
        print('AMEX')
    elif isMastercard:
        print('MASTERCARD')
    else:
        print('INVALID')  # prints what type of card the input is


def number_of_digits(number):
    if number >= 1000000000000 and number < 10000000000000:
        return 13

    if number >= 100000000000000 and number < 1000000000000000:
        return 15

    if number >= 1000000000000000 and number < 10000000000000000:
        return 16  # return number of digits on credit card

    return 0  # if not one of these, then return 0 as it is invalid


def check_visa(number, digits):
    if digits != 16 and digits != 13:  # if not 16 or 13, not visa
        return False

    newNumber = 0

    if digits == 16:
        newNumber = int(number / 1000000000000000)
    else:
        newNumber = int(number / 1000000000000)

    if newNumber % 10 != 4:  # if last number isn't 4, then it's not visa
        return False

    if luhn_algorithm(number, digits):
        return True

    return False


def check_amex(number, digits):
    if digits != 15:
        return False
    if int(number / 100000000000000) % 10 != 3:
        return False
    if int(number / 10000000000000) % 10 != 4 and int(number / 10000000000000) % 10 != 7:
        return False
    if luhn_algorithm(number, digits):  # checks certain digits and then checks luhn
        return True
    return False


def check_mastercard(number, digits):
    if digits != 16:
        return False
    if int(number / 1000000000000000) % 10 != 5:
        return False
    if int(number / 100000000000000) % 10 > 5 or int(number / 100000000000000) % 10 == 0:
        return False
    if luhn_algorithm(number, digits):  # checks certain digits and then checks luhn
        return True
    return False


def luhn_algorithm(number, digits):
    luhnSum = 0
    luhnModulus = 0
    for i in range(1, digits + 1):
        luhnModulus = number % 10  # getting the modulus at the digit we are at
        if i % 2 == 0:  # if i is even, do this (more complicated blegh)
            luhnModulus *= 2
            if (luhnModulus >= 10):
                luhnSum += (luhnModulus % 10)
                luhnModulus = int(luhnModulus / 10)
                luhnSum += (luhnModulus % 10)  # adds the first and second digit (separately) to the sum
            else:
                luhnSum += luhnModulus
        else:
            luhnSum += luhnModulus  # if an odd number of the card, just add modulus to sum (nice and easy)

        number /= 10  # slashes the last digit off of the number, so that we look at a new one
        number = int(number)

    if luhnSum % 10 == 0:
        return True

    return False


main()