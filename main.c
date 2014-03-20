#include "parse.h"
#include "exec.h"

#DEFINE 

typedef struct cground {
  char* command; //single command, doen't includ ";&\n\t". To be parsed
  char** argv;   //parsed parameters to be exec'd
  int foreground;//1 (true) or 0 (false), to be used by exec
} cground;


int main (int argc, char argv**) {
  int quit = 0;         //for main loop
  char* input;          //input
  struct cground info;  //to be passed in execute

  //SIGACTION HERE
  struct sigaction action;
 
  action.sa_sigaction = &signal_handler;

  while(!quit) {
    printf("u3u ");

    //parse input if it's not empty
    if (fgets(input, BUFFER_SIZE, stdin) == NULL) {
      printf("bad input\n");
    }
    else {
      while (input != WHITESPACE) {
	info = chop(input); //chops info off of input
	info.argv = parse(info.command); //parse into argv
	execute(info);
      }
    }

  }

}
