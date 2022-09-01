#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "arrOfSavedWords.h"
#define MAX_LINE_LENGTH 80
#define MAX_LABEL_LENGTH 30

int LineIsEmpty(char line[])
{
   int index;
   for(index = 0 ,index <= MAX_LINE_LENGTH ,index++)
   {
      if((isspace(line[index])) && (line[index] != '\n'))
      {
         continue;
      }
      if(line[index] == '/n')
      {
         return 1;
      }
      return 0; /* no need to cheack for EOF because fgets stops at EOF */   
   }
}/* end of LineIsEmpty */ 


int LineIsComment(char line[])
{
   if(line == ';')/* first char is ';' */ 
   {
      return 1;
   }
   return 0;
}/* end of LineIsComment */ 


int LineIsEntry(char line[])
{
   int index;
   index = 0;
   while((isspace(line[index])) && (line[index] != '\n')) /* skipping spaces and reaching a char */
   {   
      index++;
   }  
   if (!strcmp(line[index], ".entry"))
   {	
      return 1;
   }
   return 0; /* here the line dosnt start with ".entry" */  
}/* end of LineIsEntry */


int LineIsExtern(char line[])
{
   int index;
   index = 0;
   while( (isspace(line[index])) && (line[index] != '\n')) /* skipping spaces and reaching a char */
   {
      index++;
   }
   if (!strcmp(line[index], ".extern"))
   {
      return 1;
   }
   return 0; /* here the line dosnt start with ".extern" */  
}/* end of LineIsExtern */


