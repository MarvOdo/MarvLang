/**
 * parser.c
 * Marvin Odobashi
 * Main implementation file for the parser of the MarvLang compiler
*/

#include "parser.h"

enum State
{
    S_SOURCE,
    S_FUNCDEF,
    S_STATEMENTS,
    S_ASSIGN,
    S_RETURN,
    S_OP
};

struct ParseNode *parser(struct TokenList *token_list)
{
    struct ParseNode *sourceNode = malloc(sizeof(struct ParseNode));
    sourceNode->nodeCode = SOURCE;
    sourceNode->num_children = 0;
    sourceNode->children = NULL;

    enum State currentState = S_SOURCE;
    union Token *tokenArray = token_list->array;

    //loop through token_list
    for (size_t i = 0; i < token_list->size; i++)
    {
        switch (currentState)
        {
            case S_SOURCE:
                //if we read "int *ID* (", then we have beginning of a function definition
                if ((tokenArray[i].code == 2) && (tokenArray[i+1].value[0] == 'I') && (tokenArray[i+2].code == 5))
                {
                    currentState = S_FUNCDEF;

                    //set up new function definition node
                    struct ParseNode *funcDefNode = malloc(sizeof(struct ParseNode));
                    funcDefNode->nodeCode = FUNCDEF;
                    funcDefNode->id = tokenArray[i+1].value;
                    funcDefNode->parent = sourceNode;

                    //set last child of source to this newest function definition node
                    *(sourceNode->children + sourceNode->num_children) = funcDefNode;
                    sourceNode->num_children++;

                    //move i by 2, since we looked 2 ahead already
                    i += 2;

                    break;
                }
                fprintf(stderr, "PARSER ERROR: Incorrect function declaration on document-level / statement found outside function\n");
                exit(1);
                break;
            
            case S_FUNCDEF:
                break;
        }

    }
}