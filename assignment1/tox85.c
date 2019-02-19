#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "lex.h"

#define t0 02B0
#define t1 02B1
#define t2 02B2
#define t3 02B3
#define t4 02B4
#define t5 02B5
#define t6 02B6

#define SP 03E7  // the stack pointer

int line_no;    // line no in assembly file
int flag = -1;
FILE *fout;

int cond_stck[20],cond_stck_top=0; //stacks conditions for handle the nested cases
int if_stck[20],if_stck_top=0;  //stacks if for handle the nested cases
int while_stck[20],while_stck_top=0; //stacks while for handle the nested cases

int table_size=0;             // no of variables declared uptill now
char symbol_table[10][10];      // variable length at max 10, at max 10 varioables

void get_hex_line(int num, int bits)
{                                      // convert numbers to hexadecimal with 'bits' number of bits
    int temp,i;
    char hex[bits];
    char a;
    for(i=0;i<bits;i++)
    {
        temp=num%16;
        if(temp<10)
            a=48+temp;
        else
            a=55+temp;
        hex[i]=a;
        num=num/16;
    }
    for(i=bits-1;i>=0;i--)
        fprintf(fout,"%c",hex[i]);
    fprintf(fout,"H\n");
}


void _if(int i)                              //implemented if
{
    fprintf(fout,"LXI H, 02B%dH\n",i);
    fprintf(fout,"MOV A, M\n");
    fprintf(fout, "ADI 00H\n");
    fprintf(fout,"JZ xxxxH\n");             //add dummy address, which will be edited later using fseek
    if_stck[if_stck_top++]=ftell(fout)-6;
    line_no+=9;                             // 9 lines added in this function
}

void _endif()          //  go to prev. if to take the address
{
  fseek(fout,if_stck[--if_stck_top],SEEK_SET);
  get_hex_line(line_no, 4);
  fseek(fout,0,SEEK_END);
}

// add this cndition in while
void while_cond()
{
  cond_stck[cond_stck_top++]=line_no;
}


// give a address at begining
void _while(int i)
{
    fprintf(fout,"LXI H, 02B%dH\n",i);
    fprintf(fout,"MOV A, M\n");
    fprintf(fout, "ADI 00H\n");
    fprintf(fout,"JZ xxxxH\n");                   //add dummy address, which will be edited later using fseek
    while_stck[while_stck_top++]=ftell(fout)-6;
    line_no+=9;                                   // 9 lines added in this function
}

void while_end()
{
  fprintf(fout,"JMP ");
  get_hex_line(cond_stck[--cond_stck_top], 4);
  fprintf(fout,"\n");
  line_no+=3;
  fseek(fout,while_stck[--while_stck_top],SEEK_SET);
  get_hex_line(line_no,4);
  fseek(fout,0,SEEK_END);
}

// t1 += t2
void binary_plus(int i, int j)
{
    fprintf(fout,"LXI H, 02B%dH\n",j);
    fprintf(fout,"MOV B, M\n");
    fprintf(fout,"LXI H, 02B%dH\n",i);
    fprintf(fout,"MOV A, M\n");
    fprintf(fout,"ADD B\n");
    fprintf(fout,"STA 02B%dH\n",i);
    line_no+=12;                        // 12 lines added in this function
}

// t1 -= t2
void binary_subtract(int i, int j)
{
    fprintf(fout, "LXI H, 02B%dH\n", j);
    fprintf(fout, "MOV B, M\n");
    fprintf(fout, "LXI H, 02B%dH\n", i);
    fprintf(fout, "MOV A, M\n");
    fprintf(fout, "SUB B\n");
    fprintf(fout, "STA 02B%dH\n", i);
    line_no += 12;                      // 12 lines added in this function
}

// t1 *= t2
void multiply(int i, int j)
{
   fprintf(fout,"LXI H, 02B%dH\n",i);
   fprintf(fout,"MOV B, M\n");
    fprintf(fout,"LXI H, 02B%dH\n",j);
    fprintf(fout,"MOV C, M\n");
    fprintf(fout,"MVI A, 00H\n");
    fprintf(fout,"ADD B\n");
    fprintf(fout,"DCR C\n");
    fprintf(fout,"JNZ ");
    get_hex_line(line_no+10,4);
    fprintf(fout,"LXI H, 02B%dH\n",i);
    fprintf(fout,"MOV M, A\n");
    line_no+=19;                        // 19 lines added in this function
}

// t1 /= t2

void divide(int i, int j)
{
    fprintf(fout, "LXI H, 02B%dH\n", j);
    fprintf(fout, "MOV B, M        ;Get the dividend in B - reg.\n");
    fprintf(fout, "MVI C, 00H      ;Clear C - reg for quotient\n");
    fprintf(fout, "LXI H, 02B%dH\n", i);
    fprintf(fout, "MOV A, M        ;Get the divisor in A - reg\n");
    fprintf(fout, "NEXT: CMP B           ;Compare A - reg with register B.\n");
    fprintf(fout, "JC LOOP         ;Jump on carry to LOOP\n");
    fprintf(fout, "SUB B           ;Subtract A - reg from B - reg. \n");
    fprintf(fout, "INR C           ;Increment content of register C.\n");
    fprintf(fout, "JMP NEXT        ;Jump to NEXT \n");
    fprintf(fout, "LOOP: MOV A, C        ;Move Content of C - Reg to A - Reg\n");
    fprintf(fout, "STA 02B%dH        ;Store the quotient in memory \n", i);
    line_no += 23;              // 23 lines added in this function

}


