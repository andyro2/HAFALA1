//		commands.c
//********************************************
#include "commands.h"
//********************************************
// function name: ExeCmd
// Description: interperts and executes built-in commands
// Parameters: pointer to jobs, command string
// Returns: 0 - success,1 - failure
//**************************************************************************************
int ExeCmd(char* lineSize, char* cmdString, char* prev_folder, Phistory history)
{
	char* cmd;
	char* args[MAX_ARG];
	char pwd[MAX_LINE_SIZE];
	char* delimiters = " \t\n";
	int i = 0, num_arg = 0;
	bool illegal_cmd = false; // illegal command
	cmd = strtok(cmdString, delimiters);
	if (cmd == NULL)
		return 0;
	args[0] = cmd;
	for (i = 1; i<MAX_ARG; i++)
	{
		args[i] = strtok(NULL, delimiters);
		if (args[i] != NULL)
			num_arg++;
	}


	/*************************************************/
	// Built in Commands PLEASE NOTE NOT ALL REQUIRED
	// ARE IN THIS CHAIN OF IF COMMANDS. PLEASE ADD
	// MORE IF STATEMENTS AS REQUIRED
	/*************************************************/
	if (!strcmp(cmd, "cd"))
	{
		if (num_arg != 1) {
			illegal_cmd = true;
		}
		else
		{
			// check if there weren't a cd command before 
			bool prev_request = false;
			if (!strcmp(args[1], "-"))
				prev_request = true;

			if (prev_request && prev_folder[0] == 0)
				return 0;
			else {
				char* go_dir = (prev_request) ? prev_folder : args[1];
				if (getcwd(pwd, MAX_LINE_SIZE + 1) == NULL || chdir(go_dir) != 0) {
					printf("smash error: > \"%s\" - path not found\n", go_dir);
					return 1;
				}
				else {
					if (prev_request) printf("%s\n", prev_folder);
					strcpy(prev_folder, pwd);
				}
			}

		}
	}

	/*************************************************/
	else if (!strcmp(cmd, "pwd"))
	{
		if (num_arg != 0)
			illegal_cmd = true;
		else if (getcwd(pwd, MAX_LINE_SIZE + 1) == NULL)
		{
			perror("error");
			return 1;
		}
		else
			printf("%s\n", pwd);

	}

	/*************************************************/
	else if (!strcmp(cmd, "history"))
	{
		if (num_arg == 0)
		{

			Phistory hist = history;
			int idx = (hist->index - hist->count) % HIST_MAX;
			if (idx < 0)
				idx += HIST_MAX;
			for (int i = 0; i < hist->count - 1; ++i)
			{
				printf("%s\n", hist->memory[idx]);
				idx = (idx + 1) % HIST_MAX;
			}
		}
		else
		{
			illegal_cmd = true;
		}
	}
	/*************************************************/
	else if (!strcmp(cmd, "kill"))
	{
		if (num_arg != 2) 
		{
			illegal_cmd = true;
		}
		else {
			int signal_number = atoi(strtok(args[1], "-"));
			int job_number = atoi(args[2]);
			if (!signal_number || !job_number)
			{
				illegal_cmd = true;
			}

			else {
				Pjob curr_job = jobs;
				for (int i = 0; i < job_number - 1; i++)
				{
					if (!curr_job->next_job)
					{
						printf("%s %d - job does not exist\n", cmdString, job_number);
						return -1;
					}
					curr_job = curr_job->next_job;

				}
				if (!curr_job) {
					printf("%s %d - job does not exist\n", cmdString, job_number);
					return -1;
				}
				else {
					if (kill(curr_job->pid, signal_number)) {
						printf("%s %d - cannot send signal\n", cmdString, job_number);
						return 0;
					}
					printf("Signal %d was sent to pid %d\n", signal_number, curr_job->pid);
					if (signal_number == SIGSTOP || signal_number == SIGTSTP) //SIGSTOP signals
					{
						curr_job->stopped = true;
					}
					else if (signal_number == SIGCONT)//SIGCONT signal
					{
						curr_job->stopped = false;
					}
				}
			}
		}
	}
	else if (!strcmp(cmd, "jobs"))
	{
		if (num_arg != 0) {
			illegal_cmd = true;
		}
		else
			PrintJobs();
	}
	/*************************************************/
	else if (!strcmp(cmd, "showpid"))
	{
		if (num_arg != 0)
			illegal_cmd = true;
		else
			printf("smash pid is %d\n", getpid());
	}
	/*************************************************/
	else if (!strcmp(cmd, "fg"))
	{
		Pjob curr_job;
		int job_num;
		if (num_arg > 1)
			illegal_cmd = true;
		else {
			if (num_arg == 1)
				job_num = atoi(args[1]);
			else
				job_num = 0;

			curr_job = find_job(job_num);
			if (curr_job == NULL)
				illegal_cmd = true;
			else
			{
				if (curr_job->stopped) {
					kill(curr_job->pid, SIGCONT);
					curr_job->stopped = false;
					printf("Signal SIGCONT was sent to pid %d\n", curr_job->pid);
				}
				// *********************************************************

				printf("%s\n", curr_job->name);
				strcpy(L_Fg_Cmd, curr_job->name);
				waitpid(curr_job->pid, NULL, 0);
				//wait till child finished
				curr_run_pid = -1;
				//***************************************************************
			}
		}

	}
	/*************************************************/
	else if (!strcmp(cmd, "bg"))
	{
		Pjob curr_job;
		int job_num;
		if (num_arg > 1)
			illegal_cmd = true;
		else
		{
			if (num_arg == 1)
				job_num = atoi(args[1]);
			else
				job_num = 0;

			curr_job = find_stopped_job(job_num);

			if (curr_job == NULL) 
				illegal_cmd = true;
			else if (!curr_job->stopped) {
				if (job_num != 0)
					printf("smash error: > job %d is already running on background\n", job_num);
				else
					printf("smash error: > last job is already running on background\n");
			}
			else
			{
				kill(curr_job->pid, SIGCONT);
				printf("Signal SIGCONT was sent to pid %d\n", curr_job->pid);
				curr_run_pid = -1;
				curr_job->stopped = false;
			}
		}
	}
	/*************************************************/
	else if (!strcmp(cmd, "quit"))
	{
		if (num_arg > 1)
			illegal_cmd = true;

		else {
			if (num_arg == 0) {
				free_jobs();
				exit(0);
			}
			else if (!strcmp(args[1], "kill")) {
				kill_jobs();
				free_jobs();
				exit(0);
			}
			else
				illegal_cmd = true;
		}

	}
	else if (!strcmp(cmd, "mv"))
	{
		if (num_arg != 2)
			illegal_cmd = true;
		else
		{
			if (rename(args[1], args[2]))
			{
				perror("error");
				return 1;
			}
			else
				printf("%s has been renamed to %s\n", args[1], args[2]);

		}
	}
	/*************************************************/
	else // external command
	{
		ExeExternal(args, cmdString, num_arg);
		return 0;
	}
	if (illegal_cmd == true)
	{
		printf("smash error: > \"%s\"\n", cmdString);
		return 1;
	}
	return 0;
}
//**************************************************************************************
// function name: ExeExternal
// Description: executes external command
// Parameters: external command arguments, external command string
// Returns: void
//**************************************************************************************
void ExeExternal(char *args[MAX_ARG], char* cmdString, int num_arg)
{
	int pID;
	switch (pID = fork())
	{
	case -1:
		perror("smash error: > ");
		exit(1);
	case 0:
	{
		// Child Process
		setpgrp();
		curr_run_pid = (int)getpid();
		if (!strcmp(args[num_arg], "&"))
			args[num_arg] = '\0';
		execvp(cmdString, args);
		perror("error: > ");
		printf("Execution of %d pid failed\n", curr_run_pid);
		curr_run_pid = -1;
		exit(1);
	}

	default:
		// parent process 
		if (!strcmp(args[num_arg], "&")) // for bg command
		{
			if (!create_Job(pID, args[0], false))
				printf("Can't add job\n");
		}
		else {
			curr_run_pid = pID;
			strcpy(L_Fg_Cmd, args[0]);
			waitpid(pID, NULL, 0);

			curr_run_pid = -1;

		}
	}
}
//**************************************************************************************
// function name: ExeComp
// Description: executes complicated command
// Parameters: command string
// Returns: 0- if complicated -1- if not
//**************************************************************************************
int ExeComp(char* lineSize)
{
	//char ExtCmd[MAX_LINE_SIZE + 2];
	char *args[MAX_ARG];
	if ((strstr(lineSize, "|")) || (strstr(lineSize, "<")) || (strstr(lineSize, ">")) || (strstr(lineSize, "*")) || (strstr(lineSize, "?")) || (strstr(lineSize, ">>")) || (strstr(lineSize, "|&")))
	{
		args[0] = (char*)"/bin/csh";
		args[1] = (char*)"-f";
		args[2] = (char*)"-c";
		args[3] = lineSize;
		args[4] = NULL;
		for (int k = 4; k < MAX_ARG; ++k)      args[k] = '\0';
		ExeExternal(args, "/bin/bash", 0);
		return 0;
	}
	return -1;
}

