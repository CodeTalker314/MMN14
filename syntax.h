#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "arrOfSavedWords.c"
#define MAX_LINE_LENGTH 80
#define MAX_LABEL_LENGTH 30
#define MAX 100

int LineIsEmpty(char line[]);/* Not tested */ 
int LineIsComment(char line[]);/* Not tested */
int LineIsEntry(char line[]);/* Not tested */
int LineIsExtern(char line[]);/* Not tested */
int LegalLabelForEntryAndExtern(char line[]);/* Not tested */
int LegalLabelInStart(char line[]);/* Not tested */
int isSavedWord(char line[],char newSaved[]);/* Not tested */
int isGuidance(char line[]);/* Not tested */ 
int isDataLegal(char line[]);/* Not tested */
int isStringLegal(char line[]); /* Not teasted */ 
int isStructLegal(char line[]); /* Not teasted */ 
int isCommandZero(char line[]); /* Not teasted */
int isCommandOne(char line[]); /* Not teasted */
int LegalOpForIsCommandOne(char line[],int isprn); /* Not teasted */
int isCommandTwo(char line[]); /* Not teasted */
int LegalOpForIsCommandTwo(char line[],int iscmp,int islea); /* Not teasted */
int isOneR(char line[]); /* Not teasted */
int isOneS(char line[]); /* Not teasted */
int isTwoS(char line[]); /* Not teasted */
