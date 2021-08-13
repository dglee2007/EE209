/*20200443 Donggeon Lee 5 ish.c*/
#define _GNU_SOURCE
#include <assert.h>
#include <signal.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <wait.h>
#include <errno.h>
#include <sys/types.h>
#include "dynarray.h"
#include "dfa.h"

enum {MAX_LINE_SIZE = 1024};
enum {SUCCESS, FAIL};
enum {BUILT_IN, NOT_BUILT_IN};
enum {INPUT, OUTPUT};
enum {EXE_FILE, EXE_PROMPT};

/* If SIGQUIT raises, exit the process */
static void termHandler(int iSig){
  exit(0);
}

/* If SIGQUIT raises, print the message and set the alarm
   Handle SIGQUIT to exit if second SIGQUIT raises*/
static void quitHandler(int iSig){
  char * temp = "\nType Ctrl-\\ again within 5 seconds to exit.\n% ";
  write(1, temp, strlen(temp));
  alarm(5);
  void (*Ret)(int);
  Ret = signal(SIGQUIT, termHandler);
  assert(Ret!=SIG_ERR);
}

/* If SIGALRM raises, reset the changes caused by SIGQUIT*/
static void alarmhandler(int iSig){
  void (*Ret)(int);
  Ret = signal(SIGQUIT, quitHandler);
  assert(Ret!=SIG_ERR); 
}

/*syntatic() :Ffind syntatic error of Tokens if exist
  If there is no error, it will return SUCCESS
  If there is an error, print message and return FAIL*/
int syntatic(DynArray_T Tokens, char *argv[]){
  assert(Tokens!=NULL); 
  int length=0, in_num=0, out_num=0, pipe_num=0;
  int in=1024; int out=1024; 
  int pipe_max=-1; int pipe_min=1024;
  int pipe[MAX_LINE_SIZE]={0,};
  void *pItem; struct Token *pToken; char *check;
  char *error1="Pipe or redirection destination not specified\n";
  char *error2="Command name missing\n";
  char *error3="Multiple redirection of stdin/stdout\n";
  
  length = DynArray_getLength(Tokens);
  /* find number and position of pipe and redirection in Tokens*/
  for(int i=0; i<length; i++){
    pItem = DynArray_get(Tokens, i);
    pToken = (struct Token*)pItem;
    check = pToken->pcValue;
    if(strcmp(check, "|")==0){
      pipe[pipe_num]=i;
      if(i>pipe_max)pipe_max=i;
      if(i<pipe_min)pipe_min=i;
      pipe_num++; }
    else if(strcmp(check, "<")==0){
      in=i; in_num++; }
    else if(strcmp(check, ">")==0){
      out=i; out_num++; }
  }
  /*in_num and out_num are redirected more than once*/
  if(in_num>1 || out_num>1){
    fprintf(stderr, "%s: %s", argv[0], error3);
    return FAIL;}
  /*Pipe located earlier than in_stream redirection*/
  if(pipe_num>0 && in_num>0){
    if(in-1 > pipe_min){
      fprintf(stderr, "%s: %s", argv[0], error3);
      return FAIL;}
    else if(in+1==pipe_min||in-1==pipe_min){
      fprintf(stderr, "%s: %s", argv[0], error1);
      return FAIL;}
  }
  /*Pipe is located later than out_stream rediretion*/
  if(pipe_num>0 && out_num>0){
    if(out+1 < pipe_max){
      fprintf(stderr, "%s: %s", argv[0], error3);
      return FAIL;}
     else if(out-1==pipe_max||out+1==pipe_max){
       fprintf(stderr, "%s: %s", argv[0], error1);
       return FAIL;}
  }
  if(pipe_max==length-1||in==length-1||out==length-1){
    fprintf(stderr, "%s: %s", argv[0], error1);
    return FAIL;}
  if(pipe_min==0||in==0||out==0){
    fprintf(stderr, "%s: %s", argv[0], error2);
    return FAIL;}
  if(pipe_num>1){
    for(int j=0; j<=pipe_num; j++){
      if(pipe[j+1]-pipe[j]==1){
	fprintf(stderr, "%s: %s", argv[0], error1);
	return FAIL;}
    }
  }
  return SUCCESS;
}

/*find positions of difference in Tokens and save them in pivot*/
void difference(DynArray_T Tokens, int *pivot){
  assert(Tokens!=NULL);
  int length; void *pItem; struct Token *pToken; char *check;
  length = DynArray_getLength(Tokens);
  for(int i=0; i<length; i++){
    pItem = DynArray_get(Tokens, i);
    pToken = (struct Token*)pItem;
    check = pToken->pcValue;
    if(strcmp(check, "<")==0){
      pivot[0]=i;}
    else if(strcmp(check, ">")==0){
      pivot[1]=i;}
  }
}

/*redirect(): 
  get the file name from Tokens by using number in pivot
  if success to open file, redirect stdin or stdout
  else print proper error message */
