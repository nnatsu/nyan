#ifndef _job_h
#define _job_h

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

typedef struct Job Job;

struct Job {
    pid_t pid;
    int jid;
    int status;
    Job *prev;
    Job *next;
    char **args;
};

void init_job();

void add_job(pid_t, int, char **);

void delete_job(int);

Job* find_job(int);

void print_job();

void print_arg(char **);

void update_job();

#endif