int LegalLabelForEntryAndExtern(char line[])
{
   int index;
   index = 0; 
   int CharCounts;
   CharCounts = 0;
   while( (isspace(line[index])) && (line[index] != '\n') /* skipping spaces and reaching a char */
   {
      index++;
   }
   while( (('a' <= line[index]) && (line[index] <= 'z')) || (('A' <= line[index]) && (line[index] <= 'Z')) && (('0' <= line[index]) && (line[index] <= '9')) )
   {
      index++;
      CharCounts++;
   }
   while( (isspace(line[index])) && (line[index] != '\n')) /* skipping spaces and reaching a char */
   { 	
      index++;
   }
   if(line[index] != '\n')
   { 	
      return 2;/* incorrect character in label name */
   } 
   if(CharCounts > MAX_LABEL_LENGTH)
   {
      return 3;/* error label length */
   }
   return 1;/* Legal Label */
}/* end of LegalLabelForEntryAndExtern */

         
int LegalLabelInStart(char line[])
{
   int CharCounts;
   CharCounts = 0;
   while( (isspace(line[index])) && (line[index] != '\n') /* skipping spaces and reaching a char */
   {
      index++;
   }
   while( (('a' <= line[index]) && (line[index] <= 'z')) || (('A' <= line[index]) && (line[index] <= 'Z')) && (('0' <= line[index]) && (line[index] <= '9')) )
   {
      index++;
      CharCounts++;
   }/* here we reach the end of the label name (if its there) */   
   if(CharCounts > MAX_LABEL_LENGTH)
   {
      return 2;/* error label length */
   }
   if(line[index] != ':')
   {
      return 0;/* not a label */ 
   }
   return 1;/* Legal Label */
}/* end of LegalLabelInStart */

         
int isSavedWord(char line[],char newSaved[])
{
   int index;
   int indexNew;
   index = 0;
   indexNew =0;
   while(saved[index] != NULL)
   {
      if(!strcmp(line,saved[index]))
      {
         return 2;
      }
   }
   while(newSaved[indexNew] != NULL)
   {
      if(!strcmp(line,newSaved[indexNew]))
      {
         return 1;
      }
      while(newSaved[indexNew] != NULL)
      {
         indexNew++;
      }
      indexNew++;
   }
   return 0;
}/* end of isSavedWord */
         
int isGuidance(char line[])
{
   if(!strcmp(line,".data"))
   {
      return 1;
   }
   if(!strcmp(line,".string"))
   {
      return 2;
   }
   if(!strcmp(line,".struct"))
   {
      return 3;
   }
   return 0;
}/* end of isGuidance */  
 
         
int isDataLegal(char line[])
{
   int index;
   index = 0;
   while( (isspace(line[index])) && (line[index] != '\n') /* skipping spaces and reaching a char */
   {
      index++;
   }/* here we encounter the first char after the .data */ 
   while(line[index] != '\n')
   {
      while( (isspace(line[index])) && (line[index] != '\n') /* skipping spaces and reaching a char */
      {
         index++;
      }/* here we encounter the first char after the last number ',' */ 
      if(line[index] == ',')
      {
         return 3;
      }      
      if( (line[index] == '-') || (line[index] == '+') || ( (line[index] >= '0') && (ine[index] <= '9') ) )
      {
         index++;
      }/* here the first char is legal */
      while((line[index] >= '0') && (ine[index] <= '9'))
      {
         index++;
      }/* here we arrive at the first char after the number */
      while( (isspace(line[index])) && (line[index] != '\n') /* skipping spaces and reaching a char */
      {
         index++;
      }/* here we encounter the first char after the last number and white space */ 
      if(line[index] == ',')
      {
         index++;
         continue;
      }
      if(line[index] == '\n')
      {
         return 1;
      }
      return 2;
   }         
}/* end of isDataLegal */

            
int isStringLegal(char line[]) 
{
   int closed;
   int index;
   closed = 1;
   index = 0;
   while( (isspace(line[index])) && (line[index] != '\n') /* skipping spaces and reaching a char */
   {
      index++;
   }/* here we encounter the first char after the .string */
   if(line[index] != '"')
   {
      return 2;
   }
   index++; /* here we arrive at the first char in the string */ 
   while(line[index] != NULL)
   {
      if(closed % 2 == 0)
      {
         while(isspace(line[index]))  
         {
            index++;
         }
         if(line[index] != NULL)
         {
            closed++;
         }
      if(line[index] == '"')
      {
         closed++;
      }
      index++;
   }
   if(closed % 2 == 1)
   {
      return 3;
   }
   return 1;
}/* end of isStringLegal */

         
int isStructLegal(char line[])
{
   int index;
   index = 0;
   if(line[index]) == ',')
   {
      return 2;   
   }
   while(line[index] != ',')
   {   
      if( (line[index] == '-') || (line[index] == '+') || ( (line[index] >= '0') && (ine[index] <= '9') ) )
      {
         index++;
      }/* here the first char is legal */
      while( (line[index] >= '0') && (ine[index] <= '9') )/* skiping legal chars */
      {
         index++;
      }/* here we at ilagel input for number */
      while( (isspace(line[index])) && (line[index] != '\n') /* skipping spaces and reaching a char */
      {
         index++;
      }  
      if(line[index] != ',')
      {
         return 3;
      }
   }/* if we end up here the number is legal */
   index++; /* skip the ',' */
   while( (isspace(line[index])) && (line[index] != '\n') /* skipping spaces and reaching a char */
   {
      index++;
   }  
   if(line[index] != '"')
   {
      return 4;
   }
   index++;/* skip the '"' */ 
   int closed;
   closed = 1;
   while(line[index] != NULL)
   {
      if(closed % 2 == 0)
      {
         while(isspace(line[index]))  
         {
            index++;
         }
         if(line[index] != NULL)
         {
            closed++;
         }
      if(line[index] == '"')
      {
         closed++;
      }
      index++;
   }
   if(closed % 2 == 1)
   {
      return 5;
   }
   return 1;
}/* end of isStructLegal */


int isCommandZero(char line[])
{
   if(!strcmp(line,"rts"))
   {
      return 1;
   }
   if(!strcmp(line,"hlt"))
   {
      return 2;
   }
   return 0;
}/* end of isCommandZero */ 


int isCommandOne(char line[])
{
   if(!strcmp(line,"not"))
   {
      return 1;
   }
   if(!strcmp(line,"clr"))
   {
      return 2;
   }
   if(!strcmp(line,"inc"))
   {
      return 3;
   }
   if(!strcmp(line,"dec"))
   {
      return 4;
   }
   if(!strcmp(line,"jmp"))
   {
      return 5;
   }
   if(!strcmp(line,"bne"))
   {
      return 6;
   }
   if(!strcmp(line,"get"))
   {
      return 7;
   }
   if(!strcmp(line,"prn"))
   {
      return 8;
   }
   if(!strcmp(line,"jsr"))
   {
      return 9;
   }
   return 0;
}/* end of isCommandOne  */      
   

int LegaOpForIsCommandOne(char line[],bollean isprn)
{
   int index;
   index = 0; 
   int CharCounts;
   CharCounts = 0;
   while( (isspace(line[index])) && (line[index] != '\n') /* skipping spaces and reaching a char */
   {
      index++;
   }
   if( (isprn) && (line[index] == '#') )
   {
      index++;
      if( (line[index] == '-') || (line[index] == '+')
      {
         index++;
      }
      while(('0' <= line[index]) && (line[index] <= '9'))
      {   
         index++;
         CharCounts++;
      }
   }else{
            while( (('a' <= line[index]) && (line[index] <= 'z')) || (('A' <= line[index]) && (line[index] <= 'Z')) && (('0' <= line[index]) && (line[index] <= '9')) )
            {
               index++;
               CharCounts++;
            }
            if(line[index] == '.') 
            {
               index++;
               CharCounts++;
            }
            while( (('a' <= line[index]) && (line[index] <= 'z')) || (('A' <= line[index]) && (line[index] <= 'Z')) && (('0' <= line[index]) && (line[index] <= '9')) )
            {
               index++;
               CharCounts++;
            }
        }
   while( (isspace(line[index])) && (line[index] != '\n')) /* skipping spaces and reaching a char */
   { 	
      index++;
   }
   if(line[index] != '\n')
   { 	
      return 2;/* incorrect character in op / label name */
   } 
   if(CharCounts > MAX_LABEL_LENGTH)
   {
      return 3;/* error label / op length */
   }
   return 1;/* Legal op */
}/* end of LegalOpForIsCommandOne */       
 
         
int isCommandTwo(char line[])
{
   if(!strcmp(line,"mov"))
   {
      return 1;
   }
   if(!strcmp(line,"cmp"))
   {
      return 2;
   }
   if(!strcmp(line,"add"))
   {
      return 3;
   }
   if(!strcmp(line,"sub"))
   {
      return 4;
   }
   if(!strcmp(line,"lea"))
   {
      return 5;
   }
   return 0;
}/* end of isCommandTwo  */ 
 

int LegalOpForIsCommandTwo(char line[],int iscmp,int islea)
{
   int index;
   index = 0; 
   int CharCounts;
   CharCounts = 0;
   int CharCountsTwo;
   CharCountsTwo = 0;
   while( (isspace(line[index])) && (line[index] != '\n') /* skipping spaces and reaching a char */
   {
      index++;
   }
   if(islea)
   {  
      if(line[index] == '#')
      {
         return 4;
      }
      if(isOneR(line[index]))
      {
         return 5;
      }
   }
   if(line[index] == '#')
   {
      index++;
      if( (line[index] == '-') || (line[index] == '+')
      {
         index++;
      }
      while(('0' <= line[index]) && (line[index] <= '9'))
      {   
         index++;
         CharCounts++;
      }
   }else{
           while( (('a' <= line[index]) && (line[index] <= 'z')) || (('A' <= line[index]) && (line[index] <= 'Z')) && (('0' <= line[index]) && (line[index] <= '9')) )
           {
              index++;
              CharCounts++;
           }
           if(line[index] != '.')
           { 	
              index++;
           } 
           while( (('a' <= line[index]) && (line[index] <= 'z')) || (('A' <= line[index]) && (line[index] <= 'Z')) && (('0' <= line[index]) && (line[index] <= '9')) )
           {
              index++;
              CharCounts++;
           } 
        }
   while( (isspace(line[index])) && (line[index] != '\n')) /* skipping spaces and reaching a char */
   { 	
      index++;
   }
   if(line[index] != ',')
   { 	
      return 2;/* incorrect character in op / label name */
   }
   CharCounts = 0; /* reset */    
   index++;/* skip the ',' */      
   while( (isspace(line[index])) && (line[index] != '\n')) /* skipping spaces and reaching a char */
   { 	
      index++;
   }     
   if( (!(iscmp)) && (line[index] == '#') )
   {
         return 6;
   }
   if( (iscmp) && line[index] == '#' )
   {
      index++;
      if( (line[index] == '-') || (line[index] == '+')
      {
         index++;
      }
      while(('0' <= line[index]) && (line[index] <= '9'))
      {   
         index++;
         CharCounts++;
      }
   }else{
           while( (('a' <= line[index]) && (line[index] <= 'z')) || (('A' <= line[index]) && (line[index] <= 'Z')) && (('0' <= line[index]) && (line[index] <= '9')) )
           {
              index++;
              CharCountsTwo++;
           }
           if(line[index] != '.')
           { 	
              index++;
           } 
           while( (('a' <= line[index]) && (line[index] <= 'z')) || (('A' <= line[index]) && (line[index] <= 'Z')) && (('0' <= line[index]) && (line[index] <= '9')) )
           {
              index++;
              CharCounts++;
           }
        }
   while( (isspace(line[index])) && (line[index] != '\n')) /* skipping spaces and reaching a char */
   { 	
      index++;
   }      
   if(line[index] != '\n')
   { 	
      return 2;/* incorrect character in op / label name */
   } 
   if( (CharCounts > MAX_LABEL_LENGTH) || (CharCountsTwo > MAX_LABEL_LENGTH) )
   {
      return 3;/* error label / op length */
   }
   return 1;/* Legal Label / op */
}/* end of LegalOpForIsCommandTwo */  
         
int isOneR(char line[])
{
   int index;
   index = 0;
   if(line[index] == 'r')
   {
      index++;
      if(('0' <= line[index]) && (line[index] <= '7'))
      {
         index++;
         while( (isspace(line[index])) && (line[index] != '\n')) /* skipping spaces and reaching a char */
         { 	
            index++;
         }
         if(line[index] == ',')
         {
            return 1;
         }
      }
   }
   return 0;
}/* end of isOneR */ 

         
int isOneS(char line[])
{
   int index;
   index = 0;
   while(line[index] != ',')
   {
      if(line[index] == '.')
      {
         return 1;
      }
   }
   return 0;
}/* end of isOneS */ 
 
         
int isTwoS(char line[])
{
   int index;
   index = 0;
   while(line[index] != '\n')
   {
      if(line[index] == '.')
      {
         return 1;
      }
   }
   return 0;
}/* end of isTwoS */ 
