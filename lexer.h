#ifndef LEXER_H
#define LEXER_H

#define MAX_KEYWORDS 20
#define MAX_TOKEN_SIZE 100
#include <stdio.h>
typedef enum 
{
    KEYWORD,
    OPERATOR,
    SPECIAL_CHARACTER,
    CONSTANT,
    INT_CONSTANT,
	CHAR_CONSTANT,
	STR_CONSTANT,
	REAL_CONSTANT,
	BIN_CONSTANT,
	HEX_CONSTANT,
	OCT_CONSTANT,
    IDENTIFIER,
    UNKNOWN_IDENTIFIER,
    UNKNOWN
} TokenType;

typedef struct 
{
    char lexeme[MAX_TOKEN_SIZE];
    TokenType type;
    char *file_name;
   FILE *file_fptr;
} Token;

void initializeLexer(char **argv,Token *lex_ptr);
Token getNextToken(Token *token); 
void categorizeToken(Token* token);
int isKeyword(const char* str);
int isOperator(const char* str);
int isSpecialCharacter( char ch);
int isConstant(const char* str,Token *token);
int isIdentifier(const char* str,Token *token);
void checkSpecialCharacters(Token *token); 

#endif
