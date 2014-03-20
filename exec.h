#ifndef _exec_h
#define _exec_h

typedef struct {
    char* command; //single command, doen't includ ";&\n\t". To be parsed
    char** argv;   //parsed parameters to be exec'd
    int foreground;//1 (true) or 0 (false), to be used by exec
} Cground;


void execute(Cground);

void signal_handler(int);

void make_fg(int);

void start_bg(int);


#endif
