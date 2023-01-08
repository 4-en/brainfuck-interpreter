# removes non valid characters from file

import sys

if len(sys.argv) < 2:
    # no arguments
    exit()

# get file
name = ""
# get first argument or multiple arguments if they are in quotes
if sys.argv[1][0] == "\"":
    for arg in sys.argv[1:]:
        name += arg + " "
    name = name[1:-1]
else:
    name = sys.argv[1]

# read file
code = ""
with open(name, "r") as file:
    code = file.read()

# remove non valid characters
newCode = ""
lineCounter = 0
for c in code:
    if c in "<>+-.,[]":
        newCode += c
        lineCounter += 1
        if lineCounter > 80:
            newCode += "\n"

# write to file
n, ext = name.rsplit(".", 1)
if not ext or ext == "":
    ext = "b"

newName = n + "_cleaned." + ext

with open(newName, "w") as file:
    file.write(newCode)
    