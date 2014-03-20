//
//  Job.c
//  
//
//  Created by Kangni Hu on 3/19/14.
//
//

#include <stdio.h>

typedef struct {
    pid_t pid;
    int jid;
    int status;
    Job *prev;
    Job *next;
    char** args;
}Job;

Job *head, tail;

void init_job() {
    head = malloc(sizeof(Job));
    tail = malloc(sizeof(Job));
    head->next = tail;
    tail->prev = head;
}

void add_job(pid_t char**) {
    
}

void delete_job(pid_t);



Job* find_job(int);

void print_job();

void update_job();