#ifndef _exec_h
#define _exec_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <semaphore.h>
#include <unistd.h>
#include <termios.h>
#include "job.h"

typedef struct {
    char* command; //single command, doen't includ ";&\n\t". To be parsed
    char** argv;   //parsed parameters to be exec'd
    int foreground;//1 (true) or 0 (false), to be used by exec
} Cground;

extern sigset_t blockmask;
extern int shell_terminal;
extern Job *head, *tail;
extern int last_job_backgrounded, last_job_suspended;
extern sem_t mutex;
extern pid_t sid;
extern struct termios shell_tmodes;


void execute(Cground);

void signal_handler(int);

void make_fg(int);

void start_bg(int);


#endif
