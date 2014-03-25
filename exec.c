#include "exec.h"

#define RUNNING 1
#define SUSPENDED 0
#define TERMINATED 2

void execute(Cground info) {

    char *command = info.command;       //single command to be parsed
    char **argv = info.argv;         //args to exec
    int foreground = info.foreground;   //1 T 0 F
    int len = info.len; //length of argument
    int i=0;

    if (strcmp(argv[0], "fg") == 0) {       //Built-in commands
      if (len == 1) i = -1;
      else i = atoi(argv[1]);
      make_fg(i);

    } else if (strcmp(argv[0], "bg") == 0) {
      if (len == 1) i = 0;
      else i = atoi(argv[1]);
      start_bg(i);
        
    } else if (strcmp(argv[0], "kill") == 0) {
      kill(find_job(atoi(argv[1]))->pid, SIGKILL);
        
    } else if (strcmp(argv[0], "jobs") == 0 || strcmp(argv[0], "ps") == 0) {
      print_job();
    } else if (strcmp(argv[0], "quit") == 0 || strcmp(argv[0], "exit") == 0) {
      clean_up_jobs();
      exit(0);
    } else {
    
    /* Everything in else */
    pid_t pid = fork();

    if (pid < 0) {                          //Failed fork
        perror("Fork failed\n");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {                  //Child
        setpgid(pid, pid);                  //Set process group ID
        if (execvp(argv[0], argv) == -1) {  //Check execution
            perror("Error");
            exit(EXIT_FAILURE);
        } else {
            _exit(EXIT_SUCCESS);
        }
    } else {                                //Parent
        sem_wait(&mutex);                           //Lock job list
        sigprocmask(SIG_BLOCK, &blockmask, NULL);   //Block SIGCHLD
        add_job(pid, RUNNING, command, foreground); //Add new job to list
        sem_post(&mutex);                           //Unlock job list
        sigprocmask(SIG_UNBLOCK, &blockmask, NULL); //Unblock SIGCHLD
        if (foreground != 1) {                      //If job is foreground
            printf("cur job is in fg\n");
            make_fg(-1);			    //Fg last job added to bg
        }
    }

   }
}


void make_fg(int jid) {

    Job *cur = head->next;

    while (cur != NULL && jid != -1 && cur->jid != jid) {  //Fg specified job
        cur = cur->next;
    }

    if (cur != NULL && cur->jid == jid) {
        if (cur->status == SUSPENDED) {
            kill(cur->pid, SIGCONT);
            cur->status = RUNNING;
        }
    }
    
    if (jid == -1) {                                //Or fg last job backgrounded
        cur = find_job(lastbg->prevbg->jid);
printf("now jid %d", cur->jid);
        if (cur->status == SUSPENDED) {
            kill(cur->pid, SIGCONT);
            cur->pid = RUNNING;
        }
    }
    
    if (cur == NULL && !check_empty()) {       //If cannot find job & list not empty
        printf("Job not found\n");
    } else if (check_empty()) {
        printf("Currently has no job\n");
    } else {
        int status;
        //sigprocmask(SIG_BLOCK, &blockmask_rest, NULL);
        tcsetpgrp(shell_terminal, cur->pid);        //Make job fg
        
printf("wait in fg");
        pid_t w = waitpid(-1, &status, WUNTRACED); //Wait for job to finish
        if (w == -1) {
            perror("Waitpid");
            exit(EXIT_FAILURE);
        } else {
            if (WIFSTOPPED(status)) {               //Job suspended
                cur->status = SUSPENDED;

                cur->nextbg->prevbg = cur->prevbg;    //Unlink from list
                cur->prevbg->nextbg = cur->nextbg;

                cur->prevbg = lastbg->prevbg;         //Relink at the end
                cur->nextbg = lastbg;
                cur->prevbg->nextbg = cur;
                lastbg->prevbg = cur;
            } else {                                //Job exited/killed
                sem_wait(&mutex);                   //Lock job list
                sigprocmask(SIG_BLOCK, &blockmask, NULL);

                //delete_job(cur->jid);               //Block SIGCHLD & set to term
                cur->status = TERMINATED;
                cur->term_flag = 1;
                cur->prevbg->nextbg = cur->nextbg;    //Unlink from list
                cur->nextbg->prevbg = cur->prevbg;
                cur->prevbg = NULL; cur->nextbg = NULL;

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
        cur = find_job(lastbg->prevbg->jid);
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

void child_handler(int signum) {
    int status;
    pid_t w;
    while((w = waitpid(-1, &status, WNOHANG)) > 0) {
        Job* temp = find_job_pid(w);
        delete_job(temp->jid);
         
    }    
}

void stop_handler(int signum) {
  
}

void int_handler(int signum) {

}
