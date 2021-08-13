/*Donggeon Lee, 2, str.c*/
#include <assert.h> /* to use assert() */
#include <stdio.h>
#include <stdlib.h> /* for strtol() */
#include <string.h>
#include "str.h"
#include <limits.h>
#include <ctype.h>
/*#define LONG_MIN (-2147483647L - 1)   // Minimum (signed) long value
#define LONG_MAX   2147483647L   // Maximum (signed) long value
#define ULONG_MAX   0xffffffffUL   // Maximum unsigned long value

 Your task is: 
   1. Rewrite the body of "Part 1" functions - remove the current
      body that simply calls the corresponding C standard library
      function.
   2. Write appropriate comment per each function
*/

/* Part 1 */
/*------------------------------------------------------------------------*/
//Input: pointer to string pcSrc
//Output: Length of pcSrc
size_t StrGetLength(const char* pcSrc)
{
  const char *pcEnd;
  assert(pcSrc); /* NULL address, 0, and FALSE are identical. */
  pcEnd = pcSrc;
	
  while (*pcEnd) /* null character and FALSE are identical. */
    pcEnd++;

  return (size_t)(pcEnd - pcSrc);
}

/*------------------------------------------------------------------------*/
//Input: Pointer of strings pcDest and pcSrc
//Output: pointer of pcDest which is copied from pcSrc
char *StrCopy(char *pcDest, const char* pcSrc)
{
  assert(pcDest);
  assert(pcSrc);
  char *temp = pcDest;
  while((*pcSrc)!='\0'){
    *temp = *pcSrc;
    temp++; pcSrc++;
  }
  *temp='\0';
  return pcDest;
}

/*------------------------------------------------------------------------*/
//Input: Pointer of string pcS1 pcS2
//Output: if string is identical->return 0
//ASCII code of pcS1 is bigger->return positive value
//ASCII code of pcS2 is bigger->return negative value
int StrCompare(const char* pcS1, const char* pcS2)
{
  /* TODO: fill this function */
  assert(pcS1); 
  assert(pcS2);
  while ((*pcS1)!='\0' || (*pcS2)!='\0'){
    if ((*pcS1)!=(*pcS2)){return (*pcS1)-(*pcS2); }
    pcS1++;pcS2++;
  }
  return 0;
}
/*------------------------------------------------------------------------*/
//Input: Pointer of string pcHaystack and character
//Output: pointer of first found character or null
char *StrFindChr(const char* pcHaystack, int c)
{
  /* TODO: fill this function */
 assert(pcHaystack);
 if (c==0){
   while ((*pcHaystack)!='\0'){pcHaystack++;}
return (char*)pcHaystack;}
   
 while ((*pcHaystack)!='\0'){
   if ((*pcHaystack)==c){return (char *)pcHaystack;}
   else {pcHaystack++;}
 }
  return NULL;
}
/*------------------------------------------------------------------------*/
//Input: Pointer of strings pcHaystack and pcNeedle
//Output: String include another string->pointer of the first found string or null
char *StrFindStr(const char* pcHaystack, const char *pcNeedle)
{
  /* TODO: fill this function */
  assert(pcHaystack); assert(pcNeedle);
  size_t l=0;
  size_t needle = StrGetLength(pcNeedle);
  const char *check = NULL;
  const char *pivotH = pcHaystack;
  const char *pivotN = pcNeedle;
  if (StrGetLength(pcNeedle)==0){return (char*) pcHaystack;}
  while ((*pivotH)!='\0' && (*pivotN)!='\0'){
    if ((*pivotH)==(*pivotN)){
      check=pivotH;l++;
      while ((*pivotH)==(*pivotN)){
	pivotH++; pivotN++; l++;
	if (l==needle){return (char *)check;}}
    }
    else {pivotH++; pivotN= pcNeedle;}
  }
  return NULL;
}
/*------------------------------------------------------------------------*/
//Input: Pointer of a string pcDest pcSrc
//output: pointer of pcDest which is sum of two strings
char *StrConcat(char *pcDest, const char* pcSrc)
{
  /* TODO: fill this function */
  assert(pcDest);
  assert(pcSrc);
  char *temp = pcDest;
  const char *copy=pcSrc;
  size_t LS=StrGetLength(pcSrc);
  while ((*temp)!='\0'){temp++;}
  for (size_t i=0;i<LS+1;i++){(*temp)=(*copy);temp++;copy++;}
  return pcDest; 
}

/*------------------------------------------------------------------------*/
//Input: Pointer of string nptr, pointer of the pointer to endptr, and integer base
//Output: return 0 when base!=10
//        return long integer which is from nptr
long int StrToLong(const char *nptr, char **endptr, int base)
{
  /* handle only when base is 10 */
  if (base != 10) return 0;
  assert(nptr); assert(&endptr);
  const char *p;
  p=nptr;
  long flag=1;
  long number=0;
  long maxval=LONG_MAX/10;
  long num;
  while ((*p)!='\0' && isspace(*p)){p++;}
  if (*p=='-'){flag=-1;p++;}
  else if (*p=='+'){p++;}
  while (*p && isdigit(*p)){
    if (flag==1){
      if (number<maxval){
	num=*p-'0';
        number=number*10+num;
        p++;
      }
      else if (number==maxval){
        if (*p>7){
          return LONG_MAX;
        }
        else {
	  num=*p-'0';
          number=number*10+num;
          p++;
        }
      }
      else {return LONG_MAX;}
    }
    else if (flag==-1){
      if (number<maxval){
	num=*p-'0';
        number=number*10+num;
        p++;
      }
      else if (number==maxval){
        if (*p>8){
          return LONG_MIN;
        }
        else {
	  num=*p-'0';
          number=number*10+num;
          p++;
        }
      }
      else {return LONG_MIN;}
    }
  }
  if (endptr != NULL) {*endptr = (char *) p;}
  return number*flag;
  
  /* TODO: fill this function */
  //return strtol(nptr, endptr, 10);
}
