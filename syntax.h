#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


#define MAX_LINE_LENGTH 80
#define MAX_LABEL_LENGTH 30
#define MAX 100


struct labels
{
	char labelName[MAX_LINE_LENGTH];
	char icAddress[MAX_LINE_LENGTH];
	char dcAddress[MAX_LINE_LENGTH];
	char addresEXT[MAX];
	int external;
	int entry;
	int code;
	struct labels* next;
};

char *savedWords[]={"mov","cmp","add","sub","not","clr","lea","inc","dec","jmp","bne","get","prn","jsr","rts","hlt","r0","r1","r2","r3","r4","r5","r6","r7"};

int LineIsEmpty(char line[],int index);
int LineIsComment(char line[]);
int firstFieldIsLabel(char line[]);
int isGuidance(char line[],int index);
int setLabelName( struct labels *currentle ,struct labels *head, char line[]);
int isDataLegal(char line[] , int index);
int isStringLegal(char line[],int index);
int isStructLegal(char line[],int index);
int LineIsEntry(char line[]);
int LineIsExtern(char line[]);
int LegalLabelForEntryAndExtern(char line[],int index);
int setNameExternAndEntry( struct labels *currentle , struct labels *head, char line[],int index);
int isCommandZero(char line[],int index);
int isCommandOne(char line[],int index);
int isCommandTwo(char line[],int index);
int LegalOpForIsCommandOne(char line[],int index,int isprn);
int isTwoS(char line[],int index);
int LegalOpForIsCommandTwo(char line[],int index, int iscmp,int islea);
int commandOneOp(char line[],int index);
int commandTwoFirstOp(char line[],int index);
int commandTwoSecondOp(char line[],int index)
