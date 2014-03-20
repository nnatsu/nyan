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
        sigset_t blockmask, emptymask;
        sigemptyset(&blockmask);
        sigaddset(&blockmask, SIGCHLD);
        sigprocmask(SIG_BLOCK, &blockmask, NULL)
    }
    
}
