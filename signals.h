#ifndef _SIGS_H
#define _SIGS_H
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h> 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <stdbool.h>
#include "commands.h"
#include "jobs.h"

void ctrlc_handler(int sig_number);
void ctrlz_handler(int sig_number, Pjob job, char* L_Fg_Cmd);



#endif

