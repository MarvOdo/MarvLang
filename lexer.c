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

// convert single char into token code
long get_char_token(char c)
{
    if (c == '{') {return 3;}
    else if (c == '}') {return 4;}
    else if (c == '(') {return 5;}
    else if (c == ')') {return 6;}
    else if (c == ',') {return 7;}
    else if (c == ';') {return 8;}
    else if (c == '=') {return 9;}
    else if (c == '+') {return 10;}
    else if (c == '-') {return 11;}
    else if (c == '*') {return 12;}
    else if (c == '/') {return 13;}
    else if (c == '&') {return 14;}
    else if (c == '|') {return 15;}
    else if (c == '^') {return 16;}
    else if (c == '~') {return 17;}
    else {return -1;} //unknown char / not expected
}

// convert keyword string to code
long get_string_token(char *read, int end_index)
{
    char copy[end_index];
    memcpy(copy, read+1, (size_t)end_index);
    if (!strcmp(copy, "undefined")) {return 0;}
    else if (!strcmp(copy, "return")) {return 1;}
    else if (!strcmp(copy, "int")) {return 2;}
    else if (!strcmp(copy, ">>")) {return 18;}
    else if (!strcmp(copy, "<<")) {return 19;}
    else {return -1;} //not keyword, but identifier
}

// Finite state machine implementation for the lexer
struct TokenList *lexer(FILE *fptr)
{
    struct TokenList *result = malloc(sizeof(struct TokenList));
    result->size = 0;
    result->capacity = 16;
    result->array = malloc(sizeof(union Token) * 16);

    char c; //current char being read
    char current_read[32]; //current reading, max 30-char identifier, 1st char for I or L, last for '\0'
    int counter; //count what char we are on in current_read
    enum State current_state = SOURCE;
    union Token current_token;

