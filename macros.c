#include <stdio.h>
#include <stdlib.h>
#include "macros.h"

typedef struct datatype Macros;
struct datatype {
    char macroname;
    char macrodata;
    Macros *next;
};

int StartEndMacro(char line[]) /* Checks whether it is the beginning or the end of a macro definition */
{
  int index = 0 , mindex = 0 ;
  char macro [MAX];
  memset(macro , '\0' , MAX);
  while(isspace(line[index])) /* skipping spaces */
       index ++;
  while (!isspace(line[index]) && line[index] != '\n') /* reaching a char */
  {
    macro[mindex] = line[index]; /* put the value in macro array */
    mindex++;
    index++;
  }
  if (!strcmp(macro, "macro"))
    return 1; /* the beginning of a macro */
  else if (!strcmp(macro, "endmacro"))
    return 2; /* the end of a macro */
  return 0; /* not beginning and not end of a macro = regular word */
}

void InsertName(struct Macros *temp, char line[]) /* Insert the macro name to the macros table */
{
  int index = 0 , nindex = 0 ;
  char name [MAX];
  memset(name , '\0' , MAX);
  while(isspace(line[index])) /* skipping spaces */
    index ++; 
  while (!isspace(line[index]) && line[index] != '\n') /* skipping the chars of the first word (macro) */
    index++; 
  while(isspace(line[index])) /* skipping spaces */
    index ++; 
  while (!isspace(line[index]) && line[index] != '\n') /* reaching a char which is the name */
  {
    name[nindex] = line[index]; /* put the value in name array */
    nindex++;
    index++;
  }
  strcpy(temp->macroname,name); /* adding the name to its place in the node */
}

void InsertContent(struct Macros *temp, FILE *fp) /* Insert the content of macro to the macros table */
{
  char line [MAX];
  char content [MAX];
  memset(line , '\0' , MAX);
  memset(content , '\0' , MAX);
  fgets(line, MAX, fp);
  while(StartEndMacro(line) != 2) /* while not reaching the end of a macro definition */
  {
    strncat(content, line , MAX); /* concat the content of the given line */
    fgets(line, MAX, fp); /* get next line from file */
  }
  strcpy(temp->macrodata,content); /* adding the content to its place in the node */
}

int IsMacroCall(char line[], FILE *fpw, struct Macros *tail) /* If macro call, replace line with macro content */
{
  int index = 0, mindex = 0;
  char Mname [MAX];
  struct  Macros *temp = tail;
  memset(Mname, '\0' , MAX);
  while(isspace(line[index])) /* skipping spaces */
    index ++;
  while (!isspace(line[index]) && line[index] != '\n') /* reaching a char */
  {
    Mname[mindex] = line[index]; /* put the value in name array */
    mindex++;
    index++;
  }
  while (temp != NULL) /* while we have macros */
  {
    if (!strcmp(temp->macroname , Mname)) /* if the macro name matches to the call in the file */
    {
    fprintf(fpw, "%s", temp->macrodata); /* sending the content to its place in the file */
    return 1; /* it is a macro call */
    }
    temp = temp ->next; /* go over to the next macro */
  }
  return 0; /* that's not a macro call */
}

/*Performing the first pass on the file (inserting the macros into the macro table, */
/*  copying the corresponding rows from the table to the file, etc.) */

FILE* PreReadFile(char *file, struct Macros *head)      
{
  char line [MAX];
  FILE *fp;
  memset(line , '\0' , MAX);
  fp = fopen(file,"r");
  if(fp == NULL) /* failed to open the file */
  {
     printf("error: cant open the file: %s \n \n" , file);
     return NULL;
  }
  while(fgets(line, MAX, fp)) /* getting lines */
  {

      struct  Macros* temp = NULL;
   temp = (struct Macros*)malloc(sizeof(struct Macros)); 
   if(StartEndMacro(line) == 1) /* the beginning of the definition of a macro */
      {
       printf("%s\n", line);
        InsertName(temp , line); /* add the name to temp in macro list */
        InsertContent(temp , fp); /* add the content to temp in macro list */
        temp -> next = head ; /* add another node to macro list */
        head = temp;
      }
  }
  return fp; /* succeeded to process the file */
}


FILE* WritePreFile(char *asfile, char *amfile, struct Macros *tail)
{
  int macroflag;
  FILE *fpw; /* file we write to */
  FILE *fpr; /* file we read from */
  char line[MAX];
  memset(line , '\0' , MAX);
  fpr = fopen(asfile,"r");
  fpw = fopen(amfile,  "w");
  if(fpr == NULL) /* failed to open the file */
     printf("error: cant open the file: %s \n \n" , asfile);
  while(fgets(line, MAX, fpr)) /* getting lines */
  {
    if(!macroflag)
    {
        if(!IsMacroCall(line, fpw, tail))
       {
         if(StartEndMacro(line) == 0) /* no macro = regular code word */
         {
            fprintf(fpw,"%s",line); /* add word to file */
            printf("%s\n", line);
         }
         else 
         {
            macroflag = 1;
         } 
       }
    }
    else
    {
      if(StartEndMacro(line) == 2) /* end of macro */
        macroflag = 0;
    }        
  } 
  fclose(fpr);
  return fpw;
}
