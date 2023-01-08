import sys
import os
import time

# load argument
args = sys.argv
if len(args) < 2:
    print("Error: No argument given")
    exit()

# check if argument is file
name = ""
if args[1][0] == "\"":
    # multiple arguments in quotes
    for i in range(1, len(args)):
        name += args[i]
        if args[i][-1] == "\"":
            break
else:
    # single argument
    name = args[1]

# if argument is file, load file
bfcode = ""
with open(name, "r") as f:
    bfcode = f.read()


if not bfcode or bfcode == "":
    print("Error: No code in file")
    exit()

ccode = "// compiled from brainfuck code\n"
ccode += "#include <stdio.h>\n"
ccode += "#include <string.h>\n"
ccode += "int main() {\n"
ccode += "    char memory[30000];\n"
ccode += "    memset(memory, 0, 30000);\n"
ccode += "    char *ptr = memory;\n"

# convert brainfuck code to c code
for c in bfcode:
    if c == ">":
        ccode += "    ptr++;\n"
    elif c == "<":
        ccode += "    ptr--;\n"
    elif c == "+":
        ccode += "    ++*ptr;\n"
    elif c == "-":
        ccode += "    --*ptr;\n"
    elif c == ".":
        ccode += "    putchar(*ptr);\n"
    elif c == ",":
        ccode += "    *ptr = getchar();\n"
    elif c == "[":
        ccode += "    while (*ptr) {\n"
    elif c == "]":
        ccode += "    }\n"

ccode += "    return 0;\n"
ccode += "}"

# write c code to file
cname = name + ".c"
with open(cname, "w") as f:
    f.write(ccode)

# compile c code
os.system("gcc " + cname + " -o \"" + name.rsplit(".", maxsplit=1)[0] +"\"")




