#ifndef _job_h
#define _job_h

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

typedef struct {
    pid_t pid;
    int jid;
    int status;
    struct Job *prev;
    struct Job *next;
    char **args;
} Job;


void init_job();

void add_job(pid_t, int, char **);

void delete_job(int);

Job* find_job(int);

void print_job();

void update_job();

#endif
