/*Donggeon Lee, 2, str.h*/
#ifndef _STR_H_
#define _STR_H_
#include <unistd.h> /* for typedef of size_t */

/* StrGetLength()
   - Receives a start address(pcSrc) of a string.
   - Returns the difference between end address of a string.*/
size_t StrGetLength(const char* pcSrc);

/* StrCopy()
   - Receives pointer of two string, pcDest and pcSrc. 
   - Copy all characters from pcSrc to pcDest.
   - Return the address of string, pcDest.*/
char *StrCopy(char *pcDest, const char* pcSrc);

/* StrCompare() 
   - Receive pointer of two string, pcS1 and pcS2. 
   - Returns the difference values of ASCII code of first 
   different character*/
int StrCompare(const char* pcS1, const char* pcS2);

/* Receives pointer of a string pcHaystack
Returns the pointer where c is found or return null when it is not found
*/
char *StrFindChr(const char *pcHaystack, int c);

/*Receives pointer of a string pcHaystack
Returns the pointer of the start address of substring if found or return null
*/
char *StrFindStr(const char* pcHaystack, const char *pcNeedle);

/*Receives two string pointer and returns address of pcDest which is concatenation of two strings
*/
char *StrConcat(char *pcDest, const char* pcSrc);

/*Receive string pointer that has numbers and return with long int type
*/
long int StrToLong(const char *nptr, char **endptr, int base);

#endif /* _STR_H_ */
