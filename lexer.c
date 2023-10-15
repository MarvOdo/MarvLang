/**
 * lexer.c
 * Marvin Odobashi
 * Main implementation file for the lexer of the MarvLang compiler
*/
#include "lexer.h"

// Helper function to open file and return FILE* to it, otherwise throw error
// if file cannot be opened
FILE *read_file(char fname[])
{
    FILE *result = fopen(fname, "r");
    if (result != NULL)
    {
        return result;
    }
    fprintf(stderr, "INPUT ERROR: Could not open file at %s\n", fname);
    exit(1);
}



// Will be implemented as a Finite-State Machine
struct TokenList *lexer(FILE *fptr)
{
    struct TokenList *result = malloc(sizeof(struct TokenList));
    result->size = 0;
    result->capacity = 16;
    result->array = malloc(sizeof(union Token) * 16);

    char c; //current char for finite state machine
    char current_token[30]; //current reading, max 30-char identifier

    while ((c = fgetc(fptr)) != EOF)
    {
        //ADD FINITE STATE LOGIC
    }

    return result;
}

//main to be moved to other file once lexer is done
int main(int argc, char *argv[])
{
    FILE *code_stream;
    if (argc == 1)
    {
        code_stream = stdin;
    }
    else if (argc == 2)
    {
        code_stream = read_file(argv[1]);
    }
    else
    {
        fprintf(stderr, "INPUT ERROR: Too many command line arguments\n");
        exit(1);
    }
    struct TokenList *token_stream = lexer(code_stream);
    return 0;
}