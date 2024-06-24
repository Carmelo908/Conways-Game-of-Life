# Script for printing a position to the console

from sys import argv, exit
from json import load

if len(argv) < 1: 
  print("Error: no file name given")
  exit()

with open(argv[1], "r") as inputFile:
  position: list[list[int]] = load(inputFile)

  for row in position:
    for cell in row:
      print(" " + ("@" if cell == 1 else "-"), end="")
    print()

