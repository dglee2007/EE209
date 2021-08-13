/* 20200443 Donggeon Lee 5 dfa.h*/
#ifndef DFA_INCLUDED
#define DFA_INCLUDED

/* A Token is expressed as a string.*/
struct Token{
  char *pcValue;
};

/* Free token pvItem.  pvExtra is unused.*/
void freeToken(void *pvItem, void *pvExtra);

/* Print token pvItem to stdout. pvExtra is unused.*/
void printToken(void *pvItem, void *pvExtra);

/*get string as parameter and return Token which contains the string*/
struct Token *makeToken(char *pcValue);

/*lenxLine()
  read from parameter pcLine and make Tokens which contains commamnds
  return the linked list of Tokens, oTokens*/
int lexLine(const char *pcLine, DynArray_T oTokens);

/*TokenArray()
  make a Token array from parameter line by calling lexline
  if it fails for specific reason, prints the proper message */
DynArray_T TokenArray(char *line);

#endif
