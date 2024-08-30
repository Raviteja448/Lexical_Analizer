#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "lexer.h"

static const char* keywords[MAX_KEYWORDS] = {
	"int", "float", "return", "if", "else", "while", "for", "do", "break", "continue",
	"char", "double", "void", "switch", "case", "default", "const", "static", "sizeof", "struct"
};

#define STR_SIZE (sizeof(str) / sizeof(str[0]))

static const char* operators = "+-*/%=!<>|&";
static const char* specialCharacters = ",;{}()[]";
static int line=1;
static int op_count=0;
static int cp_count=0;
static int os_count=0;
static int cs_count=0;
static int of_count=0;
static int cf_count=0;
static int line_count_p=0;
static int line_count_s=0;
static int line_count_f=0;

//printf("-------------------------------------------------------------------------------------------------------------------\n");
/*function defination for initilisation*/
void initializeLexer(char **argv,Token *lex_ptr)
{
	char str[50];

	if (argv[1]!=NULL)
	{
		strcpy(str,argv[1]);
		if (strstr(str,".c"))
			lex_ptr->file_name=str;
		lex_ptr->file_fptr=fopen(lex_ptr->file_name,"r");
		if (lex_ptr->file_fptr==NULL)
			printf("ERROR: Pointer Pointing to NULL \n");
		else
			printf("Succesfully Opended Source file: \n");
	}
} 
//printf("-------------------------------------------------------------------------------------------------------------------\n");
/*Function to get the next token from the source file*/
Token getNextToken(Token *lex_ptr) 
{
	//Token token;
	int ch;
	int index = 0;
	 if(ch==EOF)
	{
		lex_ptr->type=UNKNOWN;//if it is end of file end return unknown
		return *lex_ptr;
	}
	// Skip the spaces and preproceser directive
	while ((ch = fgetc(lex_ptr->file_fptr)) != EOF) 
	{
		if (isspace(ch)) 
			continue;
		if (ch == '#') {
			// Skip the entire line
			while ((ch = fgetc(lex_ptr->file_fptr)) != EOF && ch != '\n');
			continue;
		}
		break;
	}

	//reading input for keyword or Identifier
	if (isalpha(ch))
	{
		do 
		{
			lex_ptr->lexeme[index++] = ch;
			ch = fgetc(lex_ptr->file_fptr);
		} while (isalnum(ch)|| ch=='_'|| strchr(operators,ch)==0 && strchr(specialCharacters,ch)==0 && ch != ' ');//rules for identifier

		ungetc(ch, lex_ptr->file_fptr);//moving one character into back
		lex_ptr->lexeme[index] = '\0';//assigining null with its end
		categorizeToken(lex_ptr);
		return *lex_ptr;
	}

	//reading input for operator
	if (strchr(operators, ch)) 
	{
		lex_ptr->lexeme[index++] = ch;//character is an operater
		lex_ptr->lexeme[index] = '\0';
		categorizeToken(lex_ptr);
		//token.type = OPERATOR;
		return *lex_ptr;
	}

	//reading input for special character
	if (strchr(specialCharacters, ch))
	{
		lex_ptr->lexeme[index++] = ch;//character is an special character
		lex_ptr->lexeme[index] = '\0';
		categorizeToken(lex_ptr);
		//token.type = SPECIAL_CHARACTER;
		return *lex_ptr;
	}
	//reading input for character constant
	if (ch == '\'')
	{
		lex_ptr->lexeme[index++]=ch;
		while((ch=fgetc(lex_ptr->file_fptr))!='\'' && ch!= EOF)
		{
			lex_ptr->lexeme[index++]=ch;
		}
		lex_ptr->lexeme[index++] = ch;//storing " ' "
		lex_ptr->lexeme[index] = '\0';
		categorizeToken(lex_ptr);
		//token.type = CHAR_CONSTANT;
		return *lex_ptr;
	}
	// //reading input for string constant
	if (ch == '"') {
    lex_ptr->lexeme[index++] = ch;
    while ((ch = fgetc(lex_ptr->file_fptr)) != '"' && ch != EOF && ch != ';') {
        lex_ptr->lexeme[index++] = ch;
    }
    lex_ptr->lexeme[index++] = ch;
    lex_ptr->lexeme[index] = '\0';
	categorizeToken(lex_ptr);
    //token.type = STR_CONSTANT;
    return *lex_ptr;
}

	// Check for binary, octal, or hexadecimal constant
	if (ch == '0') //if value start with 0
	{
		lex_ptr->lexeme[index++] = ch;
		ch = fgetc(lex_ptr->file_fptr);

		if (ch == 'b' || ch == 'B'|| ch=='x'|| ch=='X')
		{
			// Binary constant
			lex_ptr->lexeme[index++] = ch;
			ch = fgetc(lex_ptr->file_fptr);
			while (isdigit(ch)) {
				lex_ptr->lexeme[index++] = ch;
				ch = fgetc(lex_ptr->file_fptr);
			}
			ungetc(ch,  lex_ptr->file_fptr);
			lex_ptr->lexeme[index] = '\0';
			categorizeToken(lex_ptr);
			//token.type = CONSTANT;
			return *lex_ptr;
		} 
		else
		ungetc(ch,  lex_ptr->file_fptr);//if it is integer go back one character
		
	}
	//reading input for integer constant or double or float
	if (isdigit(ch))
	{
		do {
			lex_ptr->lexeme[index++] = ch;
			//printf("token = %s \n",lex_ptr->lexeme);
			ch = fgetc(lex_ptr->file_fptr);
		} while (isdigit(ch)|| ch== '.' || ch=='e'|| ch=='E'|| ch=='f' || ch=='F' || ch == 'L' || ch== 'l' );
		ungetc(ch, lex_ptr->file_fptr);  
		lex_ptr->lexeme[index] = '\0';  
		
		categorizeToken(lex_ptr);
		//token.type = CONSTANT;
		return *lex_ptr;
	}
}
//printf("-------------------------------------------------------------------------------------------------------------------\n");
/* categeraisation function  */
void categorizeToken(Token* lex_ptr)
{
	//printf("token = %s \n",lex_ptr->lexeme);
	if (isKeyword(lex_ptr->lexeme))
		lex_ptr->type = KEYWORD;
	else if (isConstant(lex_ptr->lexeme, lex_ptr));
		//lex_ptr->type = CONSTANT;
	else if (isOperator(lex_ptr->lexeme))
		lex_ptr->type = OPERATOR;
	else if (isSpecialCharacter(lex_ptr->lexeme[0]))
		lex_ptr->type = SPECIAL_CHARACTER;
	else if (isIdentifier(lex_ptr->lexeme, lex_ptr))
		lex_ptr->type = IDENTIFIER;
	else
		lex_ptr->type = UNKNOWN_IDENTIFIER;
}

