#include "job.h"

#define RUNNING 1
#define SUSPENDED 0
#define TERMINATED 2

Job *head, *tail, *lastbg, *firstbg;

int last_job_backgrounded, last_job_suspended;

/* Initializes the Job list */
void init_job() {
    head = (Job*)malloc(sizeof(Job));
    tail = (Job*)malloc(sizeof(Job));
    lastbg = (Job*)malloc(sizeof(Job));
    firstbg = (Job*)malloc(sizeof(Job));

    head->next = tail;
    tail->prev = head;

    lastbg->prevbg = firstbg; 
    firstbg->nextbg = lastbg;
    head->jid = 0;
}

/* Add a new job to list */
void add_job(pid_t pid, int status, char *command, int fg) {

    Job *new = (Job*)malloc(sizeof(Job));   //Create new job
    new->args = (char*)malloc(sizeof(char)*strlen(command));
    strcpy(new->args, command);
    
    new->pid = pid;
    (head->jid)++;
    new->jid = head->jid;               //Update jid
    new->status = status;                   //Update status
    new->fg = fg;

    last_job_backgrounded = new->jid;
    new->prevbg = lastbg->prevbg;	//Update lastbg'ed job
    new->nextbg = lastbg;
    lastbg->prevbg = new;
    new->prevbg->nextbg = new;
    

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
	
	printf("[%d]", cur->jid);

        if (cur->jid == lastbg->prevbg->jid) printf("+  ");
        else if (lastbg->prevbg->prevbg != NULL &&
                 cur->jid == lastbg->prevbg->prevbg->jid) printf("-  ");
        else printf("   ");

        if (cur->status == RUNNING) printf("Running.");
        if (cur->status == SUSPENDED) printf("Stopped."); 
        if (cur->status == TERMINATED &&
            cur->term_flag == 1) {
            printf("Terminated.");
            cur->term_flag == 0;
        }
        printf("         %s\n", cur->args);

        cur = cur->next;
    }
}

/* Clean up jobs after shell quits */
void clean_up_jobs() {
    Job *cur = head->next;
    free(head);
    while(cur->args != NULL) {
        free(cur);
        cur = cur->next;
    }
    free(tail);
}

/* Not sure why need */
void update_job() {
    
}
