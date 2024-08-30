#include <stdio.h>
#include<stdlib.h>
#include "lexer.h"
static const char *str[] = {
    "KEYWORD",
    "OPERATOR",
    "SPECIAL_CHARACTER",
    "CONSTANT",
    "INT_CONSTANT",
	"CHAR_CONSTANT",
	"STR_CONSTANT",
	"REAL_CONSTANT",
	"BIN_CONSTANT",
    "HEX_CONSTANT",
	"OCT_CONSTANT",
    "IDENTIFIER",
    "UNKNOWN_IDENTIFIER",
    "UNKNOWN"
    };
int main(int argc, char *argv[]) 
{

    Token token,*lex_ptr;
    lex_ptr = (Token*)malloc(sizeof(Token));
    if (lex_ptr == NULL) {
        printf("ERROR: Memory allocation failed for lex_ptr\n");
        return 1;
    }
    if (argc!=2)
    {
        printf("ERROR: INVALID ARGUMENTS:\n");
        printf("USAGE<./lexical_Analiser><source.c>\n");
        return 1;
    }
    else
    initializeLexer(argv,lex_ptr);

    printf("%5s%15s\n","Token:","type:");
    while ((token = getNextToken(lex_ptr)).type != UNKNOWN) 
    {
       printf("%5s\t\t=> %-20s \n", token.lexeme,str[token.type]);
    }
    checkSpecialCharacters(lex_ptr); 

    return 0;
}
