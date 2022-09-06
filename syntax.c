#include "syntax.h"

char *savedWords[24];

int LineIsEmpty(char line[],int index)
{
   int indexFor;
   for(indexFor = index; indexFor <= MAX_LINE_LENGTH ; index++)
   {
      if((isspace(line[indexFor])) && (line[indexFor] != '\n'))
      {
         continue;
      }
      if(line[indexFor] == '\n')
      {
         return 1;
      }
      return 0; /* no need to cheack for EOF because fgets stops at EOF */   
   }
   return 0;
}/* end of LineIsEmpty */ 



int LineIsComment(char line[])
{
   if(line[0] == ';')/* first char is ';' */ 
   {
      return 1;
   }
   return 0;
}/* end of LineIsComment */



int firstFieldIsLabel(char line[])
{
	int eror;
   int index;
   int CharCounts;
   CharCounts = 0;
   index = 0;
   while( (isspace(line[index])) && (line[index] != '\n') )/* skipping spaces and reaching a char */
   {
      index++;
   }
   if ( !((('a' <= line[index])&&(line[index] <= 'z')) || (('A' <= line[index])&&(line[index] <= 'Z'))) )
	{
	 	eror = 1;	
	} 
   while(((('a' <= line[index])&&(line[index] <= 'z'))||(('A' <= line[index])&&(line[index] <= 'Z'))) || (('0' <= line[index])&&(line[index] <= '9')))
   {
      index++;
      CharCounts++;
   }/* here we reach the end of the label name (if its there) */   
   
   if(line[index] == ':')
   {
   	if(CharCounts > MAX_LABEL_LENGTH)
   	{        
      	return 2;/* error label length */
   	}
		if(eror == 1)
   	{        
      	return 3;
   	}
      return 1;/* Legal Label */ 
   }
   return 0;/* not a label */
}/* end of firstFieldIsLabel */



int isGuidance(char line[],int index)
{
   if(!strcmp(line[index],".data"))
   {
      return 1;
   }
   if(!strcmp(line[index],".string"))
   {
      return 2;
   }
   if(!strcmp(line[index],".struct"))
   {
      return 3;
   }
   return 0;
}/* end of isGuidance */  


