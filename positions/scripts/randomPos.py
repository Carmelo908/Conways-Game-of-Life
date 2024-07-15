# Script for testing purposes. Better functionality for drawing positions will 
# be added in the future. Use this for generating random positions given a 
# width, a height and a rate of cells. Print the content to a file from the
# command line. 
from sys import argv, exit
from json import dumps
from random import randint

if len(argv) < 4:
    print(
        "The required arguments don't match with the amount of given argument. "
        "Enter a width, a height and a rate of alive cells between 1 and 100"
    )
    exit()
elif int(argv[3]) > 100 or int(argv[3]) <= 0:
    print("The cell rate is out of bounds, enter a number between 1 and 100")
    exit()

width = int(argv[1])
height = int(argv[2])

cellRate = int(argv[3])

position: list[list[int]] = [
    [
      (1 if randint(1, 100) in range(1, cellRate + 1) else 0) for _ in range(width)
    ] 
    for _ in range(height)
]

jsonString: str = "[\n"
for i in range(len(position)):
  jsonString += "  " + dumps(position[i]) + (
     ",\n" if i != (len(position) - 1) else "\n")
jsonString += "]\n"

print(jsonString)