#include <stdio.h>
#include "exec.h"
#include "job.h"

void execute(Cground) {
    char *command = Cground->command;       //single command to be parsed
    char **argv = Cground->command;         //args to exec
    int foreground = Cground->foreground;   //1 T 0 F
    
    
    if (strcmp(argv[0], "fg") == 0) {       //Built-in commands
        make_fg(argv[1]);
    } else if (strcmp(argv[0], "bg") == 0) {
        make_bg(argv[1]);
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

        sigprocmask(SIG_BLOCK, &blockmask, NULL)
    }
    
}









void jobs_lock() {
    sigprocmask(SIG_BLOCK, &chld_mask, NULL);
}

void jobs_unlock() {
    sigprocmask(SIG_UNBLOCK, &chld_mask, NULL);
}


extern int mysh_terminal;
extern pid_t mysh_pgid;
extern struct termios mysh_tmodes;
extern sigset_t chld_mask;
int save_flag;

int exec_sh(char**, int, char*);
int exec_bcmd(char**, int);
void b_exit();
int b_history();

void put_in_foreground(pid_t);
void signal_wrapper(int, void*);
void sigchld_handler(int);
void sigtstp_handler(int);
void jobs_lock();
void jobs_unlock();

#endif	/* COMMAND_H */





int exec_sh(char** args, int n_args, char* origin) {
    pid_t pid;
    
    Job* new_job = job_create((origin[0] == '&') ? JOB_BACK: JOB_FORE, origin, -1)
    //if an empty argument, free the new_job node and return
    else {
        /***** In Parent *****/
        new_job->pid = pid;
        jobs_lock();
        jobs_add(new_job);
        jobs_unlock();
        
        if(new_job->status == JOB_FORE) {
            put_in_foreground(pid);
        } else if(new_job->status == JOB_BACK) {
            printf("[%d]\t%d\n", new_job->id, pid);  /* if background, print job info,  handle SITCHLD signal */
        }
    }
}
return 0;
}