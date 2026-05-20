# Script for testing purposes. Better functionality for drawing positions will 
# be added in the future. Use this for generating random positions.
# Prints the content to stdout.
from json import dumps
from random import randint

cellsList: list[tuple[int, int]] = []

for y in range(-50, 50):
  for x in range(-50, 50):
    cell: bool = randint(1, 10) > 5 # 50% of alive cells
    if not cell: continue
    cellsList.append((x, y)) 
            
print(dumps(cellsList))