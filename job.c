#include "job.h"

#define RUNNING 1
#define SUSPENDED 0

Job *head, *tail;

int last_job_backgrounded, last_job_suspended;

/* Initializes the Job list */
void init_job() {
    head = (Job*)malloc(sizeof(Job));
    tail = (Job*)malloc(sizeof(Job));
    head->next = tail;
    tail->prev = head;
    head->jid = 0;
}

/* Add a new job to list */
void add_job(pid_t pid, int status, char ** args) {
    Job *new = (Job*)malloc(sizeof(Job));   //Create new job

    int len = sizeof(args) / sizeof(args[0]);
    new->args = (char**)malloc(sizeof(char*)*len);

    new->pid = pid;
    new->jid = ++(head->jid);               //Update jid
    new->status = status;                   //Update status
    //new->args = args;                       //This ?????????
    memcpy(new->args, args, sizeof(**args));

    new->prev = tail->prev;                 //Add job to list
    new->next = tail;
    tail->prev = new;
}

/* Delete a job from list */
void delete_job(int jid) {
    Job *cur = head->next;
    while (cur->jid != jid && cur != NULL) {
        cur = cur->next;
    }
    if (cur == NULL) {
        printf("Job not found.\n");
        
    } else {
        cur->prev->next = cur->next;
        cur->next->prev = cur->prev;
        free(cur);
    }
}

/* Find a job from list */
Job* find_job(int jid) {
    Job *cur = head->next;
    while (cur->jid != jid && cur != NULL) {
        cur = cur->next;
    }
    if (cur == NULL) {
        printf("Job not found.\n");
        return NULL;
        
    } else {
        return cur;
    }
}

/* Print all jobs */
void print_job() {
    Job *cur = head->next;
    while (cur != NULL) {
        printf("Job #%d: ", cur->jid);
        print_arg(cur->args);
        printf(". Status: ");
        if (cur->status == RUNNING) printf("Running.\n");
        if (cur->status == SUSPENDED) printf("Suspended.\n");
        cur = cur->next;
    }
}

void print_arg(char **args) {
    int len = sizeof(args) / sizeof(args[0]);
    int i;
    for (i = 0; i < len; i++) {
        printf("%s ", args[i]);
    }
}

/* Not sure why need */
void update_job() {
    
}
