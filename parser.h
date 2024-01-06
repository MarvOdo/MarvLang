/**
 * parse.h
 * Marvin Odobashi
 * Header file for the parser (syntactic + semantic analysis) of the compiler
*/

#ifndef __PARSER_H__
#define __PARSER_H__

#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "lexer.h"

enum NodeType
{
    SOURCE, //child nodes are all the function definitions of the file
    FUNCDEF, //child nodes are parameters; last child is a "STATEMENTS" node; id is name of function
    MAINDEF, //child nodes are parameters; last child is a "STATEMENTS" node; id is "main"
    PARAM, //id is name of parameter
    STATEMENTS, //child nodes are all the statements in a function
    ASSIGNSTMT, //id is name being assigned, single child is an expression
    RETSTMT, //single child is expression being returned
    CONSTANT, //value stores the value of the literal
    VARIABLE, //id is name of variable
    OPERATION //opcode is type of operation, children are things operated on
};

enum OpType
{
    ADD,
    SUB,
    MULT,
    DIV,
    AND,
    OR,
    XOR,
    NOT,
    RSHIFT,
    LSHIFT,
    FUNCCALL
};

/**
 * struct ParseNode - serves as a single node/leaf of the parse tree to be 
 * generated by the parser() function.
*/
struct ParseNode
{
    enum NodeType nodeCode; //stores type of this node
    enum OpType opCode; //stores type of operation, only used by OPERATION nodes

    //used for name in function declaration, for parameter names, and for variable names
    char *id; //used to store names where applicable 

    //used for constant nodes
    int value;

    struct ParseNode *parent; //pointer to parent node
    struct ParseNode **children; //array of pointers to child nodes, used differently for each type of node
    size_t num_children; //stores number of children
};

/**
 * Parameters: struct TokenList *token_list - pointer input stream of tokens from lexer
 * Returns: struct ParseTree* - pointer to a ParseTree struct
 * 
 * Description: Will read the tokens in order from token_list and generate the
 * parse tree as perscribed by the following regex / context-free grammar:
 * 
 * For reference, * means "0 or more times"
 * ? means "0 or 1 times"
 * + means "1 or more times"
 * | means "or"
 * 
 * 
 * Any MarvLang file is of the following form:
 * [funcdef]+ [maindef]
 * 
 * With:
 * [funcdef] - [type] [id]([[param] [, [param]]*]?) {
 *                [statement]+
 *            }
 * 
 * [maindef] - [type] main([[param] [, [param]]*]?) {
 *                 [statement]+
 *             }
 * 
 * [type] - int
 * 
 * [id] - [a-zA-Z][a-zA-Z0-9_]*
 * 
 * [param] - [type] [id]
 * 
 * [statement] - [[retstmt] | [assignstmt]]
 * 
 * [retstmt] - return [expr];
 * 
 * [assignstmt] - [var] = [expr];
 * 
 * [var] - [type]? [id]
 * 
 * [expr] - [[id] | [lit] | [op] | [funccall]]
 * 
 * [lit] - [0-9]+
 * 
 * [op] - [expr] [+ | - | * | / | & | | | ^ | ~ | >> | <<] [expr]
 * 
 * [funccall] - [id]([[expr] [, [expr]]*]?)
*/
struct ParseNode *parser(struct TokenList *token_list);

#endif