# Script for testing purposes. Better functionality for drawing positions will 
# be added in the future. Use this for generating custom positions manually
# writing to the list declaration below. Also, select a file name.
from json import dumps

fileName = "../custom.json"

drawed: list[list[str]] = [
  ["-","-","-","-","-","-","-","-","-","-"],
  ["-","-","-","-","-","-","-","-","-","-"],
  ["-","-","-","-","-","-","-","-","-","-"],
  ["-","-","-","-","-","-","-","-","-","-"],
  ["-","-","-","-","-","-","-","-","-","-"],
  ["-","-","-","-","-","-","-","-","-","-"],
  ["-","-","-","-","-","-","-","-","-","-"],
  ["-","-","-","-","-","-","-","-","-","-"],
  ["-","-","-","-","-","-","-","-","-","-"],
  ["-","-","-","-","-","-","-","-","-","-"],
]

position: list[list[int]] = [
  [
    1 if cell == "#" else 0 for cell in row 
  ] for row in drawed
]

jsonString: str = ""

for i in range(len(position)):
  jsonString += "  " + dumps(position[i]) + (
     ",\n" if i != (len(position) - 1) else "\n")

with open(fileName, "w") as file:
  file.write("[\n")
  file.write(jsonString)
  file.write("]\n")