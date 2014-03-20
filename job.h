#ifndef _job_h
#define _job_h

void init_job();

void add_job(pid_t, int, char **);

void delete_job(int);

Job* find_job(int);

void print_job();

void update_job();

#endif