    while ((c = fgetc(fptr)) != EOF)
    {
        switch (current_state)
        {
            case SOURCE:
                current_token.code = get_char_token(c);
                if (current_token.code != -1) //if single-char-builtin-token
                {
                    result->array[result->size] = current_token;
                    result->size++;
                }
                else if (isalpha(c)) //identifier or keyword
                {
                    current_state = STRING;
                    current_read[1] = c;
                    counter = 2;
                }
                else if (isdigit(c)) //reading a number / a literal
                {
                    current_state = NUMBER;
                    current_read[1] = c;
                    counter = 2;
                }
                else if (c == '<' || c == '>') //looking for multi-character built-in operators
                {
                    current_state = OTHER;
                    current_read[1] = c;
                    counter = 2;
                }
                else if (isspace(c)) //ignore whitespace
                {
                    break;
                }
                else
                {
                    fprintf(stderr, "LEXER ERROR: Unexpected character 0x%x (%c)\n", c, c);
                    exit(1);
                }
                break;
            
            case STRING:
                if (isalnum(c) || c == '_')
                {
                    if (counter <= 30) // reading still under 30 chars total
                    {
                        current_read[counter] = c;
                        counter++;
                        break;
                    }
                    else // string too long, throw error
                    {
                        current_read[0] = ' ';
                        current_read[31] = '\0';
                        fprintf(stderr, "LEXER ERROR: Unexpectedly long identifier/string%s...\n", current_read);
                        exit(1);
                    }
                }
                else if (isspace(c)) //end string, ignore whitespace
                {
                    current_state = SOURCE;
                    current_read[counter] = '\0';
                    current_token.code = get_string_token(current_read, counter);
                    if (current_token.code != -1) //if builtin keyword or multi-char operator
                    {
                        result->array[result->size] = current_token;
                        result->size++;
                        break;
                    }
                    else //identifier
                    {
                        current_read[0] = 'I';
                        current_token.value = malloc(sizeof(char) * (counter + 1));
                        current_token.value = memcpy(current_token.value, current_read, (size_t)(counter + 1));
                        result->array[result->size] = current_token;
                        result->size++;
                        break;
                    }
                }
                else //non-identifier char found, non-whitespace so need to consider it
                {
                    current_state = SOURCE;
                    current_read[counter] = '\0';
                    current_token.code = get_string_token(current_read, counter);
                    if (current_token.code != -1) //if built-in keyword
                    {
                        result->array[result->size] = current_token;
                        result->size++;
                    }
                    else //identifier
                    {
                        current_read[0] = 'I';
                        current_token.value = malloc(sizeof(char) * (counter + 1));
                        current_token.value = memcpy(current_token.value, current_read, (size_t)(counter + 1));
                        result->array[result->size] = current_token;
                        result->size++;
                    }
                    current_token.code = get_char_token(c); //deal with ending char
                    if (current_token.code != -1)
                    {
                        result->array[result->size] = current_token;
                        result->size++;
                        break;
                    }
                    else
                    {
                        fprintf(stderr, "LEXER ERROR: Unexpected character 0x%x (%c)\n", c, c);
                        exit(1);
                    }
                }
            
            case NUMBER:
                if (isdigit(c))
                {
                    if (counter <= 30) // reading still under 30 chars total
                    {
                        current_read[counter] = c;
                        counter++;
                        break;
                    }
                    else // number is too many digits to even be represented by long in decimal
                    {
                        current_read[0] = ' ';
                        current_read[31] = '\0';
                        fprintf(stderr, "LEXER ERROR: Unexpectedly long literal%s...\n", current_read);
                        exit(1);
                    }
                }
                else if (isspace(c)) //ending num reading by whitespace
                {
                    current_state = SOURCE;
                    current_read[counter] = '\0';
                    current_read[0] = 'L';
                    current_token.value = malloc(sizeof(char) * (counter + 1));
                    current_token.value = memcpy(current_token.value, current_read, (size_t)(counter + 1));
                    result->array[result->size] = current_token;
                    result->size++;
                    break;
                }
                else //ending num reading by non-whitespace char
                {
                    current_state = SOURCE;
                    current_read[counter] = '\0';
                    current_read[0] = 'L';
                    current_token.value = malloc(sizeof(char) * (counter + 1));
                    current_token.value = memcpy(current_token.value, current_read, (size_t)(counter + 1));
                    result->array[result->size] = current_token;
                    result->size++;
                    current_token.code = get_char_token(c);
                    if (current_token.code != -1)
                    {
                        result->array[result->size] = current_token;
                        result->size++;
                        break;
                    }
                    else
                    {
                        fprintf(stderr, "LEXER ERROR: Unexpected character 0x%x (%c)\n", c, c);
                        exit(1);
                    }
                }
            
            case OTHER:
                if (c == '<' || c == '>')
                {
                   if (counter <= 2) // reading still under 2 chars total
                    {
                        current_read[counter] = c;
                        counter++;
                        break;
                    }
                    else // triple <<< or >>> or random mix of the two
                    {
                        current_read[0] = ' ';
                        current_read[3] = '\0';
                        fprintf(stderr, "LEXER ERROR: Unexpectedly long operator%s\n", current_read);
                        exit(1);
                    } 
                }
                else if (isspace(c))
                {
                    current_state = SOURCE;
                    current_read[counter] = '\0';
                    current_token.code = get_string_token(current_read, counter);
                    if (current_token.code != -1) //if multi-char operator like << or >>
                    {
                        result->array[result->size] = current_token;
                        result->size++;
                        break;
                    }
                    else
                    {
                        current_read[0] = ' ';
                        fprintf(stderr, "LEXER ERROR: Unexpected operator%s\n", current_read);
                        exit(1);
                    }
                }
                else
                {
                    current_state = SOURCE;
                    current_read[counter] = '\0';
                    current_token.code = get_string_token(current_read, counter);
                    if (current_token.code != -1) //if built-in operator like << or >>
                    {
                        result->array[result->size] = current_token;
                        result->size++;
                    }
                    else //error
                    {
                        current_read[0] = ' ';
                        fprintf(stderr, "LEXER ERROR: Unexpected operator%s\n", current_read);
                        exit(1);
                    }
                    current_token.code = get_char_token(c); //deal with ending char
                    if (current_token.code != -1)
                    {
                        result->array[result->size] = current_token;
                        result->size++;
                        break;
                    }
                    else
                    {
                        fprintf(stderr, "LEXER ERROR: Unexpected character 0x%x (%c)\n", c, c);
                        exit(1);
                    }
                }
        }
        if ((result->size + 1) == result->capacity) //after each token addition, check if resizing needed
        {
            result->capacity *= 2;
            result->array = realloc(result->array, sizeof(union Token) * result->capacity);
        }
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
    //right now just prints token codes or values in order
    //FUTURE: will use tokens to make syntax tree
    for (int i = 0; i < token_stream->size; i++)
    {
        if (token_stream->array[i].code <= 100)
        {
            printf("Token Code: %ld\n", token_stream->array[i].code);
        }
        else
        {
            printf("Token Value: %s\n", token_stream->array[i].value);
        }
    }
    return 0;
}