int redirect(DynArray_T Tokens, int pivot, int direct, char *argv[]){
  assert(Tokens!=NULL);
  void* pItem; struct Token *pToken; char *dest;
  pItem = DynArray_removeAt(Tokens, pivot);
  pItem = DynArray_removeAt(Tokens, pivot);
  pToken = (struct Token*)pItem;
  dest = pToken->pcValue;
  if(direct==INPUT){ //redirect stdin
    int fd; fd = open(dest, O_RDONLY);
    if(fd==-1){perror(argv[0]); return FAIL;}
    dup2(fd, STDIN_FILENO);
    close(fd); return SUCCESS;}
  else{ //redirect stdout
    int fd; fd = open(dest, O_WRONLY | O_CREAT| O_TRUNC, 0600);
    if(fd==-1){perror(argv[0]); return FAIL;}
    dup2(fd, STDOUT_FILENO);
    close(fd); return SUCCESS;}
}

/*get string in first Token of Tokens and return it*/
char *getvalue(DynArray_T Tokens){
  assert(Tokens!=NULL);
  void* pItem; struct Token *pToken; char *check;
  pItem = DynArray_removeAt(Tokens, 0);
  pToken = (struct Token*)pItem;
  check = pToken->pcValue;
  return check;
}

/*return BUIT_IN when parameter string is builtin commands
  return NOT_BUILT_IN for other strings*/
int cmd_type(char *apcArgv){
  if(strcmp(apcArgv, "setenv")==0){return BUILT_IN;}
  else if(strcmp(apcArgv, "unsetenv")==0){return BUILT_IN;}
  else if(strcmp(apcArgv, "cd")==0){return BUILT_IN;}
  else if(strcmp(apcArgv, "exit")==0){return BUILT_IN;}
  else {return NOT_BUILT_IN;}
}

/*built_in(): apcArgv contains builtin command and parameter
  call proper function ( chdir, setenv, unsetenv, exit)
  print error when parameter set is not fine or fail to call function */
void built_in(char **apcArgv, char *argv[]){
  int length=0;  int result;
  while (apcArgv[length] != NULL) { length++; }
  if(strcmp(apcArgv[0], "setenv")==0){
    if(length==2){result=setenv(apcArgv[1], "", 0);
      if(result==-1){perror(argv[0]);}}
    else if(length==3){result=setenv(apcArgv[1], apcArgv[2], 0);
      if(result==-1){perror(argv[0]);}}
    else{fprintf(stderr,
		 "%s: setenv takes one or two parameters\n", argv[0]);}
  }
  else if(strcmp(apcArgv[0], "unsetenv")==0){
    if(length==2){result=unsetenv(apcArgv[1]);
      if(result==-1){perror(argv[0]);}}
    else{fprintf(stderr,
		 "%s: unsetenv takes one parameter\n", argv[0]);}
  }
  else if(strcmp(apcArgv[0], "cd")==0){
    if(length==2){
      result = chdir(apcArgv[1]);
      if(result==-1){perror(argv[0]);}
    }
    else{ fprintf(stderr, "%s: cd takes one parameter\n", argv[0]);}
  }
  else if(strcmp(apcArgv[0], "exit")==0){
    exit(0);
  }
}

/* check the number of pipe and return it*/
int cmdNum(DynArray_T Tokens){
  int i=0; int n=0; void* pItem; struct Token *pToken; char *check;
  while(i<DynArray_getLength(Tokens)){
    pItem = DynArray_get(Tokens, i++);
    pToken = (struct Token*)pItem;
    check = pToken->pcValue;
    if(strcmp(check, "|")==0){n++;}
  }
  return n+1;
}

/*Get commands from Tokens and make a string array that will be returned*/
char **getCmdline(DynArray_T Tokens){
  char *arg="a"; int i=0; 
  char **apcArgv = calloc(2, sizeof(char*));
  while(strcmp(arg, "|")!=0 && DynArray_getLength(Tokens)!=0){
    arg=getvalue(Tokens);
    apcArgv=(char **)realloc(apcArgv, (i+2)*sizeof(char*));
    apcArgv[i+1]=NULL;
    apcArgv[i++]=arg;
  }
  for (int k = 0; k < i; k++) {
      if (apcArgv[k][0] == '"') {
          apcArgv[k][0] = '\0';
          apcArgv[k] = &apcArgv[k][1];
      }
  }
  return apcArgv;
}

/*execute(): Call commands(apcArgv) by using execvp()
  Also, make child process when use execvp() */
void execute(char **apcArgv, char *argv[]){
  pid_t iPid; int status;
  fflush(NULL);
  iPid = fork();
  if(iPid == -1){
    perror(argv[0]);
    exit(0);
  }
  if(iPid==0){
    void (*pfRet)(int);
    pfRet = signal(SIGINT, SIG_DFL);
    assert(pfRet!=SIG_ERR);
    execvp(apcArgv[0], apcArgv);
    perror(argv[0]);
    abort();
  }
  else{
    wait(&status);
  }
}

