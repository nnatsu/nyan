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
void add_job(pid_t pid, int status, char *command) {

    Job *new = (Job*)malloc(sizeof(Job));   //Create new job
    new->args = (char*)malloc(sizeof(char)*strlen(command));
    strcpy(new->args, command);
    
    new->pid = pid;
    new->jid = ++(head->jid);               //Update jid
    new->status = status;                   //Update status

    new->prev = tail->prev;                 //Add job to list
    new->next = tail;
    tail->prev = new;
    new->prev->next = new;
}

/* Delete a job from list */
void delete_job(int jid) {
    Job *cur = head->next;
    while (cur->jid != jid && cur->args != NULL) {
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
    while (cur->jid != jid && cur->args != NULL) {
        cur = cur->next;
    }
    if (cur == NULL) {
        printf("Job not found.\n");
        return NULL;
        
    } else {
        return cur;
    }
}

Job* find_job_pid(pid_t pid) {
    Job *cur = head->next;
    while (cur->pid != pid && cur->args != NULL) {
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
    while (cur->args != NULL) {
        printf("Job #%d: %s. Status: ", cur->jid, cur->args);
        if (cur->status == RUNNING) printf("Running.\n");
        if (cur->status == SUSPENDED) printf("Suspended.\n");
        cur = cur->next;
    }
}

/* Not sure why need */
void update_job() {
    
}
