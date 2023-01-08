import sys
import numpy as np

class Interpreter:
    def __init__(self):
        self.memory = np.zeros(30000, dtype=np.uint8)
        self.pointer = 0
        self.tokens = { ">": self.incrementPointer,
                        "<": self.decrementPointer,
                        "+": self.incrementValue,
                        "-": self.decrementValue,
                        ".": self.output,
                        ",": self.input,
                        "[": self.loopStart,
                        "]": self.loopEnd
                        }

        self.skipToLoopEnd = 0
        self.code = ""
        self.current = 0

    def run(self):
        while self.current < len(self.code):
            self.expression()

    def evaluate(self, source: str):
        for token in source:
            if token in self.tokens:
                self.addExpression(token)
        self.run()

    def addExpression(self, token: str):
        self.code += token


    def expression(self):
        token = self.code[self.current]
        cmd = self.tokens.get(token)

        if False:
            # log function
            print(f"Expression: {token} -> {cmd.__name__}")

        if self.skipToLoopEnd > 0:
            if token == "[":
                self.skipToLoopEnd += 1
            elif token == "]":
                self.skipToLoopEnd -= 1
        else:
            cmd()

        self.current+=1


    def incrementValue(self):
        self.memory[self.pointer] += 1

    def decrementValue(self):
        self.memory[self.pointer] -= 1

    def incrementPointer(self):
        self.pointer += 1

    def decrementPointer(self):
        self.pointer -= 1

    def output(self):
        print(chr(self.memory[self.pointer]), end="")

    def input(self):
        # get first character of input
        c = input()
        if len(c) == 0:
            c = "\n"
        else:
            c = c[0]
        self.memory[self.pointer] = ord(c)

    def loopStart(self):
        if self.memory[self.pointer] == 0:
            self.skipToLoopEnd = 1

    def loopEnd(self):
        if self.memory[self.pointer] != 0:
            # go back to loop start
            skip = 1
            while skip != 0:
                self.current -= 1
                c = self.code[self.current]
                if c == "]":
                    # count loop ends to skip same amount of loop starts
                    skip += 1
                elif c == "[":
                    skip -= 1
                
        


def checkInput():
    if len(sys.argv) < 2:
        # go to input mode
        return None
    else:
        # go to file mode
        name = ""
        # get first argument or multiple arguments if they are in quotes
        if sys.argv[1][0] == "\"":
            for arg in sys.argv[1:]:
                name += arg + " "
            name = name[1:-1]
        else:
            name = sys.argv[1]

        # read file
        with open(name, "r") as file:
            code = file.read()
        return code

def main():
    interpreter = Interpreter()
    code = checkInput()

    if code is None:
        # input mode
        while True:
            code = input(">>> ")
            interpreter.evaluate(code)
    else:
        # file mode
        interpreter.evaluate(code)


if __name__ == "__main__":
    main()

