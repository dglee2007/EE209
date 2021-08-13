/* 20200443 Donggeon Lee 5 dfa.c */
#include "dynarray.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

enum {MAX_LINE_SIZE = 1024};
enum {FALSE, TRUE};

/* A Token is expressed as a string. */
struct Token{
   char *pcValue;
};

/* Free token pvItem.  pvExtra is unused. */
static void freeToken(void *pvItem, void *pvExtra){
  struct Token *psToken = (struct Token*)pvItem;
  free(psToken->pcValue);
  free(psToken);
}

/* Print token pvItem to stdout. pvExtra is unused.*/
static void printToken(void *pvItem, void *pvExtra){
  struct Token *psToken = (struct Token*)pvItem;
  printf("%s\n", psToken->pcValue);
}

/*get string as parameter and return Token which contains the string*/
static struct Token *makeToken(char *pcValue){
  struct Token *psToken;
  psToken = (struct Token*)malloc(sizeof(struct Token));
  if (psToken == NULL)
    return NULL;
  psToken->pcValue = (char*)malloc(strlen(pcValue) + 1);
  if (psToken->pcValue == NULL){
    free(psToken);
    return NULL;
  }
  strcpy(psToken->pcValue, pcValue);
  return psToken;
}

/*lenxLine()
  read from parameter pcLine and make Tokens which contains commamnds
  return the linked list of Tokens, oTokens*/
static int lexLine(const char *pcLine, DynArray_T oTokens){
  enum LexState {STATE_OUT, STATE_IN};
  enum LexState eState = STATE_OUT;
  
  int iLineIndex = 0; int iValueIndex = 0; char c= 1;
  char acValue[MAX_LINE_SIZE];
  struct Token *psToken;
  
  assert(pcLine != NULL);
  assert(oTokens != NULL);
  
  while(c!='\0'){
      /* "Read" the next character from pcLine. */
      c = pcLine[iLineIndex++];
      
      switch (eState){
	/*c was space characther or '|', '<', '>'*/
      case STATE_OUT:
	/*c is space characther or '|', '<', '>'*/
	if ((isspace(c)!=0) || (c=='|') || (c=='<') || (c=='>')){
	  if((c=='|') || (c=='<') || (c=='>')){
	    acValue[0]=c; acValue[1]='\0';
	    psToken = makeToken(acValue);
	    if (psToken == NULL){
	      fprintf(stderr, "Cannot allocate memory\n");
	      return FALSE;}
	    if (! DynArray_add(oTokens, psToken)){
	      fprintf(stderr, "Cannot allocate memory\n");
	      return FALSE;}
	      }
	}
	else{/*c is character in word*/
	  if(c=='"'){
	    /*make a token of string which located between two '"'*/
	    c = pcLine[iLineIndex++];
	    while(c!='"'){
	      if(c=='\0'||c==EOF) {return FALSE;}
	      if((c=='|') || (c=='<') || (c=='>')){
		char t=c; c='"';
		acValue[iValueIndex++] = c;
		acValue[iValueIndex++] = t;}
	      else{ acValue[iValueIndex++] = c;}
	      c = pcLine[iLineIndex++];
	    }
	    eState = STATE_IN;
	  }
	  else{/*c is normal character in word*/
	    acValue[iValueIndex++] = c;
	    eState = STATE_IN;}
	}
	break;
	/*c was character in word*/
      case STATE_IN:
	/*single command is completed*/
	if ((isspace(c)!=0) || (c=='|') || (c=='<') || (c=='>')){
	    acValue[iValueIndex] = '\0';
	    psToken = makeToken(acValue);
	    if (psToken == NULL){
		fprintf(stderr, "Cannot allocate memory\n");
		return FALSE;}
	    if (! DynArray_add(oTokens, psToken)){
	      fprintf(stderr, "Cannot allocate memory\n");
	      return FALSE;}
	    
	    iValueIndex = 0;
	    /*make one more token if c is pipe or redireciton*/
	    if((c=='|') || (c=='<') || (c=='>')){
	      acValue[0]=c; acValue[1]='\0';
	      psToken = makeToken(acValue);
	      if (psToken == NULL){
		fprintf(stderr, "Cannot allocate memory\n");
		return FALSE;}
	      if (! DynArray_add(oTokens, psToken)){
		fprintf(stderr, "Cannot allocate memory\n");
		return FALSE;}
	    }
	    eState = STATE_OUT;
	  }
	else{/*single command is still extending*/
	      if(c=='"'){
		c = pcLine[iLineIndex++];
		while(c!='"'){/*connects the characters between '"'*/
		  if(c=='\0'||c==EOF) {return FALSE;}
		  acValue[iValueIndex++] = c;
		  c = pcLine[iLineIndex++];}
		eState = STATE_IN;}
	      else{
		acValue[iValueIndex++] = c;
                eState = STATE_IN;}
	  }
	break;
	  
      default:
	assert(FALSE);
      }
  }
  return TRUE;
}

/*TokenArray()
  make a Token array from parameter line by calling lexline
  if it fails for specific reason, prints the proper message */
DynArray_T TokenArray(char *line){
   char acLine[MAX_LINE_SIZE];
   strcpy(acLine, line);
   DynArray_T oTokens;
   int iSuccessful;

   oTokens = DynArray_new(0);
   if (oTokens == NULL){
       fprintf(stderr, "Cannot allocate memory\n");
       exit(EXIT_FAILURE);
     }
   
   iSuccessful = lexLine(acLine, oTokens);
   if (iSuccessful){
       return oTokens;
       /*free used when it failed to make Token array*/
       DynArray_map(oTokens, printToken, NULL);
     }
   DynArray_map(oTokens, freeToken, NULL);
   DynArray_free(oTokens);
   
   return NULL;
}
