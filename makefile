assembler : main.o macros.o utils.o
	gcc -g -ansi -Wall -pedantic main.o macros.o utils.o -lm -o assembler 
main.o : main.c
	gcc -c -ansi -Wall -pedantic main.c -o main.o
macros.o : macros.c macros.h
	gcc -c -ansi -Wall -pedantic macros.c -o macros.o
utils.o : utils.c utils.h
	gcc -c -ansi -Wall -pedantic utils.c -o utils.o
