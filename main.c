#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <termios.h>
#include "job.h"
#include "parse.h"
#include "exec.h"


#define WHITESPACE " \n\t\r"
#define BUFFER_SIZE 256


int main (int argc, char **argv) {
  int quit = 0; //for main loop
  int length = 0;
  char *input;   //input
  Cground info;                   //to be passed in execute

  struct sigaction action;
  action.sa_handler = &signal_handler;
  if (sigaction(SIGCHLD, &action, NULL) < 0) {
    perror("sigaction");
    return 1;
  }

    
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
	execute(info);
      }
    }

  }

  return 0;

}
