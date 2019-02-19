#include "lex.h"
#include <stdio.h>
#include <ctype.h>


char* yytext = ""; /* Lexeme (not '\0'
                      terminated)              */
int yyleng   = 0;  /* Lexeme length.           */
int yylineno = 0;  /* Input line number        */

int lex(void){

   static char input_buffer[1024];
   char        *current;

   current = yytext + yyleng; /* Skip current
                                 lexeme        */

   while(1){       /* Get the next one         */
      while(!*current ){
         /* Get new lines, skipping any leading
         * white space on the line,
         * until a nonblank line is found.
         */

         current = input_buffer;
         if(!gets(input_buffer)){
            *current = '\0' ;

            return EOI;
         }
         ++yylineno;
         while(isspace(*current))
            ++current;
      }
      for(; *current; ++current){
         /* Get the next token */
         yytext = current;
         yyleng = 1;
         switch( *current ){
	   case EOF: 
	    return EOI;
           case ';':
            return SEMI;
           case '+':
            return PLUS;
           case '-':
            return MINUS;
           case '*':
            return TIMES;
           case '/':
            return DIV;
           case '(':
            return LP;
           case ')':
            return RP;
           case ':':
            return COLON;
           case '=':
            return EQUAL;
            case '<':
            return LESS;
            case '>':
            return GREATER;
           case '\n':
           case '\t':
           case ' ' :
            break;
           default:
            if(!isalnum(*current))
               fprintf(stderr, "Not alphanumeric <%c>\n", *current);
            else{
               while(isalnum(*current))
                  ++current;
               yyleng = current - yytext;
               char temp[yyleng+1];
               memcpy( temp, yytext, yyleng );
               temp[yyleng] = '\0';
               // printf("%s",temp);
               if(strcmp(temp,"if")==0){
                // printf("%s",temp);
                return IF;
               }
               else if(strcmp(temp,"while")==0){
                return WHILE;
               }
               else if(strcmp(temp,"then")==0){
                return THEN;
               }
               else if(strcmp(temp,"begin")==0){
                return BEGIN;
               }
               else if(strcmp(temp,"do")==0){
                return DO;
               }
               else if(strcmp(temp,"end")==0){
                
                return END;
               }
               return NUM_OR_ID;
            }
            break;
         }
      }
   }
}


 /* Lookahead token  */
static int Lookahead = -1;
int match(int token){
   /* Return true if "token" matches the
      current lookahead symbol.                */

   if(Lookahead == -1)
      Lookahead = lex();

   return token == Lookahead;
}

void advance(void){
/* Advance the lookahead to the next
   input symbol.                               */
    Lookahead = lex();
}
