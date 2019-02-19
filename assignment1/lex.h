#define EOI		0	/* End of input			*/
#define SEMI		1	/* ; 				*/
#define PLUS 		2	/* + 				*/
#define TIMES		3	/* * 				*/
#define LP		4	/* (				*/
#define RP		5	/* )				*/
#define NUM_OR_ID	6	/* Decimal Number or Identifier */

#define MINUS   7 
#define DIV	8
#define IF	9
#define ELSE	10
#define WHILE	11
#define BEGIN	12
#define THEN	13
#define DO	14
#define END	15
#define EQUAL	16
#define LESS	17
#define GREATER	18
#define COLON	19
#define ID	20

extern char *yytext;		/* in lex.c			*/
extern int yyleng;
extern yylineno;

