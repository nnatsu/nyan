#include "parse.h"
#include "exec.h"
#include <stdio.h>
#include <string.h>
#include <signal.h>

#define WHITESPACE " \n\t\r"
#define BUFFER_SIZE 256

typedef struct {
  char* command; //single command, doen't includ ";&\n\t". To be parsed
  char** argv;   //parsed parameters to be exec'd
  int foreground;//1 (true) or 0 (false), to be used by exec
} Cground;


int main (int argc, char **argv) {
  int quit = 0; //for main loop
  int length = 0;
  char *input;   //input
  Cground info;                   //to be passed in execute

  struct sigaction action;
  action.sa_sigaction = &signal_handler;
    
    sigset_t blockmask;           //Initialize set for blocking SIGCHLD
    sigemptyset(&blockmask);
    sigaddset(&blockmask, SIGCHLD);
    
    struct termios shell_tmodes;
    int shell_terminal = STDIN_FILENO;       //fd and pid for main shell
    pid_t sid = getpid();
    setpgid(sid, sid);
    tcsetpgrp(shell_terminal, sid);
    tcgetattr(shell_terminal, &shell_tmodes);
    

  while(!quit) {
    printf("u3u ");

    //parse input if it's not empty
    if (fgets(input, BUFFER_SIZE, stdin) == NULL) {
      printf("bad input\n");
    }

    else {
      while (strspn(input, WHITESPACE) != strlen(input)) {
	info.command = chop(input); //chops info off of input
	length = strlen(info.command); //nyaaaan
	input = input + length;
	info.argv = parse(info.command); //parse into argv
	execute(info.command);
      }
    }

  }

}
