/*20200443 Donggeon Lee, Assignment1, decomment.c*/
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

int flag_r=0;//show state when text is on remark mode
int flag_q=0;//show state when text is on quote mode
int flag_a=0;//show state when text is on apostrophe
int line=0;//identify the number of line where pointer is on
int line_r=0;//identify the number of line where remark has started
int c;//defines the type of getchar()

/*When there is more than one slash during it is not remark mode*/
//This function will verify the next character
//Verify the whether remark will start or not
void doubleslash(){
  c=getchar();
  if (c=='/'){ //when c is '/' again
    doubleslash(); //recursion
  }
  else if (c=='*'){
    flag_r=1;//remark mode on
    printf("%c",' ');//print the space
  }
  else if (c=='\n'){
    line++;
  }
  else {
    printf("%c",'/');
    printf("%c",c);
  }
}
/*when there is more than one star during remark mode*/
//This function will verify the next character
//verify whether remark will end or not
void doublestar(){
  c=getchar();
  if (c=='*'){//when c is '*' again
    doublestar();//recursion
  }
  else if (c=='\n'){
    line++;
  }
  else if (c=='/'){
    flag_r=0;//remark mode off
  }
}
//main function reads a character&modifies the flag state as a character changes
//Writes modified text to standard output stream
//Writes error message to standard error stream
int main(){
  if ((c=getchar())!=EOF){//preventing from plain text file
    printf("%c",c);
    line=1;
  }
  while ((c=getchar())!=EOF){//until end of file
    if (flag_r==0){//remark mode off
      if (flag_q==0 && flag_a==0){//no quotes, no apostrophe
        if (c=='/'){
          c=getchar();
          if (c=='*'){
            flag_r=1;//remark mode on
            printf("%c",' ');
            line_r=line;}//record the start point of remark
	  else if (c=='/'){
	    printf("%c",'/');
	    doubleslash();}//calling doubleslash function
	  else {
	    printf("%c",'/');
	    printf("%c",c);}
          }
        else if (c=='\n'){
          printf("%c",c);
          line++;
        }
        else if (c==34){
          flag_q=1;//quote start
          printf("%c",c);
        }
        else if (c==39){
          flag_a=1;//apostrophe start
          printf("%c",c);
        }
        else {printf("%c",c);}
      }
      else if (flag_q==1){//during quote mode
        if (c==34){
          flag_q=0;
          printf("%c",c);
        }
        else if (c=='\n'){
          printf("%c",c);
          line++;
        }
        else {printf("%c",c);}
      }
      else if (flag_a==1){//during apostrophe mode
        if (c==39){
          flag_a=0;
          printf("%c",c);
        }
        else if (c=='\n'){
          printf("%c",'\n');
          line++;
        }
        else {printf("%c",c);}
      }
    }
    else if (flag_r==1){//during remark mode
      if (c=='*'){
	c=getchar();
	if (c=='/'){
	  flag_r=0;}
	else if (c=='*'){doublestar();}//calling doublestar function
	else if (c=='\n'){
	  printf("%c",'\n');}
      }
      else if (c=='\n'){
	line++;
	printf("%c",c);}
    }
  }
  if (flag_r==1){//if still remark mode at the end of file
    fprintf(stderr,"Error: line %d: unterminated comment\n",line_r);//print error and the start point of the remark on standard error stream
    return EXIT_FAILURE;}
  else if (flag_r==0){//if remark mode off
    return EXIT_SUCCESS;}
}
