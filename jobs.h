#include <unistd.h> 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include "signals.h"
#include "commands.h"

#ifndef _Job_H_
#define _Job_H_

#define MAX_NAME_LENGTH  50


typedef struct _JOBS {
	
	int pid;
	int ini_time;
	char name[MAX_NAME_LENGTH];
	bool stopped;
	struct _JOBS* next_job;
} job,* Pjob;

bool create_Job(int pid, char* name, bool); // creates new job and inserts at the and. returns success or fail
Pjob remove_job( int line_num); // removes from list and returnes job according to line num. if line num = 0 returns last
Pjob find_job(int line_num);
void free_jobs();
int kill_jobs();
void update_jobs();

#endif