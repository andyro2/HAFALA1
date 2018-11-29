#include "jobs.h"
//**************************************************************************************
// function name: ExeExternal
// Description: executes external command
// Parameters: external command arguments, external command string
// Returns: void
//**************************************************************************************
bool create_Job(int pid, char* name, bool stopped)
{
	Pjob job;
	job = (Pjob)malloc(sizeof(job));
	if (job == NULL)
	{
		free(job);
		return false;
	}
	job->pid = pid;
	strcpy(job->name, name);
	job->ini_time = time(NULL);
	job->stopped = stopped;
	job->next_job = NULL;
	Pjob curr_job = jobs, prev_job;
	if (jobs == NULL)
		jobs = job;
	else
	{
		while (curr_job != NULL) {
			prev_job = curr_job;
			curr_job = curr_job->next_job;

		}
		prev_job->next_job = job;
	}
	return true;
}

Pjob remove_job(int line_num) {
	Pjob curr_job, prev_job;
	if (line_num != 0)
	{
		curr_job = jobs;
		for (int i = 1; i < line_num; i++) {
			if (curr_job->next_job == NULL) // line num larger than num of jobs
				return NULL;
			prev_job = curr_job;
			curr_job = curr_job->next_job;
		}
	}
	else { //num_args = 0
		while (curr_job->next_job != NULL) {
			prev_job = curr_job;
			curr_job = curr_job->next_job;
		}
	}
	prev_job->next_job = curr_job->next_job;
	curr_job->next_job = NULL;
	return curr_job;
}

void free_jobs() {
	Pjob curr_job = jobs, tmp = NULL;

	while (curr_job != NULL)
	{
		tmp = curr_job->next_job;
		free(curr_job);
		curr_job = tmp;
	}
	return;
}

int kill_jobs() {
	Pjob curr_job = jobs;
	for (int i = 1; curr_job != NULL; i++) {
		printf("[%d] %s - Sending SIGTERM... ", i, curr_job->name);

		if (kill(curr_job->pid, SIGTERM)) {
			printf("Error! can't send SIGTERM");
			return -1;
		}
		//printf("Signal %d was sent to pid %d ", SIGTERM, arr[i].pid);
		long int start_t, curr_t, res;
		time(&start_t);
		while (time(&curr_t) - start_t < 5) {

			res = waitpid(curr_job->pid,NULL, WNOHANG);
			if (res > 0) { //finished
				printf("DONE.\n");
			}
			else if (res == -1) {
				printf("Error! SIGTERM failed \n");
				return -1;
			}
		}
		printf("(5 sec passed) Sending SIGKILL...");

		res = kill(curr_job->pid, SIGKILL);

		if (res != 0) {
			printf("Error! SIGKILL failed \n");
			return -1;
		}
		else
			printf("DONE.\n");
	}
	return 0;
}

void PrintJobs()
{
	Pjob curr_job = jobs;
	for (int i = 1; curr_job != NULL; i++)
	{
		if (curr_job->stopped)
			printf("[%d] %s :  %lu secs (Stopped)\n", curr_job->pid, curr_job->name, time(NULL) - curr_job->ini_time);
		else
			printf("[%d] %s :  %lu secs\n", curr_job->pid, curr_job->name, time(NULL) - curr_job->ini_time);
		curr_job = curr_job->next_job;
	}
}

Pjob find_job(int line_num) {
	Pjob curr_job, prev_job;
	if (line_num != 0)
	{
		curr_job = jobs;
		for (int i = 1; i < line_num; i++) {
			if (curr_job->next_job == NULL) // line num larger than num of jobs
				return NULL;
			prev_job = curr_job;
			curr_job = curr_job->next_job;
			prev_job->next_job = curr_job;
		}
	}
	else { //num_args = 0
		while (curr_job->next_job != NULL) {
			prev_job = curr_job;
			curr_job = curr_job->next_job;
			prev_job->next_job = curr_job;
		}
	}
	return curr_job;
}

void update_jobs()
{
	Pjob curr_job = jobs, prev_job;
	int status, pID;	
	while (curr_job != NULL)
	{
		prev_job = curr_job;	
		pID = waitpid(curr_job->pid, &status, WNOHANG);
		if (((pID == curr_job->pid) && WIFEXITED(status)) || pID == -1)
		{
			printf("8\n");
			prev_job->next_job = curr_job->next_job;
			free(curr_job);
		}
		curr_job = prev_job->next_job;
		printf("9\n");
	}
}

