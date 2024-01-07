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
    S_PARAM,
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
    struct ParseNode *currentParent = sourceNode;

    //loop through token_list
    for (size_t i = 0; i < token_list->size; i++)
    {
        switch (currentState)
        {
            case S_SOURCE:
                //if read read "int ID ("
                if ((tokenArray[i].code == 2) && (tokenArray[i+1].value[0] == 'I') && (tokenArray[i+2].code == 5))
                {
                    currentState = S_FUNCDEF;

                    struct ParseNode *funcdefNode = malloc(sizeof(struct ParseNode));

                    //initialize function definition node
                    funcdefNode->nodeCode = FUNCDEF;
                    funcdefNode->id = tokenArray[i+1].value;
                    funcdefNode->children = NULL;
                    funcdefNode->num_children = 0;
                    funcdefNode->parent = sourceNode;
                    funcdefNode->next_sibling = NULL; //newest funcdef node, so last child for now

                    //update sourceNode children by adding a function definition node
                    if (sourceNode->num_children == 0)
                    {
                        sourceNode->children = funcdefNode;
                    }
                    else
                    {
                        struct ParseNode *currChild = sourceNode->children;
                        while (currChild->next_sibling != NULL)
                        {
                            currChild = currChild->next_sibling;
                        }
                        currChild->next_sibling = funcdefNode;
                    }
                    sourceNode->num_children++;

                    currentParent = funcdefNode;
                    i += 2;

                    break;
                }
                else
                {
                    fprintf(stderr, "PARSER ERROR: Incorrect syntax in global scope level code, potential incorrerct function declaration");
                    exit(1);
                }
            
            case S_FUNCDEF:
                //if we read ") {" there are no parameters
                if ((tokenArray[i].code == 6) && (tokenArray[i+1].code == 3))
                {
                    currentState = S_STATEMENTS;

                    struct ParseNode *statementsNode = malloc(sizeof(struct ParseNode));

                    //initialize function definition node
                    statementsNode->nodeCode = STATEMENTS;
                    statementsNode->children = NULL;
                    statementsNode->num_children = 0;
                    statementsNode->parent = currentParent;

                    //update funcDef children by adding this statements node
                    if (currentParent->num_children == 0)
                    {
                        currentParent->children = statementsNode;
                    }
                    else
                    {
                        struct ParseNode *currChild = currentParent->children;
                        while (currChild->next_sibling != NULL)
                        {
                            currChild = currChild->next_sibling;
                        }
                        currChild->next_sibling = statementsNode;
                    }
                    currentParent->num_children++;

                    currentParent = statementsNode;
                    i += 1;

                    break;
                }
                //else if we read "int ID", we found a parameter
                else if ((tokenArray[i].code == 2) && (tokenArray[i+1].value[0] == 'I'))
                {
                    currentState = S_PARAM;

                    struct ParseNode *paramNode = malloc(sizeof(struct ParseNode));

                    //initialize function definition node
                    paramNode->nodeCode = VARIABLE;
                    paramNode->id = tokenArray[i+1].value;
                    paramNode->parent = currentParent;

                    //update funcDef children by adding this statements node
                    if (currentParent->num_children == 0)
                    {
                        currentParent->children = paramNode;
                    }
                    else
                    {
                        struct ParseNode *currChild = currentParent->children;
                        while (currChild->next_sibling != NULL)
                        {
                            currChild = currChild->next_sibling;
                        }
                        currChild->next_sibling = paramNode;
                    }
                    currentParent->num_children++;

                    i += 1;

                    break;
                }
                else
                {
                    fprintf(stderr, "PARSER ERROR: Incorrect syntax in global scope level code, potential incorrerct function declaration");
                    exit(1);
                }
            
            case S_PARAM:
                //if we read ") {" there are no more parameters left to read
                if ((tokenArray[i].code == 6) && (tokenArray[i+1].code == 3))
                {
                    currentState = S_STATEMENTS;

                    struct ParseNode *statementsNode = malloc(sizeof(struct ParseNode));

                    //initialize function definition node
                    statementsNode->nodeCode = STATEMENTS;
                    statementsNode->children = NULL;
                    statementsNode->num_children = 0;
                    statementsNode->parent = currentParent;

                    //update funcDef children by adding this statements node
                    if (currentParent->num_children == 0)
                    {
                        currentParent->children = statementsNode;
                    }
                    else
                    {
                        struct ParseNode *currChild = currentParent->children;
                        while (currChild->next_sibling != NULL)
                        {
                            currChild = currChild->next_sibling;
                        }
                        currChild->next_sibling = statementsNode;
                    }
                    currentParent->num_children++;

                    currentParent = statementsNode;
                    i += 1;

                    break;
                }
                //else if we read ", int ID", we found another parameter
                else if ((tokenArray[i].code == 7) && (tokenArray[i+1].code == 2) && (tokenArray[i+2].value[0] == 'I'))
                {
                    struct ParseNode *paramNode = malloc(sizeof(struct ParseNode));

                    //initialize function definition node
                    paramNode->nodeCode = VARIABLE;
                    paramNode->id = tokenArray[i+1].value;
                    paramNode->parent = currentParent;

                    //update funcDef children by adding this statements node
                    if (currentParent->num_children == 0)
                    {
                        currentParent->children = paramNode;
                    }
                    else
                    {
                        struct ParseNode *currChild = currentParent->children;
                        while (currChild->next_sibling != NULL)
                        {
                            currChild = currChild->next_sibling;
                        }
                        currChild->next_sibling = paramNode;
                    }
                    currentParent->num_children++;

                    i += 2;

                    break;
                }
                else
                {
                    fprintf(stderr, "PARSER ERROR: Incorrect syntax in global scope level code, potential incorrerct function declaration");
                    exit(1);
                }
            
            case S_STATEMENTS:
                //if we read "int ID =", it is a variable declaration / initialization
                if ((tokenArray[i].code == 2) && (tokenArray[i+1].value[0] == 'I') && (tokenArray[i+2].code == 9))
                {
                    //TO BE CONTINUE HERE
                }
        }
    }
}