void less_than(int i, int j)               //strictly <
{
    fprintf(fout, "LXI H, 02B%dH\n", j);
    fprintf(fout, "MOV B, M\n");
    fprintf(fout, "LXI H, 02B%dH\n", i);
    fprintf(fout, "MOV A, M\n");
    //ti <= tj
    fprintf(fout, "SUB B\n");
    fprintf(fout, "MVI A, 00H\n");
    fprintf(fout, "JP ");           //move to STA
    get_hex_line(line_no+19, 4);
    fprintf(fout, "JZ ");
    get_hex_line(line_no+19, 4);
    fprintf(fout, "MVI A, 01H\n");


    fprintf(fout, "STA 02B%dH\n", i);
    line_no += 22;              // 22 lines added in this function
}

void greater_than(int i, int j)               //strictly >
{
    fprintf(fout, "LXI H, 02B%dH\n", j);
    fprintf(fout, "MOV B, M\n");
    fprintf(fout, "LXI H, 02B%dH\n", i);
    fprintf(fout, "MOV A, M\n");
    //ti >= tj
    fprintf(fout, "SUB B\n");
    fprintf(fout, "MVI A, 00H\n");
    fprintf(fout, "JM ");           //move to STA
    get_hex_line(line_no+19, 4);
    fprintf(fout, "JZ ");
    get_hex_line(line_no+19, 4);
    fprintf(fout, "MVI A, 01H\n");
    fprintf(fout, "STA 02B%dH\n", i);
    line_no += 22;                  // 22 lines added in this function
}

void compare(int i, int j)
{
    fprintf(fout, "LXI H, 02B%dH\n", j);
    fprintf(fout, "MOV B, M\n");
    fprintf(fout, "LXI H, 02B%dH\n", i);
    fprintf(fout, "MOV A, M\n");
    //ti == tj
    fprintf(fout, "SUB B\n");
    fprintf(fout, "MVI A, 01H\n");
    fprintf(fout, "JZ ");           //move to STA
    get_hex_line(line_no+16, 4);
    fprintf(fout, "MVI A, 00H\n");
    fprintf(fout, "STA 02B%dH\n", i);
    line_no += 19;                  // 19 lines added in this function
}


int assign_case1(char *sub, int i)
{
    // _id=t1
    //check if id is present in symbol_table
    int j;
    if(table_size==10)
    {
        //max limit of id reached
        fprintf(stderr, "error: can't declare more variables\n");
        return 0;
    }
    for(j=0; j<table_size; j++)
    {
        if(strcmp(sub, symbol_table[j])==0)
        {
            break;         //we have to store value at 02CjH
        }
    }
    strcpy(symbol_table[j], sub);
    table_size++;
    fprintf(fout, "LXI H, 02B%dH\n", i);
    fprintf(fout, "MOV A, M\n");
    fprintf(fout, "STA 02C%dH\n", j);
    line_no += 7;           // 7 lines added in this function
    return 1;
}

int assign_case2(int i, char *sub)
{
    //ti := id
    int j;
    for(j=0; j<table_size; j++)
    {
        if(strcmp(sub, symbol_table[j])==0)
        {
            break;         //we have to store value at 02CjH
        }
    }
    if(j == table_size)
    {
        if(table_size == 10)
        {
            printf("table_size exceeded \n");
            return 0;
        }
        strcpy(symbol_table[table_size],sub);
        table_size++;
    }

    fprintf(fout, "LXI H, 02C%dH\n", j);
    fprintf(fout, "MOV A, M\n");
    fprintf(fout, "STA 02B%dH\n", i);
    line_no += 7;           // 7 lines added in this function
    return 1;
}

void assign_case3(int i, int j)
{
     //ti:=tj
    fprintf(fout,"LXI H, 02B%dH\n",j);
    fprintf(fout,"MOV B, M\n");
    fprintf(fout,"MOV A, B\n");
    fprintf(fout,"STA 02B%dH\n", i);
    line_no+=8;             // 8 lines added in this function
}


// ti := num
void assign_case4(int i, int num)
{
    fprintf(fout,"MVI A, ");
    get_hex_line(num,2);
    fprintf(fout, "\n");
    fprintf(fout,"STA 02B%dH\n", i);
    line_no+=5;             // 5 lines added in this function
}

