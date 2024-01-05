# MarvLang
## C-Like Toy Language and Compiler by Marvin Odobashi

# Table of Contents
## [Current Objectives](#current-objectives-1)
## [Overview](#overview-1)
## [MarvLang Documentation](#marvlang-documentation-1)

# Current Objectives
1. Start working on Syntactic Analysis and Semantic Analysis
2. Add more test source files

# Overview
I've been interested in how programming languages actually work and how they are
turned into machine code for a while now. Finally, I feel that I know enough to
get started on making a small toy language and compiler myself.
## Purpose
As I mentioned, this will be a toy language, and I do not expect it to be a full
or actually useful language. The main purpose is to have fun doing it, to 
practice what I already know, and to learn new skills.
## Description
MarvLang in itself will be a simple language, since the main purpose is
actually building the compiler for it. This means that I will only include a 
single data type: int. All functions will also need to return an int, and most
all statements inside a function (other than the return statement) will simply
be assignment and reassignment statements.

# MarvLang Documentation
## Data Type (int)
Signed 32-bit integer, using Two's Complement for negative number
representation. It can range from -2147483648 to 2147483647 (-2^31 to 2^31 - 1)
## MarvLang File Structure
A normal MarvLang file will include function definitions and the main driver
function *main*, which will act as the entrypoint of the program.
## Variables
Variables can be used in MarvLang. They can either just be declared without a
value to be initialized later, or they can be initialized as part of their 
declaration. Their values can also be reassigned:

    int var1;
    var1 = 4;
    var1 = 5;

The final value of var1 is 5. Variable names must start with a letter (upper or 
lower case), and can include any alphanumeric characters or underscore (_). The
variable name can be at most 30 characters.
## Comments
There are comments available in MarvLang. Line comments can be started with the 
'%' character. '%%%' will start or end a multiline comment, as seen below:

    int var1 = 2; % this is a single line comment
    %%% This is a
    multiline
    comment%%%
    return var1;

Comments are removed during compile-time.
## Function Definitions
Example of an int function's definition:

    int myFunction(int arg1, int arg2) {
        int var1 = arg1;
        int var2 = arg2;
        int var3 = var1 + var2;
        return var3;
    }

This example showcases the return type of the function (int), its name
(myFunction), its parameters (int arg1, int arg2), and its statements.
## Function Usage
A fuction can be used in the following manner to compute and return a value:

    int var4 = myFunction(2, 5);

This example showcases the int arguments 2 and 5 passed into myFunction, whose
resulting value is stored in var4.
## int main()
The name "main" is reserved, and cannot be used for other functions. If no
main function exists in a file, then there is no entry-point, and nothing will
be executed. The final value returned by main will be the final result of the
program / calculation.
## Statements (Within Functions)
Statements can only exist within an function definition. Statements generally
include variable declarations, variable value assignments, and return 
statements. Each one can be seen below:

    int var1;
    int var2 = 5;
    return var1;

Variable value reassignment is included in "variable value assignments."