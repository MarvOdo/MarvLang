# MarvLang
## Mathematical Toy Language by Marvin Odobashi

# Table of Contents
## [Current Objectives](#current-objectives)
## [Overview](#overview)
## [MarvLang Documentation](#marvlang-documentation)
## [Potential Future Features](#potential-future-features)

# Current Objectives
1. Make Finite-State Machine Lexer
2. Make unit testing files

# Overview
I've been interested in how programming languages actually work and how they are
turned into machine code for a while now. Finally, I feel that I know enough to
get started on making a small toy language and compiler myself.
## Purpose
As I mentioned, this will be a toy language, and I do not expect it to be a full
or actually useful language. The main purpose is to have fun doing it, to 
practice what I already know, and to learn new skills.
## Description
MarvLang will be a language for computations / mathematics. This means that I
will not include data types such as "chars" or pointers. It will likely just be
some different number primitive data types (ints, longs, floats, doubles, etc.),
with derived data types such as "expressions" (equivalent to functions). It will
likely not have any graphical / visual output, but I do hope to make the 
standard math library rather extensive.
I plan to make it statically typed and allow casting directly between number 
types (casting from int to float <==> interpreting int's binary pattern as a 
float). It will also be, as mentioned previously, a compiled language.

More details to come in the future...

# MarvLang Documentation
## Data Types
The plan is to include numerical data types as well as functions (which I will
refer to as "expressions" from here on out).
### int
Signed 32-bit integer, using Two's Complement for negative number
representation. It can range from -2147483648 to 2147483647 (-2^31 to 2^31 - 1)
### undefined
This type is used for uninitialized variables, and other undefined values such
as the result of division by 0.
### *type1*(*type2* arg1, *type3* arg2, ...)
*type1* expression, takes in arguments of type *type2*, *type3*, etc. Returns a
*type1* value after sequentially evaluating its statements.

## MarvLang File Structure
A normal MarvLang file will include expression definitions and the main driver
expression *main*, which will act as the entrypoint of the program.
### Variables
Variables can be used in MarvLang. They can either just be declared without a
value to be initialized later, or they can be initialized as part of their 
declaration. Their values can also be reassigned:

    int var1;
    var1 = 4;
    var1 = 5;

The final value of var1 is 5. Variable names must start with a letter (upper or 
lower case), and can include any alphanumeric characters or underscore (_). The
variable name can be at most 30 characters.
### Comments
There are comments available in MarvLang. Line comments can be started with the 
'%' character. '%%%' will start or end a multiline comment, as seen below:

    int var1 = 2; % this is a single line comment
    %%% This is a
    multiline
    comment%%%
    return var1;

Comments are removed during compile-time.
### Expression Definitions
Example of an int expressions definition:

    int myExpression(int arg1, int arg2) {
        int var1 = arg1;
        int var2 = arg2;
        int var3 = var1 + var2;
        return var3;
    }

This example showcases the return type of the expression (int), its name
(myExpression), its parameters (int arg1, int arg2), and its statements.
### Expression Usage
An expression can be used in the following manner to compute and return a value:

    int var4 = myExpression(2, 5);

This example showcases the int arguments 2 and 5 passed into myExpression, whose
resulting value is stored in var4.
### *type1* main()
The main expression does not have a particular type to return; the type can be 
chosen by the programmer when writing in the main expression. The name "main" is
reserved, and cannot be used for other expressions. If no main expression exists
in a file, then there is no entry-point, and nothing will be executed. The final
value returned by main will be the final result of the program / calculation.
### Statements (Within Expressions)
Statements can only exist within an expression definition. Statements generally
include variable declarations, variable value assignments, and return 
statements. Each one can be seen below:

    int var1;
    int var2 = 5;
    return var1;

Variable value reassignment is included in "variable value assignments."

## Potential Future Features
### *type1*[]
Arrays of type *type1*, holding a given number of values of the same time, 
indexable, and iterable through For Each loops
### long
Signed 64-bit integer, using Two's Complement for negative number
representation. It can range from -9223372036854775808 to 9223372036854775807
(-2^63 to 2^63 - 1)
### uint
Unsigned 32-bit non-negative integer. It can range from 0 to 4294967295
(0 to 2^32 - 1)
### ulong
Unsigned 64-bit non-negative integer. It can range from 0 to
18446744073709551615 (0 to 2^64 - 1)
### float
Signed 32-bit precision decimal number. It uses 1 sign bit, 7 exponent bits, 
24 significand bits (as per IEEE 754 standards).
### double
Signed 64-bit precision decimal number. It uses 1 sign bit, 10 exponent bits, 
53 significand bits (as per IEEE 754 standards).
### rational
Signed 64-bit ratio of 2 32-bit ints.
### lrational
Signed 128-bit ratio of 2 64-bit longs.
### complex
64-bit number that holds two signed 32-bit ints (real and imaginary part)
### lcomplex
128-bit number that holds two signed 64-bit longs (real and imaginary part)
### Control Flow Statements
If, Else, If Else statements
### Loops
For, While, For Each Loops
### Global Variables
Variables defined outside of expressions; Cannot be reassigned; Need to be 
initialized when declared