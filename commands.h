#ifndef _COMMANDS_H
#define _COMMANDS_H
#include <unistd.h> 
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "jobs.h"

#define MAX_LINE_SIZE 80
#define MAX_ARG 20
#define HIST_MAX 51

extern int curr_run_pid;






typedef struct _History {

	bool prev_exists;
	int count;
	int index;
	bool bg;
	char memory[HIST_MAX][MAX_LINE_SIZE];
	struct _History* next_history;
} History, *Phistory;

int ExeComp(char* lineSize);
int BgCmd(char* lineSize, Pjob jobs);
int ExeCmd(Pjob jobs, char* lineSize, char* cmdString, char* prev_folder, Phistory history, char* L_Fg_Cmd);
void ExeExternal(char *args[MAX_ARG], char* cmdString, int num_arg);
void history_save(Phistory history, char* cmd);
#endif

