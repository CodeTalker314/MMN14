#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <math.h>





unsigned int bits(unsigned int str, int start, int end);
char * binaryToBase(char *binary);
char * decimalToBase(int num);
int binaryToDecimal(char *binary);
char * decimalToBinary(int num);
int endOfLine(char *line);
char* concat( char *s1, char *s2);
char* asFile( const char* str1);
char* amFile( const char* str1);
char* textFile( const char* str1);
char* obFile( const char* str1);
char* extFile( const char* str1);
char* entFile( const char* str1);
