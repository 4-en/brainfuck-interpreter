# converts string to brainfuck code output

import sys
import math

s = ""

for i in range(1, len(sys.argv)):
    s += sys.argv[i] + " "

# remove trailing space
s = s[:-1]

if len(s) == 0:
    print("No string given")
    exit()

# convert string to brainfuck code

# first, convert string to ascii values
vals = [ord(c) for c in s]
# find lowest value
lowest = min(vals)

# find number with lowest total difference
#lambda x: sum([x**2 - v**2 for v in vals])
#lambda xd: sum([xd / math.sqrt(xd**2 - v**2) for v in vals ])
# idk

# find next square number
square = 1
while square**2 < lowest:
    square += 1
# set lowest value to square
lowest = square**2
# set all other values to difference
vals = [v - lowest for v in vals]

# use loop to set lowest value to all cells
code = "+" * square + "[>" + "+" * square + "<-]>" + "[" + ">+" * len(vals) + "<"*len(vals) + "-]"
# pointer is now before first cell
# use loop to set all other values and print
for v in vals:
    code += ">" + "+" * v + "."

# set pointer to next free cell
code += ">"

# set all cells to zero
cellsToZero = len(vals) + 2
code += "<[-]" * cellsToZero

# pointer is now on first cell

print(code)