assembler : main.o macros.o utils.o
	gcc -g -ansi -Wall -pedantic main.o macros.o -lm -o assembler 
main.o : main.c main.h 
	gcc -c -ansi -Wall -pedantic main.c -lm -o main.o
macros.o : macros.c macros.h
	gcc -c -ansi -Wall -pedantic macros.c -lm -o macros.o
utils.o : utils.c utils.h
	gcc -c -ansi -Wall -pedantic utils.c -lm -o utils.o
