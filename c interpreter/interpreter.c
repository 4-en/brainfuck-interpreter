// brainfuck interpreter in C with 64 bit memory cells

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// #define TURBO_MODE
// #define DEBUG
#define EXTENDED_MODE

const size_t FUNC_STORAGE_SIZE = 256;
const size_t MEMORY_CELL_SIZE = 30000;
const size_t INSTRUCTION_SIZE = 30000;

char globalCharBuffer[1024];
char* gcb = globalCharBuffer;

struct Interpreter
{

    // memory
    long long *memory;
    unsigned int mem_pointer;

    // instructions
    char *instructions;
    // current size of instructions array (pointer to next free space)
    unsigned int instructions_size;
    // current instruction pointer
    unsigned int instruction_pointer;

    // counter for loops
    int skips;

    // lock for run function
    int running;
};

// convert char to instruction
void (**functions)(struct Interpreter*);


// brainfuck instructions
void incrementPointer(struct Interpreter *interpreter)
{
#ifndef TURBO_MODE
    if (interpreter->mem_pointer >= MEMORY_CELL_SIZE-1)
    {
        printf("Memory pointer out of bounds\n");
        return;
    }
#endif

#ifdef DEBUG
    printf("incrementPointer\n");
#endif
    interpreter->mem_pointer++;
}

void decrementPointer(struct Interpreter *interpreter)
{
#ifndef TURBO_MODE
    if (interpreter->mem_pointer <= 0)
    {
        printf("Memory pointer out of bounds\n");
        return;
    }
#endif

#ifdef DEBUG
    printf("decrementPointer\n");
#endif
    interpreter->mem_pointer--;
}

void incrementValue(struct Interpreter *interpreter)
{
#ifdef DEBUG
    printf("incrementValue\n");
#endif
    interpreter->memory[interpreter->mem_pointer]++;
}

void decrementValue(struct Interpreter *interpreter)
{
#ifdef DEBUG
    printf("decrementValue\n");
#endif
    interpreter->memory[interpreter->mem_pointer]--;
}

void printValue(struct Interpreter *interpreter)
{
#ifdef DEBUG
    printf("printValue\n");
#endif
    printf("%c", interpreter->memory[interpreter->mem_pointer]);
}


void readValue(struct Interpreter *interpreter)
{
#ifdef DEBUG
    printf("readValue\n");
#endif
    // read char from stdin
    scanf("%s", gcb);
    interpreter->memory[interpreter->mem_pointer] = gcb[0];
}

void loopStart(struct Interpreter *interpreter)
{
#ifdef DEBUG
    printf("loopStart\n");
#endif
    if (interpreter->memory[interpreter->mem_pointer] == 0)
    {
        interpreter->skips = 1;

        while (interpreter->skips > 0)
        {
            interpreter->instruction_pointer++;
            if (interpreter->instructions[interpreter->instruction_pointer] == '[')
            {
                interpreter->skips++;
            }
            else if (interpreter->instructions[interpreter->instruction_pointer] == ']')
            {
                interpreter->skips--;
            }
        }
    }
}

void loopEnd(struct Interpreter *interpreter)
{
#ifdef DEBUG
    printf("loopEnd\n");
#endif

    if (interpreter->memory[interpreter->mem_pointer] != 0)
    {
        // go back to loop start
        interpreter->skips = 1;
        while (interpreter->skips > 0)
        {
            interpreter->instruction_pointer--;
            if (interpreter->instructions[interpreter->instruction_pointer] == '[')
            {
                interpreter->skips--;
            }
            else if (interpreter->instructions[interpreter->instruction_pointer] == ']')
            {
                interpreter->skips++;
            }
        }
    }
}

#ifdef EXTENDED_MODE
// create funtion to output memory value as int
void printMemory(struct Interpreter *interpreter)
{
    printf("Memory value: %d\n", interpreter->memory[interpreter->mem_pointer]);
}
#endif

int isPrintable(char c)
{
    return c >= 32 && c <= 126;
}

