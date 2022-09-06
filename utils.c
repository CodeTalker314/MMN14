#include "utils.h"

const char symbols[32] = {'!','@','#','$','%','^','&','*','<','>','a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v'};

unsigned int bits(unsigned int str, int start, int end)
{
	unsigned int rslt;
	int length = end - start + 1;
	unsigned int mask = (int) pow(2, length) - 1;
	mask <<= start;
	rslt = str & mask;
	rslt >>= start;
	return rslt;
}


char * binaryToBase(char *binary)
{
  int count=0;
    int num=0;
    int i=0;
    int position=1;
    char * base = malloc(3*sizeof(char));
    if(!base)
        return "`Dynamic allocatio error.\0";
    for (i=9;i>-1;i--){
        num+=pow(2,count)*(binary[i]-'0');
        count++;
        if(count==5){
            base[position]=symbols[num];
            position--;
            count=0;

            num=0;
        }
    }
    base[2]='\0';
    return base;
}


char * decimalToBase(int num)
{
  char *base32_rslt = (char *) malloc(3);
  base32_rslt[0] = symbols[bits(num, 5, 9)];
  base32_rslt[1] = symbols[bits(num, 0, 4)];
  base32_rslt[2] = '\0';
  return base32_rslt;
}


int binaryToDecimal(char *binary)
{
  int i;
  int decimal = 0;
  for (i=9; i>-1; i--)
  {
    decimal += pow(2,9-i) * (binary[i] - '0');
  }
    if(binary[0] == '1')
        return 0 - (pow(2,10) -decimal);
    return decimal;
}


char * decimalToBinary(int num)
{
  unsigned int i = 0, abs = num;
  int j;
  char temp;
  char * binary = (char *)malloc(11* sizeof(char));
  if (!binary)
      return "Dynamic allocation error.\0";
  while (i < 10)
  {
      binary[i] = abs % 2 + '0';
      abs = abs / 2;
      i++;
  }
  for (j=0; j<5; j++)
  {
      temp = binary[9 - j];
      binary[9 - j] = binary[j];
      binary[j] = temp;
  }    
  binary[10] = '\0';
  return binary;
}


int endOfLine(char *line)
{
	return line == NULL || *line == '\0' || *line == '\n';
}


char* concat( char *s1, char *s2)
{
    char *result = malloc(strlen(s1)+strlen(s2)+1);
    if(result)
    {
        strcpy(result, s1); 
        strcat(result, s2);
        return result;
    }
    else
    {
        return NULL;
    }
}

char* asFile( const char* str1)
{
    char* result;
    asprintf(&result, "%s%s", str1, ".as\0");
    return result;
}

char* amFile( const char* str1)
{
    char* result;
    asprintf(&result, "%s%s", str1, ".am\0");
    return result;
}

char* textFile( const char* str1)
{
    char* result;
    asprintf(&result, "%s%s", str1, ".Text\0");
    return result;
}

char* obFile( const char* str1)
{
    char* result;
    asprintf(&result, "%s%s", str1, ".ob\0");
    return result;
}

char* extFile( const char* str1)
{
    char* result;
    asprintf(&result, "%s%s", str1, ".ext\0");
    return result;
}

char* entFile( const char* str1)
{
    char* result;
    asprintf(&result, "%s%s", str1, ".ext\0");
    return result;
}
