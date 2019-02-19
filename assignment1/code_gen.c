#include <stdio.h>
#include <ctype.h>
#include "lex.h"
#include "code_gen.h"

char    *factor     ( void );
char    *term       ( void );
char    *expression ( void );

extern char *newname( void       );
extern void freename( char *name );

statements()
{
    
    char *tempvar;

    while( !match(EOI) )
    {    

        /*  statements -> id := expression*/

        if (match(NUM_OR_ID)){
            char given_var[30];
            int temp = 0;
            while(temp<yyleng){
                given_var[temp] = *(yytext+temp);
                temp++;
            }
            given_var[temp]='\0';
            advance();

            if(match(COLON)){
                advance();
                if(match(EQUAL)){

                    advance();
                    tempvar = expression();
                    printf("_%s=%s\n",given_var,tempvar);
                }
                else{
                    fprintf(stderr,"%d: Missing Equal\n",yylineno);
                }
            }
            else{
                fprintf(stderr,"%d: Missing Colon\n",yylineno);
            }
        }

        /*  statements -> if expr then stmt */

        else if(match(IF)){

            advance();
            printf("if(\n");
            tempvar = expression();
            printf(")if_cond_end\n");
            if(match(THEN)){
                advance();
                printf("then{\n");
                statements();
                printf("}end_if\n");
                return ;
            }
            else{
                fprintf(stderr,"%d: Missing then\n",yylineno);
            }
        }

        /*  statements -> while expr do stmt */

        else if(match(WHILE)){
            advance();
            printf("while(\n");
            tempvar = expression();
            printf(")while_cond_end\n");
            if(match(DO)){
                advance();

                printf("do{\n");
                statements();
                printf("}end_while\n");
                return;
            }
            else{
                fprintf(stderr,"%d: Missing do\n",yylineno);
            }
        }
        
        /*  statements -> begin opt_stmts end */

        else if(match(BEGIN)){

            advance();
            printf("begin\n");
            opt_stmts();
            if(match(END)){
                advance();
                printf("end\n");
            }
            else{
                fprintf(stderr,"%d: Missing end\n",yylineno);
            }
            return;
        }
        else {
            return;
        }
        if( match( SEMI ) )
            advance();
        else
            fprintf( stderr, "%d: Inserting missing semicolon\n", yylineno );
        freename( tempvar );
    }
}

char    *expression()
{

    char  *tempvar, *tempvar2;

    tempvar = term();
    while( match( PLUS ) || match( MINUS ) || match(EQUAL) || match(GREATER) || match(LESS))
    {

        if(match(PLUS)){
            advance();
            tempvar2 = term();
            printf("%s += %s\n", tempvar, tempvar2 );
        }
        else if(match(MINUS)){
            advance();
            tempvar2 = term();
            printf("%s -= %s\n", tempvar, tempvar2 );
        }
        else if(match(EQUAL)){
            advance();
            tempvar2 = term();
            printf("%s=(%s == %s) \n",tempvar , tempvar, tempvar2 );
        }
        else if(match(GREATER)){
            advance();
            tempvar2 = term();
            printf("%s=(%s > %s) \n", tempvar, tempvar, tempvar2 );

        }
        else if(match(LESS)){
            advance();
            tempvar2 = term();
            printf("%s=(%s < %s)\n", tempvar , tempvar, tempvar2 );
        }

        freename( tempvar2 );
    }
    return tempvar;
}

char    *term()
{
    char  *tempvar, *tempvar2 ;

    tempvar = factor();
    while( match( TIMES ) || match(DIV))
    {
      if(match(TIMES)){
        advance();
        tempvar2 = factor();
        printf("%s *= %s\n", tempvar, tempvar2 );
        freename( tempvar2 );
      }
      else{
        advance();
        tempvar2 = factor();
        printf("%s /= %s\n", tempvar, tempvar2 );
        freename( tempvar2 );
      }

    }

    return tempvar;
}

char    *factor()
{
    char *tempvar;

    if( match(NUM_OR_ID) )
    {
	/* Print the assignment instruction. The %0.*s conversion is a form of
	 * %X.Ys, where X is the field width and Y is the maximum number of
	 * characters that will be printed (even if the string is longer). I'm
	 * using the %0.*s to print the string because it's not \0 terminated.
	 * The field has a default width of 0, but it will grow the size needed
	 * to print the string. The ".*" tells printf() to take the maximum-
	 * number-of-characters count from the next argument (yyleng).
	 */
        if(isdigit(yytext[0])==0)
          printf("%s=_%0.*s\n", tempvar = newname(), yyleng, yytext );
        else
          printf("%s=%0.*s\n", tempvar = newname(), yyleng, yytext );
        advance();
    }
    else if( match(LP) )
    {
        advance();
        tempvar = expression();
        if( match(RP) )
            advance();
        else
            fprintf(stderr, "%d: Mismatched parenthesis\n", yylineno );
    }
    else
	fprintf( stderr, "%d: Number or identifier expected\n", yylineno );

    return tempvar;
}

/* opt_stmts → stmt_list |є
stmt_list → stmt_list; stmt | stmt */

opt_stmts()
{
    while(!match(END)&&!match(EOI))
        {
            statements();
        }
    if(match(EOF)){
        fprintf( stderr, "%d: End of file reached no END found\n", yylineno );
    }
    return ;
}
