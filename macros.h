#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define MAX 100

struct Macros
{
char macroname[MAX];
char macrodata[MAX];
struct Macros* next;
};

int StartEndMacro(char line[]);
void InsertName(struct Macros *temp, char line[]);
void InsertContent(struct Macros *temp, FILE *fp);
int IsMacroCall(char line[], FILE *fpw, struct Macros *tail);
struct Macros* PreReadFile(char *filename, struct Macros *head);
void WritePreFile(char *filename, char *macrofile,struct Macros *tail);
void amRun( char* fullFileName , char* macroName , FILE* file );
