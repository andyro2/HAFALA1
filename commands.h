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




History* history = (History*)malloc(sizeof(History));
history->prev_exists = FALSE;
history->count = 0;
history->index = 0;
history->bg = FALSE;



typedef struct _History {

	bool prev_exists;
	int count;
	int index;
	bool bg;
	char memory[HIST_MAX][MAX_LINE_SIZE];
	struct _History* next_history;
} history, *History;


typedef enum { FALSE , TRUE } _bool;
int ExeComp(char* lineSize);
int BgCmd(char* lineSize, Pjob jobs);
int ExeCmd(Pjob jobs, char* lineSize, char* cmdString, , char* prev_folder, History* history);
void ExeExternal(char *args[MAX_ARG], char* cmdString, int num_arg);
#endif

