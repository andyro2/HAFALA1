// signals.c
// contains signal handler funtions
// contains the function/s that set the signal handlers
#include "signals.h"
/*******************************************/
/* Name: handler_cntlc
 Synopsis: handle the Control-C */

void handler_cntlc(int sig_number) {
	if (curr_run_pid == -1) return;
	else {
		if (kill(curr_run_pid, SIGINT)) 
			perror("error");
		else 
			printf("Signal SIGINT was sent to pid %d\n", curr_run_pid);
		curr_run_pid = -1;
	}
}
/*******************************************/
/* Name: handler_cntlz
Synopsis: handle the Control-Z */
void handler_cntlz(int sig_number) {
	if (curr_run_pid == -1) return;
	else {
		if (kill(curr_run_pid, SIGTSTP)) 
			perror("error");
		else {
			printf("Signal SIGTSTP was sent to pid %d\n", curr_run_pid);
			if (!create_Job(jobs, curr_run_pid, fg_cmd, true))
				printf("Can't add job\n");
		}
		curr_run_pid = -1;
	}
}