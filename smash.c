/*	smash.c
main file. This file contains the main function of smash
*******************************************************************/
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h> 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <stdbool.h>
#include "commands.h"
#include "signals.h"
#include "jobs.h"

#define MAX_LINE_SIZE 80
#define MAXARGS 20

//char* L_Fg_Cmd;
//Pjob jobs = NULL;
char lineSize[MAX_LINE_SIZE]; 
char prev_folder[MAX_LINE_SIZE + 1] = { 0 };
//**************************************************************************************
// function name: main
// Description: main function of smash. get command from user and calls command functions
//**************************************************************************************
int main(int argc, char *argv[])
{
	char cmdString[MAX_LINE_SIZE];
	Phistory history = (Phistory)malloc(sizeof(History));
	history->prev_exists = false;
	history->count = 0;
	history->index = 0;
	history->bg = false;

	
	//signal declaretions
	struct sigaction ctrlC, ctrlZ;
	ctrlC.sa_handler = &ctrlc_handler;
	ctrlZ.sa_handler = &ctrlz_handler;
	
	/************************************/
	//NOTE: the signal handlers and the function/s that sets the handler should be found in siganls.c
	sigaction(SIGINT, &ctrlC, NULL);
	sigaction(SIGTSTP, &ctrlZ, NULL);

	/************************************/

	/************************************/
	// Init globals 

	curr_run_pid = -1;
	jobs = NULL;

	L_Fg_Cmd =(char*)malloc(sizeof(char)*(MAX_LINE_SIZE+1));
	if (L_Fg_Cmd == NULL) 
			exit (-1); 
	L_Fg_Cmd[0] = '\0';
	
    	while (1)
    	{
	 	printf("smash > ");
		fgets(lineSize, MAX_LINE_SIZE, stdin);
		//printf("anddddddy\n");
		strcpy(cmdString, lineSize); 
		//printf("1\n");
		cmdString[strlen(lineSize)-1]='\0';
		//printf("2\n");
		// Save History here!
	    history_save(history, cmdString);
		//printf("3\n");
		update_jobs();
		//printf("4\n");
					// perform a complicated Command
		if(!ExeComp(lineSize)) continue; 
		//printf("11\n");
					// background command	
	 	if(!BgCmd(lineSize)) continue; 
		//printf("22\n");
					// built in commands
		ExeCmd(lineSize, cmdString, prev_folder,history);
		//printf("33\n");
		
		/* initialize for next line read*/
		lineSize[0]='\0';
		cmdString[0]='\0';
		printf("44\n");
	}
		free_jobs();
		free(L_Fg_Cmd);
		free(history);

    return 0;
}

