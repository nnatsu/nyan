#include <stdio.h>
#include "exec.h"
#include "job.h"

#define RUNNING 1
#define SUSPENDED 0

void execute(Cground info) {
    char *command = info->command;       //single command to be parsed
    char **argv = info->command;         //args to exec
    int foreground = info->foreground;   //1 T 0 F
    
    
    if (strcmp(argv[0], "fg") == 0) {       //Built-in commands
        make_fg(argv[1]);
    } else if (strcmp(argv[0], "bg") == 0) {
        start_bg(argv[1]);
    } else if (strcmp(argv[0], "kill") == 0) {
        kill(find_job(argv[1])->pid, SIGKILL);
    } else if (strcmp(argv[0], "jobs") == 0 || strcmp(argv[0], "ps") == 0) {
        print_job();
    }
    
    pid_t pid = fork();
    
    if (pid < 0) {                          //Failed fork
        perror("Fork failed\n");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {                  //Child
        setpgid(pid, pid);                  //Set process group ID
        if (execvp(argv[0], argv) == -1) {  //Check execution
            perror("Error: ");
            exit(EXIT_FAILURE);
        } else {
            _exit(EXIT_SUCCESS);
        }
    } else {                                //Parent
        Job* new = (Job*)malloc(sizeof(Job));
        new->pid = pid;                     //Blocks other jobs frommodifying list

        
        //semaphore in MAIN????????
        sem_t mutex;
        sem_init(&mutex, 1, 1); //shared semaphore
        
        
        
        //???????? EXTERN BLOCKMASK VAR???
        //Block SIGCHLD and add new job to list
        sem_wait(&mutex);                           //Lock job list
        sigprocmask(SIG_BLOCK, &blockmask, NULL);   //Block SIGCHLD
        add_job(new->pid, RUNNING, argv);           //Add new job to list
        sem_post(&mutex);                           //Unlock job list
        sigPROCMASK(SIG_UNBLOCK, &blockmask, NULL); //Unblock SIGCHLD
        
        if (foreground == 1) {                      //If job is foreground
            tcsetpgrp(shell_terminal, new->pid);    //Put in fg, auto runs
            
        }

    }
    
}


void make_fg(int jid) {
    Job *cur = head->next;

    while (cur != NULL && jid != -1) {              //Fg specified job
        if (cur->jid == jid) {
            if (cur->status == SUSPENDED) {
                kill(cur->pid, SIGCONT);
                cur->status = RUNNING;
            }
        }
    }
    
    if (jid == -1) {                                //Fg last job backgrounded
        cur = find_job(last_job_backgrounded);
        if (cur->status == SUSPENDED) {
            kill(cur->pid, SIGCONT);
            cur->pid = RUNNING;
        }
    }
    
    if (cur == NULL) {                              //Cannot find job
        printf("Job not found\n");
    } else {
        int status;
        tcsetpgrp(shell_terminal, cur->pid);        //Make job fg
        
        pid_t w = waitpid(pid, &status, WUNTRACED); //Wait for job to finish
        if (w == -1) {
            perror("Waitpid");
            exit(EXIT_FAILURE);
        } else {
            if (WIFSTOPPED(status)) {               //Job suspended
                cur->status = SUSPENDED;
            } else {                                //Job exited/killed
                sem_wait(&mutex);                   //Lock job list
                sigprocmask(SIG_BLOCK, &blockmask, NULL);
                delete_job(cur->jid);               //Block SIGCHLD & delete
                sem_post(&mutex);                   //Unlock job list
                sigprocmask(SIG_UNBLOCK, &blockmask, NULL);
            }
        }
        
    }
    
    tcsetpgrp(shell_terminal, sid);
    tcsetattr(shell_terminal, TCSADRAIN, &shell_tmodes);
}

void start_bg(int jid) {
    Job *cur = head->next;
    while (cur != NULL && jid != -1) {              //Cont specified job
        if (cur->jid == jid && cur->status == SUSPENDED) {
            kill(cur->pid, SIGCONT);
        }
    }
    
    if (jid == -1) {                                //Cont last suspended job
        cur = find_job(last_job_suspended);
        if (cur->status == SUSPENDED) {
            kill(cur->pid, SIGCONT);
        } else {
            printf("Last suspended job is already running\n");
        }
    }
    
    if (cur == NULL) {                              //Cannot find job
        printf("Job not found\n");
    }
}

void signal_handler(int signum) {
    
}