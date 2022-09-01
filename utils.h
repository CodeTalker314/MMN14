#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <math.h>

extern char symbols[32];



unsigned int bits(unsigned int str, int start, int end);
char * binaryToBase(char *binary);
char * decimalToBase(int num);
int binaryToDecimal(char *binary);
char * decimalToBinary(int num);
int endOfLine(char *line);
char* concat( char *s1, char *s2);