//printf("-------------------------------------------------------------------------------------------------------------------\n");
/* checking is key_word function */
int isKeyword(const char* str)
{
	//printf("keyword_functon\n");
	for (int i=0;i<MAX_KEYWORDS;i++)
	{
		if (strcmp(str,keywords[i])==0){
			//printf("keyword = %s\n",str);
			return 1;
		}
	}
	
	return 0;
}
//printf("-------------------------------------------------------------------------------------------------------------------\n");
/* isoperater function */
int isOperator(const char* str)
{
	//printf("operater function\n");
	if (strchr(operators,str[0]))
		return 1;
	return 0;
}
//printf("-------------------------------------------------------------------------------------------------------------------\n");
/* isspecial character: */
int isSpecialCharacter(char ch)
{
	//printf("special char function\n");
	if (strchr(specialCharacters,ch))
		return 1;
	return 0;
}
//printf("-------------------------------------------------------------------------------------------------------------------\n");
/*isconstant function*/
int isConstant(const char* str,Token *token) 
{
	int i = 0;
	// Check for float constant
	int count1=0,count2=0;
	if (isdigit(str[0]))
	{
		i=1;
		while(isdigit(str[i]) || str[i] == '.' || str[i] == 'f' || str[i] == 'F') 
		{
			if (str[i] == 'f' || str[i] == 'F') 
				count1++;
			if (str[i]=='.')
				count2++;

			i++;
		}
		if ((count1==1 && (count2==1 || count2==0))|| (count2==1 && (count1 ==1 || count1==0)) && str[i] == '\0') 
		{
			token->type=REAL_CONSTANT;
			return 1;
		}
		else if (count1 > 1 || count2 > 1 )
		{ 
			printf("ERROR: \"%s\" Invalid REAL_CONSTANT\n", str);
			return 0;
		}
	}
	
	// Check for double constant
	count1 = 0,count2=0;
	if (isdigit(str[0]))
	{
		i=1;
		while (isdigit(str[i]) || str[i] == '.' || str[i] == 'l' || str[i] == 'L') 
		{
			if (str[i] == 'l' || str[i] == 'L') 
				count1++;
			if (str[i]=='.')
				count2++;
		i++;
		}
		if ((count1==1 && (count2==1 || count2==0))|| (count2==1 && (count1 ==1 || count1==0)) && str[i] == '\0')
		{
			token->type=REAL_CONSTANT;
			return 1;
			//printf("=> %10s :%20s",str,"REAL_CONSTANT");
		}
		else if (count1 > 1 || count2 > 1) 
		{
			//token->type=UNKNOWN_IDENTIFIER;
			printf("ERROR: \"%s\" Invalid REAL_CONSTANT\n", str);
			return 0;
		}
	}
	
	// Check for character constant
	if (str[0] == '\'' && str[2] == '\'' && str[3] == '\0')
	{
		token->type=CHAR_CONSTANT;
		return 1;
		//printf("=> %10s :%20s",str,"CHAR_CONSTANT");
	}
	else if (str[0] == '\'' && str[3] != '\0') 
	{
		//token->type=UNKNOWN_IDENTIFIER;
		printf("ERROR: \"%s\" INVALID CHARACTER_CONSTANT\n", str);
		return 0;
	}
	// Check for string constant
	if (str[0] == '"')
	{
		i = 1;
		while (str[i] != '"' && str[i] != '\0') 
		{
			if (str[i] == '\a' || str[i] == '\n' || str[i] == '\t' || str[i] == '\r') 
				printf(" Escape sequence \" %c \" \n", str[i]);

			i++;
		}
		if (str[i] == '"') 
		{
			token->type=STR_CONSTANT;
			return 1;
		}
		else 
		{
			//token->type=UNKNOWN_IDENTIFIER;
			printf("ERROR: %s Missing closing double quote\n",str);
			return 0;
		}

	}
	// Check for binary constant (prefix 0b or 0B)
	if (str[0] == '0' && (str[1] == 'b' || str[1] == 'B')) {
		i = 2;
		while (str[i] == '0' || str[i] == '1') {
			i++;
		}
		if (str[i] == '\0') {
			token->type=BIN_CONSTANT;
			return 1;
		} else {
			//token->type=UNKNOWN_IDENTIFIER;
			printf("ERROR: \" %s \" Invalid Binary value\n", str);
			return 0;
		}
	}
	// Check for hexadecimal constant
	if (str[0] == '0' && (str[1] == 'x' || str[1] == 'X')) 
	{
		i = 2;
		while (isxdigit(str[i])) 
			i++;
		if (strlen(str) == i) {
			token->type=HEX_CONSTANT;
			return 1;
		}
		else {
			printf("ERROR: \" %s \" Invalid HEXA value\n", str);
			return 0;
		}
	}
	// Check for octal constant
	if (str[0] == '0' && isdigit(str[1]))
	{
		i = 1;
		while (str[i] >= '0' && str[i] <= '7') 
			i++;
		if (strlen(str) == i)  {
			token->type=OCT_CONSTANT;
			return 1;
		}
		
	}
	i=0;
	// Check for integer constant
	while (isdigit(str[i]) && str[i] != '\0')
	 {
		i++;
	}
	if (strlen(str) == i)
	{
		//printf("const = %s\n",str);
		token->type=INT_CONSTANT;
		return 1;
	}
	else if (isdigit(str[0])){
			printf("ERROR: \" %s \" Invalid integer value\n", str);
			return 0;
		}
	return 0;

}
//printf("-------------------------------------------------------------------------------------------------------------------\n");
/*Identifier funtion*/
int isIdentifier(const char* str, Token *token)
{
      if (isalpha(str[0]) || str[0] == '_')
    {
        for (int i = 1; str[i] != '\0'; i++) 
        {
            if (!isalnum(str[i]) && str[i] != '_') {
                printf("ERROR: \" %s \" Invalid Identifier \n", str);
                return 0;
            }
        }
        return 1;
    }
    return 0;
}
//printf("-------------------------------------------------------------------------------------------------------------------\n");
/* checkSpecialCharacters function*/
void checkSpecialCharacters(Token *token) {
	rewind(token->file_fptr);
    int ch;
    while ((ch = fgetc(token->file_fptr)) != EOF) {
        if (ch == '\n') {
            line++;
        } else if (ch == '(') {
            op_count++;
            line_count_p = line;
        } else if (ch == ')') {
            op_count--;
            line_count_p = line;
        } else if (ch == '[') {
            os_count++;
            line_count_s = line;
        } else if (ch == ']') {
            os_count--;
            line_count_s = line;
        } else if (ch == '{') {
            of_count++;
            line_count_f = line;
        } else if (ch == '}') {
            of_count--;
            line_count_f = line;
        }
    }

    if (op_count != 0) {
        printf("Error: Mismatched parentheses '%c' at line %d\n", op_count > 0 ? ')' : '(', line_count_p);
    }
    if (os_count != 0) {
        printf("Error: Mismatched square brackets '%c' at line %d\n", os_count > 0 ? ']' : '[', line_count_s);
    }
    if (of_count != 0) {
        printf("Error: Mismatched curly braces '%c' at line %d\n", of_count > 0 ? '}' : '{', line_count_f);
    }
}
//printf("*******************************************--COMPLETED --**************************************************************\n");