// create function to entire memory
void memoryDump(struct Interpreter *interpreter)
{

    // print memory pointer
    printf("Memory pointer: %08x -> %08x (%c)\n", interpreter->mem_pointer, interpreter->memory[interpreter->mem_pointer], isPrintable(interpreter->memory[interpreter->mem_pointer]) ? interpreter->memory[interpreter->mem_pointer] : '.');

    for (int i = 0; i < MEMORY_CELL_SIZE; i+=8)
    {
        
        sprintf(gcb, "%08x: %08x %08x %08x %08x %08x %08x %08x %08x     ", i, interpreter->memory[i], interpreter->memory[i+1], interpreter->memory[i+2], interpreter->memory[i+3], interpreter->memory[i+4], interpreter->memory[i+5], interpreter->memory[i+6], interpreter->memory[i+7]);

        printf("%s", gcb);

        // print printable characters
        sprintf(gcb, "%c %c %c %c %c %c %c %c", isPrintable(interpreter->memory[i]) ? interpreter->memory[i] : '.', isPrintable(interpreter->memory[i+1]) ? interpreter->memory[i+1] : '.', isPrintable(interpreter->memory[i+2]) ? interpreter->memory[i+2] : '.', isPrintable(interpreter->memory[i+3]) ? interpreter->memory[i+3] : '.', isPrintable(interpreter->memory[i+4]) ? interpreter->memory[i+4] : '.', isPrintable(interpreter->memory[i+5]) ? interpreter->memory[i+5] : '.', isPrintable(interpreter->memory[i+6]) ? interpreter->memory[i+6] : '.', isPrintable(interpreter->memory[i+7]) ? interpreter->memory[i+7] : '.');
        printf("%s\n", gcb);
        
        // after every 8th line, ask for confirmation to continue
        if (i % 64 == 0 && i != 0)
        {
            printf("\nContinue? (y/n): ");
            scanf("%s", gcb);
            if (gcb[0] == 'n')
            {
                break;
            }
            printf("\n");
        }
    }
    
    printf("\n");
}

void unknownInstruction(struct Interpreter *interpreter)
{
#ifdef DEBUG
    printf("Unknown instruction\n");
#endif
}

void run(struct Interpreter *interpreter)
{
    if(interpreter->running == 1)
    {
        printf("Interpreter is already running\n");
        return;
    }

    interpreter->running = 1;

    while (interpreter->instruction_pointer < interpreter->instructions_size)
    {
        functions[interpreter->instructions[interpreter->instruction_pointer]](interpreter);
        interpreter->instruction_pointer++;
    }

    interpreter->running = 0;
}

void addInstruction(struct Interpreter *interpreter, char instruction)
{

// remove safety checks for faster execution
#ifndef TURBO_MODE
    // check if instruction is valid
    if (functions[instruction] == functions[0])
    {
        printf("Unknown instruction\n");
        return;
    }

    // check if array is big enough

    if (interpreter->instructions_size >= INSTRUCTION_SIZE)
    {
        // no more space in array
        printf("No more space in instruction array\n");
        return;
    }
#endif

    interpreter->instructions[interpreter->instructions_size] = instruction;
    interpreter->instructions_size++;
}

void addInstructions(struct Interpreter *interpreter, char *instructions)
{
    for (int i = 0; instructions[i] != '\0'; i++)
    {
        addInstruction(interpreter, instructions[i]);
    }
}

struct Interpreter *initInterpreter()
{
    // allocate memory for interpreter
    struct Interpreter *interpreter = malloc(sizeof(struct Interpreter));

    // allocate memory for memory
    interpreter->memory = malloc(MEMORY_CELL_SIZE * sizeof(long long));
    memset(interpreter->memory, 0, MEMORY_CELL_SIZE * sizeof(long long));
    interpreter->mem_pointer = 0;

    // allocate memory for instructions
    interpreter->instructions = malloc(INSTRUCTION_SIZE * sizeof(char));
    interpreter->instructions_size = 0;
    interpreter->instruction_pointer = 0;

    interpreter->skips = 0;

    interpreter->running = 0;

    return interpreter;
}

void cleanUp(struct Interpreter *interpreter)
{
    // free allocated memory
    free(interpreter->memory);
    free(interpreter->instructions);
    free(interpreter);
}