int setLabelName( struct labels *currentle , struct labels *head, char line[])
{
	int index;
	int nindex;	
	int savedIndex;
	char name [MAX];
	struct labels *nowCheck;
	nowCheck = head;
	index = 0;
	nindex = 0;
	savedIndex = 0;
	memset(name , '\0' , MAX);
	while(isspace(line[index])) /* skipping spaces */
	{
   	index ++;
	} 
   while (line[index] != ':') /* reaching a char which is the name */
   {
   	name[nindex] = line[index]; /* put the value in name array */
      nindex++;
      index++;
   }
   while(!strcmp(nowCheck->labelName,name))
   {
   	nowCheck = nowCheck->next;
	}
	if(strcmp(nowCheck->labelName,name))	
   {
		return 2;/* label name is taken */
   }
	nowCheck = head;/* reset node list pointer */
	while(!strcmp(name,savedWords[savedIndex]))	
   {
		savedIndex++;
	}
	if(strcmp(name,savedWords[savedIndex])
	{
		return 1;/* label name is a system saved word */
   }
	strcpy(currentle->labelName,name); /* adding the name to its place in the node */
	return 0;
}/* end of setLabelName */


int isDataLegal(char line[] , int index)
{
	while( (isspace(line[index])) && (line[index] != '\n') ) /* skipping spaces and reaching a char */
   {
      index++;
   }/* here we encounter the first char after the .data */ 
   while(line[index] != '\n')
   {
      while( (isspace(line[index])) && (line[index] != '\n') ) /* skipping spaces and reaching a char */
      {
         index++;
      }/* here we encounter the first char after the last number ','  or for the first time */ 
      if(line[index] == ',')
      {
         return 3;
      }      
      if( (line[index] == '-') || (line[index] == '+') || ( (line[index] >= '0') && (line[index] <= '9') ) )
      {
         index++;
      }/* here the first char is legal */
      while((line[index] >= '0') && (line[index] <= '9'))
      {
         index++;
      }/* here we arrive at the first char after the number */
      while( (isspace(line[index])) && (line[index] != '\n') )/* skipping spaces and reaching a char */
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
   return 0;         
}/* end of isDataLegal */	


int isStringLegal(char line[],int index)
{
	int closed;
   closed = 1;
   while( (isspace(line[index])) && (line[index] != '\n') ) /* skipping spaces and reaching a char */
   {
      index++;
   }/* here we encounter the first char after the .string */
   if(line[index] != '"')
   {
      return 2;
   }
   index++; /* here we arrive at the first char in the string */ 
   while(line[index] != '\n')
   {
      if(closed % 2 == 0)
      {
         while(isspace(line[index]))  
         {
            index++;
         }
         if(line[index] != '\n')
         {
            closed++;
         }
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


int isStructLegal(char line[],int index)
{
   int closed;
   if(line[index] == ',')
   {
      return 2;   
   }
   while(line[index] != ',')
   {   
      if( (line[index] == '-') || (line[index] == '+') || ( (line[index] >= '0') && (line[index] <= '9') ) )
      {
         index++;
      }/* here the first char is legal */
      while( (line[index] >= '0') && (line[index] <= '9') )/* skiping legal chars */
      {
         index++;
      }/* here we at ilagel input for number */
      while( (isspace(line[index])) && (line[index] != '\n') ) /* skipping spaces and reaching a char */
      {
         index++;
      }  
      if(line[index] != ',')
      {
         return 3;
      }
   }/* if we end up here the number is legal */
   index++; /* skip the ',' */
   while( (isspace(line[index])) && (line[index] != '\n')) /* skipping spaces and reaching a char */
   {
      index++;
   }  
   if(line[index] != '"')
   {
      return 4;
   }
   index++;/* skip the '"' */ 

   closed = 1;
   while(line[index] != '\n')
   {
      if(closed % 2 == 0)
      {
         while(isspace(line[index]))  
         {
            index++;
         }
         if(line[index] != '\n')
         {
            closed++;
         }
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


int LineIsEntry(char line[], int index)
{
    
   int index;
   index = 0;
   while((isspace(line[index])) && (line[index] != '\n')) /* skipping spaces and reaching a char */
   {   
      index++;
   }  
   if (!(strcmp(&line[index], ".entry")))
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
   if (!strcmp(&line[index], ".extern"))
   {
      return 1;
   }
   return 0; /* here the line dosnt start with ".extern" */  
}/* end of LineIsExtern */


int LegalLabelForEntryAndExtern(char line[])
{
	int index;
   int eror;
   int CharCounts;
   index = 0;
   CharCounts = 0;
   while( (isspace(line[index])) && (line[index] != '\n')) /* skipping spaces and reaching a char */
   {
      index++;
   }
	if ( !((('a' <= line[index])&&(line[index] <= 'z')) || (('A' <= line[index])&&(line[index] <= 'Z'))) )
	{
	 	eror = 1;	
	}
   while(((('a' <= line[index])&&(line[index] <= 'z'))||(('A' <= line[index])&&(line[index] <= 'Z'))) || (('0' <= line[index])&&(line[index] <= '9')))
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
	if(eror == 3)
   {
      return 4;/* error label length */
   }
   return 1;/* Legal Label */
}/* end of LegalLabelForEntryAndExtern */


int setNameExternAndEntry( struct labels *currentle , struct labels *head, char line[], int index)
{
	int nindex;	
	int savedIndex;
	char name [MAX];
	struct labels *nowCheck;
	nowCheck = head;
	nindex = 0;
	savedIndex = 0;
	memset(name , '\0' , MAX);
	while(isspace(line[index])) /* skipping spaces */
	{
   	index ++;
	}  
	while(((('a' <= line[index])&&(line[index] <= 'z'))||(('A' <= line[index])&&(line[index] <= 'Z'))) || (('0' <= line[index])&&(line[index] <= '9')))
   {
   	name[nindex] = line[index]; /* put the value in name array */
      nindex++;
      index++;
   }
   while(!strcmp(nowCheck->labelName,name))
   {
   	nowCheck = nowCheck->next;
	}
	if(strcmp(nowCheck->labelName,name))	
   {
		return 2;/* label name is taken */
   }
	nowCheck = head;/* reset node list pointer */
	while(!strcmp(name,savedWords[savedIndex]))	
   {
		savedIndex++;
	}
	if(strcmp(name,savedWords[savedIndex])
	{
		return 1;/* label name is a system saved word */
   }
	strcpy(currentle->labelName,name); /* adding the name to its place in the node */
	return 0;
}/* end of setLabelName */


int isCommandZero(char line[],int index)
{
	char *linee = line[index];
   if(!strcmp(linee,"rts"))
   {
      return 1;
   }
   if(!strcmp(linee,"hlt"))
   {
      return 2;
   }
   return 0;
}/* end of isCommandZero */ 


int isCommandOne(char line[], int index)
{
	char *linee = line[index];
   if(!strcmp(linee,"not"))
   {
      return 1;
   }
   if(!strcmp(linee,"clr"))
   {
      return 2;
   }
   if(!strcmp(linee,"inc"))
   {
      return 3;
   }
   if(!strcmp(linee,"dec"))
   {
      return 4;
   }
   if(!strcmp(linee,"jmp"))
   {
      return 5;
   }
   if(!strcmp(linee,"bne"))
   {
      return 6;
   }
   if(!strcmp(linee,"get"))
   {
      return 7;
   }
   if(!strcmp(linee,"prn"))
   {
      return 8;
   }
   if(!strcmp(linee,"jsr"))
   {
      return 9;
   }
   return 0;
}/* end of isCommandOne  */ 


int isCommandTwo(char line[], int index)
{
	char *linee = line[index];
   if(!strcmp(linee,"mov"))
   {
      return 1;
   }
   if(!strcmp(linee,"cmp"))
   {
      return 2;
   }
   if(!strcmp(linee,"add"))
   {
      return 3;
   }
   if(!strcmp(linee,"sub"))
   {
      return 4;
   }
   if(!strcmp(linee,"lea"))
   {
      return 5;
   }
   return 0;
}/* end of isCommandTwo  */ 


int LegalOpForIsCommandOne(char line[],int index,int isprn)
{
   int CharCounts;
   CharCounts = 0;
   while( (isspace(line[index])) && (line[index] != '\n') )/* skipping spaces and reaching a char */
   {
      index++;
   }
   if( (isprn) && (line[index] == '#') )
   {
      index++;
      if( (line[index] == '-') || (line[index] == '+') )
      {
         index++;
      }
      while(('0' <= line[index]) && (line[index] <= '9'))
      {   
         index++;
         CharCounts++;
      }
   }else{
            while(((('a' <= line[index])&&(line[index] <= 'z'))||(('A' <= line[index])&&(line[index] <= 'Z'))) || (('0' <= line[index])&&(line[index] <= '9')))
            {
               index++;
               CharCounts++;
            }
            if(line[index] == '.') 
            {
               index++;
               CharCounts++;
            }
            while(((('a' <= line[index])&&(line[index] <= 'z'))||(('A' <= line[index])&&(line[index] <= 'Z'))) || (('0' <= line[index])&&(line[index] <= '9')))
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


int isTwoS(char line[],int index)
{
   while(line[index] != '\n')
   {
      if(line[index] == '.')
      {
         return 1;
      }
   }
   return 0;
}/* end of isTwoS */   


int commandOneOp(char line[],int index)
{
   while(line[index] != '\n')
   {
      if(line[index] == '#')
      {
         return 0;
      }
		if(line[index] == '.')
      {
         return 2;
      }
		if( (line[index] == 'r') && ((line[index+1] >= '0') && (line[index+1] <= '9')) && (isspace(line[index+2])) )
		{
			return 3;
		}
   }
   return 1;
}/* end of isTwoS */


int LegalOpForIsCommandTwo(char line[],int index, int iscmp,int islea)
{
   int CharCounts;
   int CharCountsTwo;
   CharCountsTwo = 0;
   CharCounts = 0;
   while( (isspace(line[index])) && (line[index] != '\n') )/* skipping spaces and reaching a char */
   {
      index++;
   }
   if(islea)
   {  
      if(line[index] == '#')
      {
         return 4;
      }
      if(isOneR(line,index))
      {
         return 5;
      }
   }
   if(line[index] == '#')
   {
      index++;
      if( (line[index] == '-') || (line[index] == '+') )
      {
         index++;
      }
      while(('0' <= line[index]) && (line[index] <= '9'))
      {   
         index++;
         CharCounts++;
      }
   }else{
           while(((('a' <= line[index])&&(line[index] <= 'z'))||(('A' <= line[index])&&(line[index] <= 'Z'))) || (('0' <= line[index])&&(line[index] <= '9')))
           {
              index++;
              CharCounts++;
           }
           if(line[index] != '.')
           { 	
              index++;
           } 
           while(((('a' <= line[index])&&(line[index] <= 'z'))||(('A' <= line[index])&&(line[index] <= 'Z'))) || (('0' <= line[index])&&(line[index] <= '9')))
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
      if( (line[index] == '-') || (line[index] == '+') )
      {
         index++;
      }
      while(('0' <= line[index]) && (line[index] <= '9'))
      {   
         index++;
         CharCounts++;
      }
   }else{
           while(((('a' <= line[index])&&(line[index] <= 'z'))||(('A' <= line[index])&&(line[index] <= 'Z'))) || (('0' <= line[index])&&(line[index] <= '9')))
           {
              index++;
              CharCountsTwo++;
           }
           if(line[index] != '.')
           { 	
              index++;
           } 
           while(((('a' <= line[index])&&(line[index] <= 'z'))||(('A' <= line[index])&&(line[index] <= 'Z'))) || (('0' <= line[index])&&(line[index] <= '9')))
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


int commandTwoFirstOp(char line[],int index)
{
   while(line[index] != ',')
   {
      if(line[index] == '#')
      {
         return 0;
      }
		if(line[index] == '.')
      {
         return 2;
      }
		if( (line[index] == 'r') && ((line[index+1] >= '0') && (line[index+1] <= '9')) && (isspace(line[index+2])) )
		{
			return 3;
		}
   }
   return 1;
}/* end of isTwoS */


int commandTwoSecondOp(char line[],int index)
{
   while(line[index] != '\n')
   {
      if(line[index] == '#')
      {
         return 0;
      }
		if(line[index] == '.')
      {
         return 2;
      }
		if( (line[index] == 'r') && ((line[index+1] >= '0') && (line[index+1] <= '9')) && (isspace(line[index+2])) )
		{
			return 3;
		}
   }
   return 1;
}/* end of isTwoS */  














