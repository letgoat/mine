import math
import os
import time
import datetime
from datetime import date

print("Hello, World!")
#num = input("Enter a number: ")
#print("number now is: ", num)
print(math.pi)

def greet(name):
    """
    This function greets the user by name.
    """
    print("Hello, " + name + ". Good morning!")

greet("Felix")
print(greet.__doc__)

print("current woking directory is:", os.getcwd())
print(os.listdir())


class Bird:
    def __init__(self):
        print("Bird is ready")
    
    def whoisthis(self):
        print("Bird")

    def swim(self):
        print("Swim faster")

class Penguin(Bird):
    def __init__(self):
        print("Penguin is ready")
    
    def whoisthis(self):
        print("Penguin")

    def run(self):
        print("Run faster")

peggy = Penguin()
peggy.whoisthis()
peggy.swim()
peggy.run()

date_time = datetime.datetime.now()
print(date_time)
date_object = datetime.date.today()
print(date_object)

a = date(2025, 9, 26)
print(a)

print(time.time_ns())

print()

print(__name__)

total = 0
for i in range(1, 101):
    total += i
print(total)

total2 = 0
for i in range(1, 101):
    if i % 2 == 0:
        total2 += i
print(total2)
        
total3 = 0
for i in range(2, 101, 2):
    total3 += i
print(total3)

print(sum(range(2, 101, 2)))