int main(int argc, char *argv[])
{

    functions = malloc(FUNC_STORAGE_SIZE * sizeof(void *));

    // set all instructions to unknown
    for (int i = 0; i < FUNC_STORAGE_SIZE; i++)
    {
        functions[i] = &unknownInstruction;
    }

    // set known instructions

    functions['>'] = &incrementPointer;
    functions['<'] = &decrementPointer;
    functions['+'] = &incrementValue;
    functions['-'] = &decrementValue;
    functions['.'] = &printValue;
    functions[','] = &readValue;
    functions['['] = &loopStart;
    functions[']'] = &loopEnd;

#ifdef EXTENDED_MODE
    functions['i'] = &printMemory;
#endif

    struct Interpreter *interpreter = initInterpreter();


    
    if (argc < 2)
    {

        // print startup message
        printf("Brainfuck interpreter\n");
#ifdef EXTENDED_MODE
        printf("Extended mode enabled\n");
#endif
#ifdef TURBO_MODE
        printf("Turbo mode enabled\n");
#endif
#ifdef DEBUG
        printf("Debug mode enabled\n");
#endif
        
        /*
        ___________.._______
        | .__________))______|
        | | / /      ||
        | |/ /       ||
        | | /        ||.-''.|
        | |/         |/  _  \\
        | |          ||  `/,|
        | |          (\\\\`_.'|
        | |         .-`--'.|
        | |        /Y . . Y\\
        | |       // |   | \\\\
        | |      //  | . |  \\\\
        | |     ')   |   |   (`
        | |          ||'|||
        | |          || |||
        | |          || |||
        | |          || |||
        | |         / | | \\
        */
        
        printf("___________.._______\n");
        printf("| .__________))______|\n");
        printf("| | / /      ||\n");
        printf("| |/ /       ||\n");
        printf("| | /        ||.-''.|\n");
        printf("| |/         |/  _  \\\n");
        printf("| |          ||  `/,|\n");
        printf("| |          (\\\\`_.'|\n");
        printf("| |         .-`--'.|\n");
        printf("| |        /Y . . Y\\\n");
        printf("| |       // |   | \\\\\n");
        printf("| |      //  | . |  \\\\\n");
        printf("| |     ')   |   |   (`\n");
        printf("| |          ||'|||\n");
        printf("| |          || |||\n");
        printf("| |          || |||\n");
        printf("| |          || |||\n");
        printf("| |         / | | \\\n");
        printf("\n");
        // press help for help
        printf("Type 'help' for help\n\n");


        // get arguments from command line


        char line[4096];

        while (1)
        {
            // get line from command line on windows

            printf("\n\n");
            printf("Enter command: ");
            scanf("%4095s", line);

            // check if line is help
            if (strcmp(line, "help") == 0)
            {
                // print brainfuck tokens
                printf(">\tIncrement pointer\n");
                printf("<\tDecrement pointer\n");
                printf("+\tIncrement value\n");
                printf("-\tDecrement value\n");
                printf(".\tPrint value\n");
                printf(",\tRead value\n");
                printf("[\tLoop start\n");
                printf("]\tLoop end\n");
#ifdef EXTENDED_MODE
                printf("i\tPrint memory\n");
#endif
                printf("\n");
                printf("help\tPrint help\n");
                printf("memDump\tPrint memory dump\n");
                printf("exit\tExit program\n");
                printf("\n");
                continue;
            }

            // check if line is memDump
            if (strcmp(line, "memDump") == 0)
            {
                // print memory dump
                printf("Memory dump:\n");
                
                memoryDump(interpreter);

                continue;
            }

            // check if line is exit
            if (strcmp(line, "exit") == 0)
            {
                // exit program
                break;
            }


            // add line to instructions
            addInstructions(interpreter, line);

            // run interpreter
            run(interpreter);

            // reset line
            memset(line, 0, 1024);


        }

        return 0;
        
    } else {
        // get file name from command line
        char *file_name = argv[1];

        // open file
        FILE *file = fopen(file_name, "r");

        // check if file exists
        if (file == NULL)
        {
            printf("File does not exist\n");
            return 1;
        }

        // read file
        char *txt = malloc(1024);
        do {
            fgets(txt, 1024, file);
            addInstructions(interpreter, txt);
        } while (!feof(file));

        // close file
        fclose(file);

        // run interpreter
        run(interpreter);
    }

    

    cleanUp(interpreter);
}