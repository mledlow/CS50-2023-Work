import cs50

while True:
    height = cs50.get_int('Height: ')
    if height > 0 and height < 9:
        break

for i in range(height):  # general for loop for iterating through pyramid
    for j in range(height):  # for loop for left pyramid (ascending)
        if (i + j) >= (height - 1):
            print('#', end='')  # prints a block or a white space
        else:
            print(' ', end='')
    print('  ', end='')  # print a 2 character whitespace between half pyramids
    for k in range(height):  # for loop for right pyramid
        if (i >= k):
            print('#', end='')  # prints a block or white space

    print('')  # basically just prints a new line so we go down one

