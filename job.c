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
    char **args;
}Job;

Job *head, tail;

void init_job() {
    head = malloc(sizeof(Job));
    tail = malloc(sizeof(Job));
    head->next = tail;
    tail->prev = head;
    tail->jid = 0;
}

void add_job(pid_t pid, int status, char ** args) {
    Job *new = malloc(sizeof(Job));
    new->pid = pid;
    new->jib = ++(tail->jid);
    new->status = status;
    new->args = args;
    new->prev = tail->prev;
    new->next = prev;
    prev->prev = new;
}

void delete_job(pid_t pid) {
    Job *cur = head;
    do {
        cur = cur->next;
    } while (cur->pid != pid && cur != NULL);
    cur->prev->next = cur->next;
    cur->next->prev = cur->prev;
    free(cur);
}

Job* find_job(int) {
    
}

void print_job();

void update_job();