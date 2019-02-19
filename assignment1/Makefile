
CC=gcc

all: main.o
	$(CC) main.o lex.o code_gen.o name.o -o main

main.o: code_gen.o
	$(CC) -c main.c -o main.o

code_gen.o : lex.o name.o
	$(CC) -c code_gen.c -o code_gen.o

name.o: lex.o
	$(CC) -c name.c -o name.o

lex.o :
	$(CC) -c lex.c -o lex.o

clean:
	rm *.asm *.out *.o main
