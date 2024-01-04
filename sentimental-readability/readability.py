import cs50


def main():
    text = cs50.get_string('Text: ')
    letters = count_letters(text)
    words = count_words(text)
    sentences = count_sentences(text)
    index = round((0.0588 * letters * 100 / words) - (0.296 * sentences * 100 / words) - 15.8)
    if (index >= 16):
        print('Grade 16+')  # if greater than 16, this is higher than college level
    elif (index < 1):
        print('Before Grade 1')  # if lower than 1, then this is pre grade school
    else:
        print(f"Grade {index}")  # prints calculated index if neither of these


def count_letters(text):
    letterCount = 0
    for i in range(len(text)):
        letter = text[i]
        if letter.isalpha() == True:
            letterCount += 1  # if the current place in text is alphabet, increase letter count
    return letterCount


def count_words(text):
    wordCount = 0
    for i in range(len(text)):
        if text[i] == ' ':
            wordCount += 1   # if a blank space, then we can assume a word has ended, add to count

    return wordCount + 1  # last word won't end in a space


def count_sentences(text):
    sentenceCount = 0
    for i in range(len(text)):
        if text[i] == '.' or text[i] == '?' or text[i] == '!':
            sentenceCount += 1

    return sentenceCount


main()
