#include "main.h"

struct labels
{
	char labelName[MAX_LINE_LENGTH];
	char icAddress[MAX_LINE_LENGTH];
	char dcAddress[MAX_LINE_LENGTH];
	int external;
	int entry;
	int code;
	struct labels* next;
};

int main(int argc, const char *argv[])
{
   /* variable declaration */
	FILE *file;/* for pointing the file we working with */
   char *fullFileName;/* for pointing the input file (.as) */
   char *macroName;/* for pointing the file after the macros opening (.am) */ 
	char *obF;
	char *extF;
	char *entF;
   char getLine[MAX_LINE_LENGTH];/* for saving current line */
   char *line;/* for pointing line start */
   char data[MAX][10];
   char command[MAX][10];
   char numArr[MAX];
   int num;
   int i;/* for index inside *argv[] */
	int index;/* for counting chars in line */
   int IC;/* for counting instruction words */
   int DC;/* for counting data words */
	int labelFlag;/* flag for label in line start */
	int errorNum;/* for counting errors found */
	int lineNum;/*for counting lines in the after macro file */
	int numIndex;/*for counting numArr */ 
	int charString;
	int iscmp;
   int islea;
	int isprn;
	int L;
	int firstIsR;
	char name[MAX];
   int nindex;
	

	/* for labels node list */ 
   struct labels* head;
   struct labels* currentle;
   struct labels* nextNode;
  

 	memset(name , '\0' ,MAX);/* puting null in arr */
	memset(getLine , '\0' ,MAX_LINE_LENGTH);/* puting null in line arr */
	line = getline[0];/* pointing the line start */

   /*no file input case*/
   if (argc <= 1) 
   {
  		printf("EROR: No File input detected.\n");
      exit(0);
   }

   /* here we have input files so we start the assembler process */
   for (i = 1; i < argc; i++)
   { 
      /* resets pointers */ 
      fullFileName = NULL;
      macroName = NULL;
      linenum = 0;
		firstIsR = NULL;
      
      /*  initialization of label node list */     
      head = (struct labels*)malloc(sizeof(struct labels));
		if(head)
		{              	
			printf("error: malloc failed. \n");
		}
      currentle = head;

      fullFileName = asFile(argv[i]); /* pointing .as file */
      macroName = amFile(argv[i]); /* pointing .am file */
      syntaxFile = textFile(argv[i]);/* ointing .text file */
      printf("Started %s assembler process\n\n", fullFileName);/* Message to user */ 
      file = fopen(fullFileName, "ab+"); /* create .as file */
     
      /* create .am file */  
      amRun( fullFileName , macroName , file );
      
      /* Messages to user */
      printf("created the %s\n",macroName);
      printf("starting first read on %s file\n",macroName);

		/* 1 */
      DC = 0;
		IC = 0;

      
      file = fopen(macroFile,"r");
		if(file == NULL) /* failed to open the file */
	   {      
	      printf("error: cant open the after macro file.\n" );
	   }

		/* 2 */
      while(fgets(getLine, MAX_LINE_LENGTH, file)) /* getting lines */
      {
			lineNum++;/*update line number */ 			
			
			/* reset flags */
			labelFlag = NULL;
			isprn = NULL;
			iscmp = NULL;
         islea = NULL;

			L = 0;
			

			/* empty line case */
			if(LineIsEmpty(line) == 1)   
	      {
   	      continue;/* empty line are ignored */
   	   }  

         /* comment line case */ 
			if(LineIsComment(line) == 1) 
      	{ 
      	   continue;/* comment line are ignored */
     		}

			if( firstFieldIsLabel(line) == 1 )
         {				
         	/* 4 */		   
            labelFlag = 1;
         }
			if( firstFieldIsLabel(line) == 2)
         {
				errorNum++;
            printf("error(file after macro,line: %d): label name exceeds the limit \n" , lineNum);
				/* 4 */		   
            labelFlag = 1;
			}
			if(firstFieldIsLabel(line) == 3 )
		   {
				errorNum++;
            printf("error(file after macro,line: %d): label name start with worng char. \n" , lineNum);
			   continue;
         }
			if(labelFlag)
			{
         	while(getLine[index] != ':' )/* skiping after label */
				{
					index++;
				}
				index++;				
			}
			while( (isspace(getLine[index])) && (getLine[index] != '\n') ) /* skipping spaces and reaching a char */
         {
         	index++;
         }
         /* 5 */
			if(isGuidance(line,index))
			{
				/* 6 */
				if(labelFlag)
				{
					if(!setLabelName( currentle ,head ,  line))
					{
						currentle->dcAddress = DC;
                  nextNode = (struct labels*)malloc(sizeof(struct labels));
						if(nextNode)
						{
                  	printf("error: malloc failed. \n" , lineNum);
						}
                  currentle -> next = nextNode;
                  currentle = nextNode;	
					}else{
						     if(setLabelName( currentle ,head ,  line) == 1 )
							  {
								 errorNum++;
                         printf("error(file after macro,line: %d): label name is a saved system word. \n" , lineNum);
							 	 continue;
                       }
							  if(setLabelName( currentle ,head ,  line) == 2 )
							  {
								 errorNum++;
                         printf("error(file after macro,line: %d): label name is taken by another label. \n" , lineNum);
							 	 continue;
                       }
                    }	
				}
				/* 7 */
				if(isGuidance(line,index)  == 1)/* data case */
				{
					index = index + 5; /* skiping the ".data" */
					if(LineIsEmpty(line,index) == 1)
               {
                  errorNum++;
                  printf("error(file after macro,line: %d): data guidance requires at least one valid number\n" , lineNum);
                  continue;      
               }	
					if(isDataLegal(line,index) == 3 )
               {
            	   errorNum++;
            	   printf("error(file after macro,line: %d): found , in worng place \n" , lineNum);
            	   continue;
               }
					if(isDataLegal(line,index) == 2 )   
               {
               	errorNum++;
               	printf("error(file after macro,line: %d): illegal number in data \n" , lineNum);
               	continue;
               }
					if(isDataLegal(line,index) == 1 )
					{
						while(line[index] != '\n')
						{
							memset(numArr , '\0' ,MAX);/* puting null in num arr */
							numIndex = 0;
							while( (isspace(line[index])) && (line[index] != '\n') ) /* skipping spaces and reaching a char */
		               {
		               	index++;
		               }/* here we encounter the first char after the last number ','  or for the first time */ 						
							if(line[index] == '+')
		               {
		           	   	index++;
		               }
		               if(line[index] == '-')
		               {
								numArr[numIndex] = '-' ;
	                  	index++;
                        numIndex++;
		               }
							while( (line[index] >= '0') && (line[index] <= '9') )
							{
						     	numArr[numIndex] = line[index];
                        numIndex++;
                        index++;
                     }
                     num = numArr;/* add num word to data */
                     num =decimalToBinary(num);
                     data[DC] = num;
							DC++;
							while( (isspace(line[index])) && (line[index] != '\n') ) /* skipping spaces and reaching a char */
		               {
		               	index++;
		               }
							if(line[index] != '\n') /*skip the , */
							{
		               	index++;
		               }		                     
						}
					continue;							
					}
				}/* end of data case */
				if(isGuidance(line,index) == 2 )/* string case */ 
			   {
					index = index + 7;/* skiping the ".string" */
               if(LineIsEmpty(line,index) == 1)
               {
               	errorNum++;
            		printf("error(file after macro,line: %d): string guidance input not found \n" , lineNum);
            		continue;      
         		}
         		if(isStringLegal(line,index) == 2 )
         		{
            		errorNum++;
            		printf("error(file after macro,line: %d): missing \" after .string \n" , lineNum);
            		continue;    
         		}
         		if(isStringLegal(line,index) == 3 )
        			{
            	   errorNum++;
            		printf("error(file after macro,line: %d): missing last \" after string input \n" , lineNum);
           		   continue;    
         		}
         		if(isStringLegal(line,index) == 1 )   
         		{/* here the string is legal */
						while( (isspace(line[index])) && (line[index] != '\n') )/* skipping spaces and reaching a char */
               	{
             			index++;
              		}/* here we encounter the first char after the .string */    
          			index++;/* skip the first " */            		
						while(line[index] != '"')
            		{
							data[DC]	 = decimalToBinary( (int)line[index] );                	
                     if( LineIsEmpty(line,index) ) 
                     {
                			data[DC] =	{0,0,0,0,0,0,0,0,0,0};
								DC++;
								continue;
							}
							index++;
							DC++;
              	   }
						continue;
				   }
				}/* end of string case */	
				if(isGuidance(line,index) == 3 )/* struct case */	     
				{
					index = index + 7;/* skiping the ".struct" */
					if(LineIsEmpty(line,index) == 1)
               {
         	      errorNum++;
         		   printf("error(file after macro,line: %d): struct guidance input not found \n" , lineNum);
           		   continue;      
         		}
               while( (isspace(line,index)) && (line[index] != '\n') ) /* skipping spaces and reaching a char */
               {
                  index++;
               }  
               if(isStructLegal(line,index) == 2 )
               {
               	errorNum++;
                  printf("error(file after macro,line: %d): found , before number input \n" , lineNum);
                  continue;    
               } 
               if(isStructLegal(line,index) == 3 )
               {
               	errorNum++;
            		printf("error(file after macro,line: %d): found worng char in number input \n" , lineNum);
            		continue;    
         		}
         		if(isStructLegal(line,index) == 4 )
       		   {
           		   errorNum++;
           		   printf("error(file after macro,line: %d): no \" found for string input \n" , lineNum);
           		   continue;    
               }
               if(isStructLegal(line,index) == 5 )
               {
               	errorNum++;
            		printf("error(file after macro,line: %d): no \" found in end of string input\n", lineNum);
                  continue;    
               }          
               if(isStructLegal(line,index) == 1 )   
               {/* here the input is legal */
						memset(numArr , '\0' ,MAX);/* puting null in num arr */
						numIndex = 0;
						while( (isspace(line[index])) && (line[index] != '\n') ) /* reaching a char */
		         	{
		 	      		index++;
               	}/* here we encounter the first char after .struct */
						if(line[index] == '+')
		        	   {
	        	   		index++;
	            	}
		         	if(line[index] == '-')
		         	{
							numArr[numIndex] = '-' ;
	              		index++;
               	   numIndex++;
		         	}
						while( (line[index] >= '0') && (line[index] <= '9') )
						{
					     	numArr[numIndex] = line[index];
                     numIndex++;
                     index++;
                  }
                  num = numArr;/* add num word to data */
                  num =decimalToBinary(num);
                  data[DC] = num;
			      	DC++;	
						while(line[index] != ',')
						{
							index++;
						}
						index++;
						while( (isspace(line[index])) && (line[index] != '\n') )/* skipping spaces and reaching a char */
               	{
             			index++;
              		}/* here we encounter the first char after the , */    
          			index++;/* skip the first " */            		
						while(line[index] != '"')
            		{
							data[DC]	 = decimalToBinary( (int)line[index] );                	
                     if( LineIsEmpty(line,index) ) 
                     {
                			data[DC] =	{0,0,0,0,0,0,0,0,0,0};
								DC++;
								continue;
							}
							index++;
							DC++;
              	   }
						continue;				
					}
				}/* end of struct case */				
			}
			/* 8 */ 
			if(  (LineIsEntry(line,index)) ||  (LineIsExtern(line,index))  )
         {
				/* 9*/ 
				if( LineIsExtern(line,index) )/* extern case */
				{
					index = index  + 7;/* taking the index after the word ".extern" */
					if(LegalLabelForEntryAndExtern(line,index) == 4) 
               {
               	errorNum++;
                  printf("error(file after macro,line: %d):  incorrect first character in label name\n" , lineNum);
                  continue;
               }
					if(LegalLabelForEntryAndExtern(line,index) == 3) 
               {
               	errorNum++;
                  printf("error(file after macro,line: %d): label name exceeds the limit \n" , lineNum);
                  continue;
               }
               if(LegalLabelForEntryAndExtern(line,index) == 2)
               {
               	errorNum++;
            		printf("error(file after macro,line: %d): incorrect character in label name \n" , lineNum);
            		continue;
         		}
         		if(LegalLabelForEntryAndExtern(line,index) == 1)
					{
						if(!setLabelName( currentle ,head ,  line))
					   {
							currentle->external = 1;
                  	nextNode = (struct labels*)malloc(sizeof(struct labels));
							if(nextNode)
							{
                  		printf("error: malloc failed. \n" , lineNum);
							}
                  	currentle -> next = nextNode;
                  	currentle = nextNode;	
						}else{
						        if(setLabelName( currentle ,head ,  line) == 1 )
							     {
								     errorNum++;
                             printf("error(file after macro,line: %d): label name is a saved system word. \n" , lineNum);
							 	     continue;
                          }
							     if(setLabelName( currentle ,head ,  line) == 2 )
							     {
								     errorNum++;
                             printf("error(file after macro,line: %d): label name is taken by another label. \n" , lineNum);
							 	     continue;
                          }
                       }	  
				     }/* end extern case */
					  /* 10 */
					  continue;
				}	
			}
			/* 11 */
			if(labelFlag == 1)
			{
				if(!setLabelName( currentle ,head ,  line))
				{
					currentle->code = 1;
					currentle->icAddress = ic;
               nextNode = (struct labels*)malloc(sizeof(struct labels));
					if(nextNode)
					{
                  printf("error: malloc failed. \n" , lineNum);
					}
               currentle -> next = nextNode;
               currentle = nextNode;	
				}else{
						  if(setLabelName( currentle ,head ,  line) == 1 )
						  {
						     errorNum++;
                       printf("error(file after macro,line: %d): label name is a saved system word. \n" , lineNum);
							  continue;
                    }
						  if(setLabelName( currentle ,head ,  line) == 2 )
						  {
						     errorNum++;
                       printf("error(file after macro,line: %d): label name is taken by another label. \n" , lineNum);
							  continue;
                    }
                 }		
			}
         /* 12 */
			if( (!isCommandZero(line,index)) && (!isCommandOne(line,index)) && (!isCommandTwo(line,index)) )
			{
            errorNum++;
            printf("error(file after macro,line: %d): Illegal command name.\n" , lineNum);
				continue;
			}
			/* 13 */ 
         if(isCommandZero(line,index))/* commands with no input */ 
			{
			 	if(isCommandZero(line,index) == 1)/* rts */
				{
					command[IC] = {1,1,1,0,0,0,0,0,0,0};
					IC++;
				}
				if(isCommandZero(line,index) == 2)/* hlt */
				{
					command[IC] = {1,1,1,1,0,0,0,0,0,0};
					IC++;
				}
			}
			if(isCommandOne(line,index))
			{
				if(isCommandOne(line,index) == 1) /* not */
				{
					command[IC][0] = 0;
					command[IC][1] = 1;
					command[IC][2] = 0;
					command[IC][3] = 0;
				}
				if(isCommandOne(line,index) == 2) /* clr */
				{
					command[IC][0] = 0;
					command[IC][1] = 1;
					command[IC][2] = 0;
					command[IC][3] = 1;
				}
				if(isCommandOne(line,index) == 3) /* inc */
				{
					command[IC][0] = 0;
					command[IC][1] = 1;
					command[IC][2] = 1;
					command[IC][3] = 1;
				}
				if(isCommandOne(line,index) == 4) /* dec */
				{
					command[IC][0] = 1;
					command[IC][1] = 0;
					command[IC][2] = 0;
					command[IC][3] = 0;
				}
				if(isCommandOne(line,index) == 5) /* jmp */
				{
					command[IC][0] = 1;
					command[IC][1] = 0;
					command[IC][2] = 0;
					command[IC][3] = 1;
				}
				if(isCommandOne(line,index) == 6) /* bne */
				{
					command[IC][0] = 1;
					command[IC][1] = 0;
					command[IC][2] = 1;
					command[IC][3] = 0;
				}
				if(isCommandOne(line,index) == 7) /* get */
				{
					command[IC][0] = 1;
					command[IC][1] = 0;
					command[IC][2] = 1;
					command[IC][3] = 1;
				}
				if(isCommandOne(line,index) == 8) /* prn */
				{
					command[IC][0] = 1;
					command[IC][1] = 1;
					command[IC][2] = 0;
					command[IC][3] = 0;
					isprn = 1;
				}
				if(isCommandOne(line,index) == 9) /* jsr */
				{
					command[IC][0] = 1;
					command[IC][1] = 1;
					command[IC][2] = 0;
					command[IC][3] = 1;
				}
				index = index + 3;
				if(LineIsEmpty(line,index))
      	   {
          	  errorNum++;
          	  printf("error(file after macro,line: %d): no input after command \n" , lineNum);
           	  continue; 
       	   }
			   if( !((isspace(line[index])) && (line[index] != '\n')) )
            {
               errorNum++;
               printf("error(file after macro,line: %d): no whitespace after command \n" , lineNum);
               continue;
            }
			   while( (isspace(line[index])) && (line[index] != '\n') )/* skipping spaces and reaching a char */
            {
               index++;
            }
			   if(LegalOpForIsCommandOne(line,index,isprn) == 2)
            {
               errorNum++;
               printf("error(file after macro,line: %d): incorect character in op / label name\n" , lineNum);
               continue;
            }
            if(LegalOpForIsCommandOne(line,index,isprn) == 3)
            {
               errorNum++;
               printf("error(file after macro,line: %d): incorrect label / op length\n" , lineNum);
               continue;
            }
            if(LegalOpForIsCommandOne(line,index,isprn) == 1)
            {/* here we know that the input is legal */
					
					command[IC][4] = 0;
					command[IC][5] = 0;
					while( (isspace(line[index])) && (line[index] != '\n') )/* reaching a char */
              	{
             		index++;
               }
					if (commandOneOp(line,index) == 0)
					{
						command[IC][6] = 0;
						command[IC][7] = 0;
						L++;
					}
					if (commandOneOp(line,index) == 1)
					{
						command[IC][6] = 0;
						command[IC][7] = 1;
						L++;
					}
					if (commandOneOp(line,index) == 2)
					{
						command[IC][6] = 1;
						command[IC][7] = 0;
						L = L + 2;
					}
					if (commandOneOp(line,index) == 3)
					{
						command[IC][6] = 1;
						command[IC][7] = 1;
						L++;
					}
					command[IC][8] = 0;
					command[IC][9] = 0;
					IC++;
	         }	   
			}/* end of command one case */
			if(isCommandTwo(line,index))
			{
				if(isCommandTwo(line,index) == 1) /* mov */
				{
					command[IC][0] = 0;
					command[IC][1] = 0;
					command[IC][2] = 0;
					command[IC][3] = 0;
				}
				if(isCommandTwo(line,index) == 2) /* cmp */
				{
					command[IC][0] = 0;
					command[IC][1] = 0;
					command[IC][2] = 0;
					command[IC][3] = 1;
					iscmp = 1;
				}
				if(isCommandTwo(line,index) == 3) /* add */
				{
					command[IC][0] = 0;
					command[IC][1] = 0;
					command[IC][2] = 1;
					command[IC][3] = 0;
				}
				if(isCommandTwo(line,index) == 4) /* sud */
				{
					command[IC][0] = 0;
					command[IC][1] = 0;
					command[IC][2] = 1;
					command[IC][3] = 1;
				}
				if(isCommandTwo(line,index) == 5) /* lea */
				{
					command[IC][0] = 0;
					command[IC][1] = 1;
					command[IC][2] = 1;
					command[IC][3] = 0;
					islea = 1;
				}	
				index = index + 3;
				if(LineIsEmpty(line,index))
            {
               errorNum++;
               printf("error(file after macro,line: %d): no input after command \n" , lineNum);
               continue; 
            }
				if( !((isspace(line[index])) && (line[index] != '\n')) )
            {
               errorNum++;
               printf("error(file after macro,line: %d): no white space after command with input\n" , lineNum);
               continue;
            }
            while( (isspace(line[index])) && (line[index] != '\n') ) /* skipping spaces and reaching a char */
            {
               index++;
            }
				if(LegalOpForIsCommandTwo(line,index,iscmp,islea) == 2)
            {
               errorNum++;
               printf("error(file after macro,line: %d): incorrect character in op / label name \n" , lineNum);
               continue;
            }
            if(LegalOpForIsCommandTwo(line,index,iscmp,islea) == 3)
		      {
		         errorNum++;
		         printf("error(file after macro,line: %d): error label / op length \n" , lineNum);
		         continue;
		      }
		      if(LegalOpForIsCommandTwo(line,index,iscmp,islea) == 4)
		      {
		         errorNum++;
		         printf("error(file after macro,line: %d): lea command cant get number as source op \n" , lineNum);
		         continue;
		      }
		      if(LegalOpForIsCommandTwo(line,index,iscmp,islea) == 5)
		      {
		         errorNum++;
		         printf("error(file after macro,line: %d): lea command cant get register source op \n" , lineNum);
		         continue;
		      }
		      if(LegalOpForIsCommandTwo(line,index,iscmp,islea) == 6)
		      {
		         errorNum++;
		         printf("error(file after macro,line: %d): command cant get number as destination op \n" , lineNum);
		         continue;
		      }
		      if(LegalOpForIsCommandTwo(line,index,iscmp,islea) == 1)
		      {
					if (commandOneFirstOp(line,index) == 0)
					{
						command[IC][4] = 0;
						command[IC][5] = 0;
						L++;
					}
					if (commandOneFirstOp(line,index) == 1)
					{
						command[IC][4] = 0;
						command[IC][5] = 1;
						L++;
					}
					if (commandOneFirstOp(line,index) == 2)
					{
						command[IC][4] = 1;
						command[IC][5] = 0;
						L = L + 2;
					}
					if (commandOneFirstOp(line,index) == 3)
					{
						command[IC][4] = 1;
						command[IC][5] = 1;
						L++;
						firstIsR = 1;
					}
					while( (isspace(line[index])) && (line[index] != '\n') )/* reaching a char */
              	{
             		index++;
               }
					index++;/* skip the , */
					if (commandTwoSecondOp(line,index) == 0)
					{
						command[IC][6] = 0;
						command[IC][7] = 0;
						L++;
					}
					if (commandTwoSecondOp(line,index) == 1)
					{
						command[IC][6] = 0;
						command[IC][7] = 1;
						L++;
					}
					if (commandTwoSecondOp(line,index) == 2)
					{
						command[IC][6] = 1;
						command[IC][7] = 0;
						L = L + 2;
					}
					if (commandTwoSecondOp(line,index) == 3)
					{
						command[IC][6] = 1;
						command[IC][7] = 1;
						if(!firstIsR)
						{
							L++;	
						}
					}
					command[IC][8] = 0;
					command[IC][9] = 0;
					IC++;
				}	
			}/* end commandTwo case */	
			/* 14 */
			IC = IC + L;
			L =0;
			/* 15 */
			continue;			
		}
		/* 16 */
      if(errorNum != 0)
		{
			printf("assembler stops due to errors founds in file aftert macro \n" , lineNum);
			continue;
		}
		/* 17 */
		currentle->labelName = NULL;
		currentle = head;
		while(currentle->labelName != NULL)
		{
			if(currentle->dcAddress != NULL)
			{
				currentle->icAddress = currentle->dcAddress + IC +99;
			}else{
					 currentle->icAddress +99; 			
				  }
			currentle = currentle->next;
		}
		/* 18 */

		/* Messages to user */
      printf("created the label list);
      printf("starting second read on %s file\n",macroName);

		/* 2-1 */
		IC = 0;

		/* 2-2 */
		/* RESETS */
		file = NULL;
		lineNum = 0;
      errorNum = 0;
		fclose(macroFile);
		file = fopen(macroFile,"r");
		if(file == NULL) /* failed to open the file */
	   {      
	      printf("error: cant open the after macro file.\n" );
	   }		
      while(fgets(getLine, MAX_LINE_LENGTH, file)) /* getting lines */
      {
			index = 0;
			labelFlag = NULL;
			lineNum++;
			/* empty line case */
			if(LineIsEmpty(line) == 1)   
	      {
   	      continue;/* empty line are ignored */
   	   }  
         /* comment line case */ 
			if(LineIsComment(line) == 1) 
      	{ 
      	   continue;/* comment line are ignored */
     		}
			/* 2-3 */
			if( firstFieldIsLabel(line) == 1 )
         {						   
            while(line[index] != ':')
				{
					index++;
				}
				index++;/* skip the : */
         }
			/* 2-4 */ 
			if(isGuidance(line,index))
			{						   
            continue;/* ignored */
         }
			if(LineIsExtern(line))
			{
				continue;/* ignored */
			}
			/* 2-5 */
			if(LineIsEntry(line))
			{
				/* 2-6 */
				while( (isspace(line[index])) && (line[index] != '\n') )/* reaching a char */
            {
               index++;
            }
				index = index  + 7;/* taking the index after the word ".entery" */
				while( (isspace(line[index])) && (line[index] != '\n') )/* reaching a char */
            {
             	index++;
            }
				if(LegalLabelForEntryAndExtern(line,index) == 4) 
            {
               errorNum++;
               printf("error(file after macro,line: %d):  incorrect first character in label name\n" , lineNum);
               continue;
            }
			   if(LegalLabelForEntryAndExtern(line,index) == 3) 
            {
               errorNum++;
               printf("error(file after macro,line: %d): label name exceeds the limit \n" , lineNum);
               continue;
            }
            if(LegalLabelForEntryAndExtern(line,index) == 2)
            {
               errorNum++;
               printf("error(file after macro,line: %d): incorrect character in label name \n" , lineNum);
            	continue;
            }
         	if(LegalLabelForEntryAndExtern(line,index) == 1)
				{
					memset(name , '\0' ,MAX);/* puting null in arr */
					nindex = 0;
					while (!isspace(line[index])) /* reaching a char which is the name */
               {
   	         	name[nindex] = line[index]; /* put the value in name array */
                  nindex++;
                  index++;
               }	  
					currentle = head;
	         	while(currentle->labelName != NULL)
		         {
			      	if(strcmp(currentle->labelName,name))
				   	{
			    	      currentle->entry = 1;
			         }
	          		currentle = currentle->next;
		         }
				}
				continue; 
			}
			/* 2-7 */
			if(isCommandZero(line,index))
			{
				IC++;/* first word is set */
				L = 0;
			}
			if(isCommandOne(line,index))
			{
				index = index +3; /* skip command */
				IC++;/* first word is set */
				while( (isspace(line[index])) && (line[index] != '\n') ) /* skipping spaces and reaching a char */
		      {
		      	index++;
		      }/* here we encounter the input */
				if(commandOneOp == 0)
				{
					L = 1;
				   memset(numArr , '\0' ,MAX);/* puting null in num arr */
					numIndex = 0;
					index++;/* sklip # */						
					if(line[index] == '+')
		         {
		            index++;
		         }
		         if(line[index] == '-')
		         {
					   numArr[numIndex] = '-' ;
	               index++;
                  numIndex++;
		         }
					while( (line[index] >= '0') && (line[index] <= '9') )
					{
				      numArr[numIndex] = line[index];
                  numIndex++;
                  index++;
               }
               num = numArr;/* add num word to data */
               num =decimalToBinary(num);
               command[IC] = num;
               /* add ARE */
					command[IC][8] = 0; 
					command[IC][9] = 0;
				}/* end of # case */
				if(commandOneOp == 3)/* registr case */
				{
					L = 1;
					index++;/* sklip r */
					num = line[index];
					num =decimalToBinary(num);
					command[IC] = num;
					/* add ARE */
					command[IC][8] = 0; 
					command[IC][9] = 0;	
				}/* end of registr case */
				if(commandOneOp == 2)/* scruct case */ 
				{
					L = 2;
					memset(name , '\0' ,MAX);/* puting null in arr */
					nindex = 0;
					while (line[index] != '.') /* reaching the end of struct name  */
               {
   	         	name[nindex] = line[index]; /* put the value in name array */
                  nindex++;
                  index++;
               }
					currentle = head;
					while(currentle->labelName != NULL)
		         {
			      	if(strcmp(currentle->labelName,name))
				   	{
			    	      num = currentle->icAddress;
							command[IC] = num;
							if(currentle->external != NULL)
							{
								/* add ARE */
								command[IC][8] = 1; 
								command[IC][9] = 0;
								command[IC+1][8] = 1; 
								command[IC+1][9] = 0;
							}else{
									  /* add ARE */
							     	  command[IC][8] = 0; 
								     command[IC][9] = 1;
									  command[IC+1][8] = 0; 
								     command[IC+1][9] = 1;
								  }	
							continue;
			         }
	          		currentle = currentle->next;
		         }
					index++;/* skip the . */
					command[IC+1][0] = 0;
               command[IC+1][1] = 0;
					command[IC+1][2] = 0;
					command[IC+1][3] = 0;
					command[IC+1][4] = 0;
					command[IC+1][5] = 0;
          		if(line[index] == 1)
					{
						command[IC+1][6] = 0; 
					   command[IC+1][7] = 1;
					}else{
							  command[IC+1][6] = 1; 
					        command[IC+1][7] = 0;  
						  }
				}/* end of struct case */
				if(commandOneOp == 1)/* label case */
				{
					L = 1;
               memset(name , '\0' ,MAX);/* puting null in arr */
					nindex = 0;
					while (!isspace(line[index])) /* reaching the end of struct name  */
               {
   	         	name[nindex] = line[index]; /* put the value in name array */
                  nindex++;
                  index++;
               }
					currentle = head;
					while(currentle->labelName != NULL)
		         {
			      	if(strcmp(currentle->labelName,name))
				   	{
							num = currentle->icAddress;
						   command[IC] = num;
							if(currentle->external != NULL)
							{
								/* add ARE */
								command[IC][8] = 1; 
								command[IC][9] = 0;
							}else{
									  /* add ARE */
							     	  command[IC][8] = 0; 
								     command[IC][9] = 1;
								  }	
							continue;
			         }
	          		currentle = currentle->next;
               }
				}/* end of label case */	
			}/*  end of command one case */
			if(isCommandTwo(line,index))
			{
				firstIsR =0;
				index = index +3; /* skip command */	
				IC++;/* first word is set */
				while( (isspace(line[index])) && (line[index] != '\n') ) /* reaching a char */
				{
				   index++;
				}/* here we encounter the first input */
				if(commandTwoFirstOp == 0)
				{
					L = 1;
					memset(numArr , '\0' ,MAX);/* puting null in num arr */
					numIndex = 0;
					index++;/* sklip # */						
					if(line[index] == '+')
				   {
				   	index++;
				   }
				   if(line[index] == '-')
				   {
						numArr[numIndex] = '-' ;
			         index++;
		            numIndex++;
				   }
					while( (line[index] >= '0') && (line[index] <= '9') )
					{
						numArr[numIndex] = line[index];
		            numIndex++;
		            index++;
		         }
		         num = numArr;/* add num word to data */
		         num =decimalToBinary(num);
		         command[IC] = num;
		         /* add ARE */
					command[IC][8] = 0; 
					command[IC][9] = 0;
				}/* end of # case */
				if(commandTwoFirstOp == 3)/* registr case */
				{
					firstIsR =1;
					L = 1;
					index++;/* sklip r */
					num = line[index];
					num =decimalToBinary(num);
					command[IC] = num;
					/* add ARE */
					command[IC][8] = 0; 
					command[IC][9] = 0;	
				}/* end of registr case */
				if(commandTwoFirstOp == 2)/* scruct case */ 
				{
					L = 2;
					memset(name , '\0' ,MAX);/* puting null in arr */
					nindex = 0;
					while (line[index] != '.') /* reaching the end of struct name  */
		         {
			      	name[nindex] = line[index]; /* put the value in name array */
		            nindex++;
		            index++;
		         }
					currentle = head;
					while(currentle->labelName != NULL)
				   {
					   if(strcmp(currentle->labelName,name))
						{
					 	   num = currentle->icAddress;
							command[IC] = num;
							if(currentle->external != NULL)
							{
								/* add ARE */
								command[IC][8] = 1; 
								command[IC][9] = 0;
								command[IC+1][8] = 1; 
								command[IC+1][9] = 0;
							}else{
									  /* add ARE */
									  command[IC][8] = 0; 
									  command[IC][9] = 1;
									  command[IC+1][8] = 0; 
									  command[IC+1][9] = 1;
									}	
							continue;
					   }
			         currentle = currentle->next;
				   }
					index++;/* skip the . */
					command[IC+1][0] = 0;
		         command[IC+1][1] = 0;
					command[IC+1][2] = 0;
					command[IC+1][3] = 0;
					command[IC+1][4] = 0;
					command[IC+1][5] = 0;
		       	if(line[index] == 1)
					{
						command[IC+1][6] = 0; 
						command[IC+1][7] = 1;
					}else{
							  command[IC+1][6] = 1; 
							  command[IC+1][7] = 0;  
						  }
			   }/* end of struct case */
				if(commandTwoFirstOp == 1)/* label case */
				{
					L = 1;
		         memset(name , '\0' ,MAX);/* puting null in arr */
					nindex = 0;
					while (!isspace(line[index])) /* reaching the end of struct name  */
		         {
			         name[nindex] = line[index]; /* put the value in name array */
		            nindex++;
		            index++;
		         }
					currentle = head;
					while(currentle->labelName != NULL)
				   {
					   if(strcmp(currentle->labelName,name))
						{
							num = currentle->icAddress;
							command[IC] = num;
							if(currentle->external != NULL)
							{
								/* add ARE */
								command[IC][8] = 1; 
								command[IC][9] = 0;
							}else{
									/* add ARE */
									  command[IC][8] = 0; 
									  command[IC][9] = 1;
								  }	
							continue;
					   }
			       	currentle = currentle->next;
					}
				}/* end of label case */
				while(line[index] != ',') /* skiping first op */
				{
					index++;
				}
/* 2-8a */  IC = IC + L;
				L =0;
				index++; /* skiping the , */
				if(commandOneOp == 0)
				{
					L = 1;
				   memset(numArr , '\0' ,MAX);/* puting null in num arr */
					numIndex = 0;
					index++;/* sklip # */						
					if(line[index] == '+')
		         {
		            index++;
		         }
		         if(line[index] == '-')
		         {
					   numArr[numIndex] = '-' ;
	               index++;
                  numIndex++;
		         }
					while( (line[index] >= '0') && (line[index] <= '9') )
					{
				      numArr[numIndex] = line[index];
                  numIndex++;
                  index++;
               }
               num = numArr;/* add num word to data */
               num =decimalToBinary(num);
               command[IC] = num;
               /* add ARE */
					command[IC][8] = 0; 
					command[IC][9] = 0;
				}/* end of # case */
				if(commandOneOp == 3)/* registr case */
				{
					if(firstIsR != 1)
					{
					   L = 1;
					   index++;/* sklip r */
					   num = line[index];
					   num =decimalToBinary(num);
/*FIX to 4-7arr*/  command[IC]= num;
					   /* add ARE */
					   command[IC][8] = 0; 
					   command[IC][9] = 0;	
					}else{
							  L = 0;
					        index++;/* sklip r */
					        num = line[index];
					        num =decimalToBinary(num);
/*FIX to 9-6arr*/      command[IC-1]= num;
					        /* add ARE */
					        command[IC-1][8] = 0; 
					        command[IC-1][9] = 0;
						  }
				}/* end of registr case */
				if(commandOneOp == 2)/* scruct case */ 
				{
					L = 2;
					memset(name , '\0' ,MAX);/* puting null in arr */
					nindex = 0;
					while (line[index] != '.') /* reaching the end of struct name  */
               {
   	         	name[nindex] = line[index]; /* put the value in name array */
                  nindex++;
                  index++;
               }
					currentle = head;
					while(currentle->labelName != NULL)
		         {
			      	if(strcmp(currentle->labelName,name))
				   	{
			    	      num = currentle->icAddress;
							command[IC] = num;
							if(currentle->external != NULL)
							{
								/* add ARE */
								command[IC][8] = 1; 
								command[IC][9] = 0;
								command[IC+1][8] = 1; 
								command[IC+1][9] = 0;
							}else{
									  /* add ARE */
							     	  command[IC][8] = 0; 
								     command[IC][9] = 1;
									  command[IC+1][8] = 0; 
								     command[IC+1][9] = 1;
								  }	
							continue;
			         }
	          		currentle = currentle->next;
		         }
					index++;/* skip the . */
					command[IC+1][0] = 0;
               command[IC+1][1] = 0;
					command[IC+1][2] = 0;
					command[IC+1][3] = 0;
					command[IC+1][4] = 0;
					command[IC+1][5] = 0;
          		if(line[index] == 1)
					{
						command[IC+1][6] = 0; 
					   command[IC+1][7] = 1;
					}else{
							  command[IC+1][6] = 1; 
					        command[IC+1][7] = 0;  
						  }
				}/* end of struct case */
				if(commandOneOp == 1)/* label case */
				{
					L = 1;
               memset(name , '\0' ,MAX);/* puting null in arr */
					nindex = 0;
					while (!isspace(line[index])) /* reaching the end of struct name  */
               {
   	         	name[nindex] = line[index]; /* put the value in name array */
                  nindex++;
                  index++;
               }
					currentle = head;
					while(currentle->labelName != NULL)
		         {
			      	if(strcmp(currentle->labelName,name))
				   	{
							num = currentle->icAddress;
						   command[IC] = num;
							if(currentle->external != NULL)
							{
								/* add ARE */
								command[IC][8] = 1; 
								command[IC][9] = 0;
							}else{
									  /* add ARE */
							     	  command[IC][8] = 0; 
								     command[IC][9] = 1;
								  }	
							continue;
			         }
	          		currentle = currentle->next;
				}/* end of label case */	
			}/* end of command with 2 op */	
			/* 2-8b */ 
			IC = IC + L;
			L =0;
			/* 2-9 */
			continue;			
		}
		/* 2-10 */
		if(errorNum != 0)
		{
			printf("assembler stops due to errors founds in file aftert macro \n" , lineNum);
			continue;
		}
		/* 2- 11 */ 
		printf("finshed secound read on %s file\n",macroName);
		printf("start obfile %s file\n",macroName);





		file = fopen(obF,"w");
		if(file == NULL) /* failed to open the file */
	   {      
	      printf("error: cant open the after macro file.\n" );
	   }
		file = fopen(extF,"w");
		if(file == NULL) /* failed to open the file */
	   {      
	      printf("error: cant open the after macro file.\n" );
	   }
		file = fopen(entF,"w");
		if(file == NULL) /* failed to open the file */
	   {      
	      printf("error: cant open the after macro file.\n" );
	   }



		/* Messages to user */
      printf("created the %s\n",macroName);
      printf("starting first read on %s file\n",macroName);
		
		
		  
		

		free(head);
   }/* end of file loop */   
	return 1;
}/* end of main */









