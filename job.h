//
//  job.h
//  
//
//  Created by Kangni Hu on 3/19/14.
//
//

#ifndef _job_h
#define _job_h

void init_job();

void add_job(pid_t char**);

void delete_job(pid_t);

Job* find_job(int);

void print_job();

void update_job();

#endif
