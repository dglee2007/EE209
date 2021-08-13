/* Donggeon Lee Assignment#2, sgrep.c*/

/* Part 2 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h> /* for skeleton code */
#include <unistd.h> /* for getopt */
#include "str.h"

#define MAX_STR_LEN 1023

#define FALSE 0
#define TRUE  1
int num_A=0;               // count number of '*'s in pattern 

int Astx[MAX_STR_LEN+1];     //saves the order where '*' appears at pattern
                             


/* 
   PrintUsage()
   print out the usage of the Simple Grep Program
   while the argument has parsing error            */

void 
PrintUsage(const char* argv0) 
{
  const static char *fmt = 
	  "Simple Grep (sgrep) Usage:\n"
	  "%s pattern [stdin]\n";

  printf(fmt, argv0);
}

/* 
 SearchPattern()
-Input: pointer to string(pattern)
-Usage: Gets one line each from stdin and searches whether
        identical string with pattern exists. If it does,
        it prints out the line to the stdout. If pattern
        includes '*', it regards as any character sequence.
-return value: returns FALSE if the length of input line is
               bigger than MAX_STR_LEN. If not, returns TRUE.
 */
 
int
SearchPattern(const char *pattern)
{
  char buf[MAX_STR_LEN + 2]; 
  int len;

  int l;
  l = StrGetLength(pattern);
  if (l > MAX_STR_LEN) {            ///checking length of pattern
      fprintf(stderr, "Error: argument is too long\n");
      //prints error message to stdandard error stream
  }
  //to check # of '*'s and save the order
  Astx[0] = 0;
  for (int j = 1; j <= l; j++) {
      if (pattern[j - 1] == '*') {
          ++num_A;
          Astx[num_A] = j;           
      }
  }
  Astx[num_A + 1] = l + 1;

  while (fgets(buf, sizeof(buf), stdin)) {
	  
    // print error message when line is longer than standard
    
    if ((len = StrGetLength(buf)) > MAX_STR_LEN) {  
      fprintf(stderr, "Error: input line is too long\n");
      return FALSE;
    }
    
    //when * is not on pattern
    if (num_A==0){
      if (StrFindStr(buf, pattern)){ 
	fprintf(stdout, "%s", buf);
	  }
    }
    
    //when * is on pattern
    else {
         
      char *flag = buf;
      for (int x=0;x!=(num_A+1);x++){
      
    //when * appears two in a row
	if ((Astx[x+1]-Astx[x])==1) {
	  if (x==num_A){
	    fprintf(stdout, "%s", buf);
	    continue;
	  }
	}
  
     //part is divided by *  
    	else{
	char part[Astx[x+1]-Astx[x]];
	for (int i=0; i!=sizeof(part)-1;i++){
	  part[i] = pattern[Astx[x]+i];
	  }
	part[sizeof(part)-1] = '\0';
	if(StrFindStr(flag, part)!=NULL){
	  flag = StrFindStr(flag, part)+StrGetLength(part);
	  if (x==num_A){
	    fprintf(stdout, "%s", buf);
	    }
	   }
	else{break;}
      }
    }
   }
  }
  return TRUE;
}

/* 
 main()
-Input: int argc and array of pointers(argv)
-Usage: checks whether argument is well parsed.
        If it is, it calls function SearchPattern().
        If not, it shows the usage of this Simple Grep program.
-return value: If argument has parsing error, returns EXIT_FAILURE.
               If not, it returns EXIT_SUCCESS while other functions
               returns TRUE. If not, it returns EXIT_FAILURE.
*/

int 
main(const int argc, const char *argv[]) 
{
  /* Do argument check and parsing */
    if (argc < 2) {
	  fprintf(stderr, "Error: argument parsing error\n");
	  PrintUsage(argv[0]);
	  return (EXIT_FAILURE);
    }

  return SearchPattern(argv[1]) ? EXIT_SUCCESS:EXIT_FAILURE;
}
