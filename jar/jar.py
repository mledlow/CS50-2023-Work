def main():
    jar = Jar()
    print(str(jar.capacity))
    jar.deposit(2)
    print(str(jar))
    jar.withdraw(1)
    print(str(jar))

class Jar:
    def __init__(self, capacity=12):
        if capacity < 0:
            raise ValueError
        else:
            self._capacity = capacity
            self._size = 0 #initializes capacity as what's passed in as size as 0 (no cookies)


    def __str__(self):
        return str(self.size)

    def deposit(self, n):
        if self.size + n > self.capacity:
            raise ValueError
        else:
            self._size += n

    def withdraw(self, n):
        if self.size - n < 0:
            raise ValueError
        else:
            self._size -= n

    @property
    def capacity(self):
        return self._capacity
    @property
    def size(self):
        return self._size

main()