int token_check(char *line)
{
    int i, j;
    
    if(strstr(line, "if(")!=NULL)
    {
    	flag = 1;
    	
        return IF;
    }
    else if(strstr(line, "while(")!=NULL)
    {
        flag = 2;
        while_cond();
        
        return WHILE;
    }
    else if(strstr(line, "end_while")!=NULL)
    {
        //we need to jump to start of while
        while_end();
        return WHILE;
    }
    else if(strstr(line, "end_if")!=NULL)
    {
        //_endif adds the jump address to the start of if
        _endif();
        return IF;         //just some non-zero value
    }
    //ignore the rest
    else if(strstr(line, "if_cond_end")!=NULL)
        return IF;
    else if(strstr(line, "while_cond_end")!=NULL)
        return WHILE;
    else if(strstr(line, "then")!=NULL)
        return THEN;
    else if(strstr(line, "do")!=NULL)
        return DO;
    else if(strstr(line, "begin")!=NULL)
        return BEGIN;
    else if(strstr(line, "end")!=NULL)
        return END;
    else if(strstr(line, " += ")!=NULL)
    {
        //ti += tj
        //call add(i, j)
        i = *(strstr(line, " += ") - 1) - '0';
        j = *(strstr(line, " += ") + 5) - '0';
        binary_plus(i, j);
        return PLUS;
    }
    else if(strstr(line, " -= ")!=NULL)
    {
        //ti -= tj
        i = *(strstr(line, " -= ") - 1) - '0';
        j = *(strstr(line, " -= ") + 5) - '0';
        binary_subtract(i, j);
        return MINUS;
    }
    else if(strstr(line, " *= ")!=NULL)
    {
        //ti *= tj
        i = *(strstr(line, " *= ") - 1) - '0';
        j = *(strstr(line, " *= ") + 5) - '0';
        multiply(i, j);
        return TIMES;
    }
    else if(strstr(line, " /= ")!=NULL)
    {
        //ti /= tj
        i = *(strstr(line, " /= ") - 1) - '0';
        j = *(strstr(line, " /= ") + 5) - '0';
        divide(i, j);
        return DIV;
    }
    else if(strstr(line, " < ")!=NULL)
    {
        // ti = (ti < tj)
        i = *(strstr(line, " < ") - 1) - '0';
        j = *(strstr(line, " < ") + 4) - '0';
        less_than(i, j);
        return LESS;
    }
    else if(strstr(line, " > ")!=NULL)
    {
        // ti = (ti > tj)
        i = *(strstr(line, " > ") - 1) - '0';
        j = *(strstr(line, " > ") + 4) - '0';
        greater_than(i, j);
        return GREATER;
    }
    else if(strstr(line, " == ")!=NULL)
    {
        //ti = (ti == tj)
        i = *(strstr(line, " == ") - 1) - '0';
        j = *(strstr(line, " == ") + 5) - '0';
        compare(i, j);
        if(flag==1){
          flag = -1;
          _if(i);
        }
        if(flag==2){
          flag=-1;
          _while(i);
        }
        return EQUAL;
    }

    else if(strstr(line, "=")!=NULL)
    {

    /*  4 possibilities
    *   id=t1
    *   t2=t1
    *   t1=num
    *   t1=identifier
    */

        if(line[0] == '_')
        {
            //id = ti
            j = *(strstr(line, "=") + 2) - '0';
            char sub[20];

            strncpy(sub, line+1, (strstr(line, "=") - line - 1) );
            sub[(strstr(line, "=") - line - 1)]='\0';
            if(assign_case1(sub, j)==0)
                return EOI;
        }
        else
        {
            i = *(strstr(line, "=") - 1) - '0';
            if( *(strstr(line, "=") + 1) == '_' )        //t1=_id
            {
                char sub[20], *temp;
                int j=0;
                temp = strstr(line, "=") + 2;
                while(*temp!='\n')
                {
                    sub[j++]=*temp;
                    temp+=1;
                }
                sub[j]='\0';
                if( assign_case2(i, sub) ==0)
                    return EOI;
            }
            else if( *(strstr(line, "=") + 1) == 't')        //t2=t1
            {
                j = *(strstr(line, "=") + 2) - '0';
                assign_case3(i, j);
            }
            else                                                //t1=number
            {
                int num=0;
                char *c = strstr(line, "=") + 1;
                for(; *c!='\n'; c++)
                {
                    num *= 10;
                    num += (*c - '0');
                }
                assign_case4(i, num);
            }
        }

        return COLON;
    }
    else                                //Return error if none found.
        return EOI;
}


int main(int argc, char* argv[])
{
    line_no=0;

    // Talking file name as input from terminal.
    if(argc <= 1)
    {
        fprintf(stderr, "Intermediate file must be provided as arguement.\n");
        return 0;
    }
    char const* const fileName = argv[1];
    FILE* inp = fopen(fileName, "r");
    fout = fopen("codex85.txt", "w+");
    char line[256];

    while (fgets(line, sizeof(line), inp))
    {

          fprintf(fout, "  ;%s\n", line);

          int key = token_check(line);

          //Exception error
          if (key == EOI)
          {
              printf("Error in Intermediate file, in line number: %s\n", line);
              return 0;
          }


      }

    // Halt line at the end of assembly file.
    fprintf(fout, "HLT\n");
    fclose(inp);
    fclose(fout);
    return 0;
}
