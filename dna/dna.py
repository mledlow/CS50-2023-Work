import csv
import sys


def main():

    # TODO: Check for command-line usage
    if len(sys.argv) != 3:
        print('Usage: python dna.py csvfile.csv textfile.txt')
        sys.exit(1)

    databaseFile = open(sys.argv[1])
    database = csv.DictReader(databaseFile)
    sequenceFile = open(sys.argv[2])
    sequence = sequenceFile.read()

    firstRow = database.fieldnames
    list = []

    for i in range(1, len(firstRow)):  # should check starting at the first not-names piece of fieldnames
        longestMatch = longest_match(sequence, firstRow[i])
        # should append a list with the name of the DNA sequence and its highest number of matches
        list.append([f'{firstRow[i]}', longestMatch])

    # TODO: Check database for matching profiles
    for row in database:
        for column in range(0, len(list)):
            if int(row[list[column][0]]) != list[column][1]:
                break
            if column == (len(list) - 1):  # then we got to the end, so that's our matching profile
                print(row['name'])
                sequenceFile.close()
                databaseFile.close()
                sys.exit(0)  # exit because we got our guy

    print('No match')
    sequenceFile.close()
    databaseFile.close()
    return


def longest_match(sequence, subsequence):
    """Returns length of longest run of subsequence in sequence."""

    # Initialize variables
    longest_run = 0
    subsequence_length = len(subsequence)
    sequence_length = len(sequence)

    # Check each character in sequence for most consecutive runs of subsequence
    for i in range(sequence_length):

        # Initialize count of consecutive runs
        count = 0

        # Check for a subsequence match in a "substring" (a subset of characters) within sequence
        # If a match, move substring to next potential match in sequence
        # Continue moving substring and checking for matches until out of consecutive matches
        while True:

            # Adjust substring start and end
            start = i + count * subsequence_length
            end = start + subsequence_length

            # If there is a match in the substring
            if sequence[start:end] == subsequence:
                count += 1

            # If there is no match in the substring
            else:
                break

        # Update most consecutive matches found
        longest_run = max(longest_run, count)

    # After checking for runs at each character in seqeuence, return longest run found
    return longest_run


main()