/*Multiply sequences divided by pipe
  Make a pipe and redirection input and output using recursion
  If fail to redirect, execvp() or fork() prints message*/
void exeMulCmd(char ***apcArgv,int len,int i,int file,char *argv[]){
  if(len==i+1){//when apcArgv[i] is last command
    dup2(file, STDIN_FILENO);
    close(file);
    execute(apcArgv[i], argv); }
  else{
    int fds[2]; int pid; int status;
    if(pipe(fds)==-1){exit(-1);}
    
    fflush(NULL);
    pid = fork();
    if(pid==0){
      void (*pfRet)(int);
      pfRet = signal(SIGINT, SIG_DFL);
      assert(pfRet!=SIG_ERR);
      close(fds[0]);
      dup2(fds[1], STDOUT_FILENO);
      dup2(file, STDIN_FILENO);
      close(fds[1]); close(file);
      execvp(apcArgv[i][0], apcArgv[i]);
      perror(argv[0]);
      abort();}
    else{//recursion
      close(fds[1]);
      wait(&status);
      exeMulCmd(apcArgv, len, i+1, fds[0], argv);}
  }
}


int shell(char *line, char* argv[], int type){

  int analyzer; int num;
  if(type==EXE_FILE){//line is read from file
    printf("%s ", "%");
    printf("%s", line);
  }
  if(line[0]=='\n'){return 0;}
  
  DynArray_T Tokens;
  Tokens = TokenArray(line);
  if(Tokens==NULL){
    fprintf(stderr,"%s: Could not find quote pair\n", argv[0]);
    return 0;}
  
  //check if commands have syntatic error
  analyzer = syntatic(Tokens, argv);
  if(analyzer==FAIL){return 0;}
  
  //check if commands contain '<' or '>'
  int pivot[2]={0,};
  int tempIN=dup(STDIN_FILENO);
  int tempOUT=dup(STDOUT_FILENO);

  difference(Tokens, pivot);
  if(pivot[0]!=0){
    analyzer=redirect(Tokens, pivot[0], INPUT, argv);
    if(analyzer==FAIL){return 0;}
  }
  if(pivot[1]!=0){
    if(pivot[0]!=0){
      analyzer=redirect(Tokens, pivot[1]-2, OUTPUT, argv);
      if(analyzer==FAIL){return 0;}
    }
    else {analyzer=redirect(Tokens, pivot[1], OUTPUT, argv);
      if(analyzer==FAIL){return 0;}
    }
  }
  
  //divide commands by the existence of pipe
  num=cmdNum(Tokens);
  
  if(num==1){
      char** apcArgv;
      apcArgv = getCmdline(Tokens);
      if (cmd_type(apcArgv[0]) == BUILT_IN) {
          built_in(apcArgv, argv);
      }
      else { execute(apcArgv, argv); }
      free(apcArgv); }
  else{
    int len=0; char ***apcArgv;
    apcArgv=(char***)calloc(2, sizeof(char**));
    for(int j=0; j<num; j++){
      apcArgv=(char***)realloc(apcArgv, j+2);
      apcArgv[j]=getCmdline(Tokens);
      while (apcArgv[i] != NULL) { len++; }
      if(strcmp(apcArgv[j][len-1], "|")==0){
	  apcArgv[j][len-1]=NULL;}	
    }
    exeMulCmd(apcArgv, num, 0, STDIN_FILENO, argv);
    free(apcArgv);
  }
  //reset the stdin and stdout 
  dup2(tempIN, STDIN_FILENO);
  close(tempIN);
  dup2(tempOUT, STDOUT_FILENO);
  close(tempOUT);
  return 0;
}

int main(int argc, char *argv[]){
  FILE *fd;
  char line[MAX_LINE_SIZE];
  sigset_t sSet;

  //Check SIGINT, SIGQUIT, and SIGALRM are not blocked
  sigemptyset(&sSet);
  sigaddset(&sSet, SIGINT);
  sigaddset(&sSet, SIGQUIT);
  sigaddset(&sSet, SIGALRM);
  sigprocmask(SIG_UNBLOCK, &sSet, NULL);
  
  //ignore 2/SIGINT signals in parent proscess
  void (*Ret)(int);
  Ret = signal(SIGINT, SIG_IGN);
  assert(Ret!=SIG_ERR);
  Ret = signal(SIGQUIT, quitHandler);
  assert(Ret!=SIG_ERR);
  Ret = signal(SIGALRM, alrmhandler);
  assert(Ret!=SIG_ERR);


  //open file and run
  fd = fopen(home, "r");
  if(fd!=NULL){
    while(fgets(line, MAX_LINE_SIZE, fd)!=NULL){
      shell(line, argv, EXE_FILE);  }
    fclose(fd); }
  
  //read from stdin and run
  for(;;){
    printf("%s ", "%"); char* term;
    term=fgets(line, MAX_LINE_SIZE, stdin);
    if(term==NULL){exit(0);}//Ctrl+D : exit the process
    else{
      shell(line, argv, EXE_PROMPT);}
  }
}
