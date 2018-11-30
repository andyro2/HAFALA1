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
			printf("[%d] %s :  %lu secs (Stopped)\n", i, curr_job->name, time(NULL) - curr_job->ini_time);
		else
			printf("[%d] %s :  %lu secs\n", i, curr_job->name, time(NULL) - curr_job->ini_time);
		curr_job = curr_job->next_job;
	}
}

Pjob find_job(int line_num) {
	Pjob curr_job = jobs;
	if (line_num != 0)
	{
		for (int i = 1; i < line_num; i++) {
			if (curr_job->next_job == NULL) // line num larger than num of jobs
			{
				printf("error line num larger than jobs number");
				return NULL;
			}
			curr_job = curr_job->next_job;
		}
	}
	else { //num_args = 0
		while (curr_job->next_job != NULL) {
			curr_job = curr_job->next_job;
		}
	}
	return curr_job;
}

Pjob find_stopped_job(int line_num) {
	Pjob curr_job = jobs;
	Pjob last_stopped_job = jobs;
	if (line_num != 0)
	{
		for (int i = 1; i < line_num; i++) {
			if (curr_job->next_job == NULL) // line num larger than num of jobs
			{
				printf("error line num larger than jobs number");
				return NULL;
			}
			curr_job = curr_job->next_job;
		}
	}
	else { //num_args = 0
		while (curr_job->next_job != NULL) {
			if (curr_job->stopped == true)
				last_stopped_job = curr_job;
			printf("jobs is %s and it stopped - %d\n", curr_job->name, curr_job->stopped);
			curr_job = curr_job->next_job;
		}
		curr_job = last_stopped_job;
	}
	return curr_job;
}

void update_jobs()
{
	Pjob curr_job = jobs, prev_job = jobs;
	int status, pID;
	while (curr_job != NULL)
	{
		pID = waitpid(curr_job->pid, &status, WNOHANG);
		if (((pID == curr_job->pid) && WIFEXITED(status)) || pID == -1)
		{
			if (curr_job == jobs) {
				jobs = curr_job->next_job;
				prev_job = jobs;
				free(curr_job);
				curr_job = jobs;
			}
			else {
				prev_job->next_job = curr_job->next_job;
				free(curr_job);
				curr_job = prev_job->next_job;
			}
		}
		else {
			prev_job = curr_job;
			curr_job = curr_job->next_job;
		}
	}
}

