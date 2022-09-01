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
FILE* PreReadFile(char *filename, struct Macros *head);
FILE* WritePreFile(char *filename,struct Macros *tail);
