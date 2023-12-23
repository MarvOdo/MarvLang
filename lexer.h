/**
 * lexer.h
 * Marvin Odobashi
 * Header file for the lexer of the MarvLang compiler
*/

#ifndef __LEXER_H__
#define __LEXER_H__

#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>

/**
 * union Token - used to store tokens read by lexer, exentually to be made an
 * array of union Tokens. If the read token is an identifier, we will utilize
 * char* value, and calloc space for its value; The first char will be I in
 * order to identify that it is an identifier. For literals, similarly, char* 
 * value will be used, but the first char will be L. If instead we read 
 * keywords, we will utilize long code as described below.
 * 
 * Code = 0         -->   undefined
 * Code = 1         -->   return
 * Code = 2         -->   int
 * Code = 3         -->   {
 * Code = 4         -->   }
 * Code = 5         -->   (
 * Code = 6         -->   )
 * Code = 7         -->   ,
 * Code = 8         -->   ;
 * Code = 9         -->   =
 * Code = 10        -->   +
 * Code = 11        -->   -
 * Code = 12        -->   *
 * Code = 13        -->   /
 * Code = 14        -->   & (binary and)
 * Code = 15        -->   | (binary or)
 * Code = 16        -->   ^ (binary xor)
 * Code = 17        -->   ~ (binary not)
 * Code = 18        -->   >> (binary right shift)
 * Code = 19        -->   << (binary left shift)
 * Value[0] = 'I'   -->   identifier
 * Value[0] = 'L'   -->   literal
*/
union Token {
    char *value;
    long code;
};

/**
 * struct TokenList - used to store the final token stream outputted by the 
 * lexer. Implemeted as a resizing array, with size and capacity storing the 
 * current number of tokens in the list, and the current max capacity
*/
struct TokenList {
    union Token *array;
    size_t size;
    size_t capacity;
};

/**
 * Parameters: FILE* fpr - input file stream
 * Returns: struct TokenList* - pointer to array of Tokens
 * 
 * Description: Will read from the file stream fptr, and return the pointer to
 * array of tokens as described needed by the MarvLang parser.
*/
struct TokenList *lexer(FILE *fptr);

#endif