//**************************************************************************************
// function name: BgCmd
// Description: if command is in background, insert the command to jobs
// Parameters: command string, pointer to jobs
// Returns: 0- BG command -1- if not
//**************************************************************************************
int BgCmd(char* lineSize)
{
	char *args[MAX_ARG];
	char* delimiters = " \t\n";
	int i = 0, num_arg = 0;
	char* cmd = strtok(lineSize, delimiters);
	if (cmd == NULL)
		return 0;
	args[0] = cmd;
	for (i = 1; i<MAX_ARG; i++)
	{
		args[i] = strtok(NULL, delimiters);
		if (args[i] != NULL)
			num_arg++;

	}

	if (*args[num_arg] == '&')
	{
		args[num_arg] = NULL;
		num_arg -= 1;
		int pID;

		switch (pID = fork()) {
		case -1:
			perror("perror: ");
			exit(1);
			return -1;
		case 0:
			setpgrp();
			if (execvp(cmd, args) == -1)
			{
				perror("Execvp error");
				exit(1);
			}
			return -1;
		default:
			
			if (create_Job(pID, cmd, false) == NULL)
				printf("Job create failed!");
					
			return 0;

		}
	}
	else return -1;
}

void history_save(Phistory history, char* cmd)
{
	if (!strcmp(cmd, "")) return;
	int ind = history->index;
	strcpy(history->memory[ind], cmd);
	if (history->count <HIST_MAX)     ++history->count;
	history->index = ind + 1;

}