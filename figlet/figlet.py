from pyfiglet import Figlet
import sys
import cs50
import random

figlet = Figlet()
list = figlet.getFonts()

if len(sys.argv) == 1: #if 0 arguments, output text in random font
    string = cs50.get_string('Input: ')
    figlet.setFont(font=list[random.randint(0, len(list))])
    print('Output: ')
    print(figlet.renderText(string))
elif len(sys.argv) == 3: #if 2 arguments, output text in given font
    if sys.argv[1] != '-f' and sys.argv[1] != '-font': #if the first argument is not f or font, invalid
        print("Invalid usage")
        sys.exit(1)
    elif sys.argv[2] in list:
        string = cs50.get_string('Input: ')
        figlet.setFont(font=sys.argv[2])
        print('Output: ')
        print(figlet.renderText(string))
    else:
        print("Invalid usage") #if argv[2] is not in list, font doesn't exist
        sys.exit(1)
else:
    print("Invalid usage") #if argv is not 0 or 2 arguments, invalid usage
    sys.exit(1)

