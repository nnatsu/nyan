#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <termios.h>
#include "job.h"
#include "parse.h"
#include "exec.h"


#define WHITESPACE " \n\t\r"
#define BUFFER_SIZE 256

sigset_t blockmask;
sem_t mutex;
struct termios shell_tmodes;
int shell_terminal;
pid_t sid; //for main shell

int main (int argc, char **argv) {
    int quit = 0; //for main loop
    int length = 0;
    char *input = (char *)malloc(sizeof(char)*BUFFER_SIZE);   //input
    Cground info;                   //to be passed in execute

    struct sigaction action;
    action.sa_handler = &signal_handler;
    if (sigaction(SIGCHLD, &action, NULL) < 0) {
        perror("sigaction");
        return 1;
    }

    sigemptyset(&blockmask);        //Initialize set for blocking SIGCHLD
    sigaddset(&blockmask, SIGCHLD);

    sem_init(&mutex, 1, 1); ////Initialize MUTEX
    

    shell_terminal = STDIN_FILENO;       //fd and pid for main shell

    sid = getpid();                     //get main shell pid
    setpgid(sid, sid);
    tcsetpgrp(shell_terminal, sid);
    tcgetattr(shell_terminal, &shell_tmodes);
    
    init_job();

    while(!quit) {
        printf("u3u ");

        //parse input if it's not empty
        if (fgets(input, BUFFER_SIZE, stdin) == NULL) {
            printf("bad input\n");
        } else {
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
