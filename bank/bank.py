from cs50 import get_string

greeting = get_string('Greeting: ').strip()
list = greeting.split(',')
if (list[0].lower() == 'hello'):
    print('$0')
elif (list[0].startswith('h') or list[0].startswith('H')):
    print('$20')
else:
    print('$100')