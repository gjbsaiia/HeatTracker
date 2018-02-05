//Griffin Saiia, Gjs64
//shell for Dumb Thermometer

//OPEN ROBO_SERVER IN SEPERATE TERMINAL WINDOW BEFORE RUNNING ROBO_SHELL

//commands: 'sample' - calls "simulated_polling", starts stream of fake data, robo_server must be open
//					'forward' - prints forward because we can't actually do anything here, but delay is real
//					'left' - prints turing left because we can't actually do anything here, but delay is real
//					'right' - prints turning right because we can't actually do anything here, but delay is real
//					'reverse' - prints reversing because we can't actually do anything here, but delay is real
//					'stop' - prints stopped because we can't actually do anything here, but delay is real

#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/socket.h>
#include <netinet/in.h>
#define MAX_LINE 80 /* 80 chars per line, per command */

int main(void) {
	char line[50];
	char *args[MAX_LINE/2 + 1];	/* command line (of 80) has max of 40 arguments */
	//controls 'Invalid Command' output
	int prompt = 0;
	int should_run = 1;
	pid_t pid;

	printf("DumbThermometer> ");
	while(should_run){
		scanf("%s",line);
		args[0] = strtok(line, " ");
		int i = 0;
		while (args[i] != NULL){
			args[++i] = strtok(NULL, " ");
		}
		i = 0;
		if (!strcmp(args[0], "exit")){
			should_run = 0;;
		}
		else{
			if( !strcmp(args[0], "sample")){
				pid = fork();
				if( pid == 0 ){
					//starts stream of fake data
					prompt = 1;
					printf("Polling...\n");
					args[0] = "./simulated_polling";
					args[1] = 0;
					execvp(args[0], args);
				}
				else{
					sleep(1);
					printf("DumbThermometer> ");
					prompt = 0;
				}
			}
			//these commands don't do anything so they don't need a seperate process.
			else{
				if(!strcmp(args[0], "forward")){
					fprintf(stdout, "Forward...\n");
					fflush(stdout);
					prompt = 1;
				}
				if(!strcmp(args[0], "left")){
					fprintf(stdout, "Turning Left...\n");
					fflush(stdout);
					prompt = 1;
				}
				if(!strcmp(args[0], "right")){
					fprintf(stdout, "Turning Right...\n");
					fflush(stdout);
					prompt = 1;
				}
				if(!strcmp(args[0], "reverse")){
					fprintf(stdout, "Reversing...\n");
					fflush(stdout);
					prompt = 1;
				}
				if(!strcmp(args[0], "stop")){
					fprintf(stdout, "Stopping...\n");
					fflush(stdout);
					prompt = 1;
				}
				sleep(1);
				if(prompt == 0){
					fprintf(stdout, "Error Invalid Command.\n");
					fflush(stdout);
				}
				printf("DumbThermometer> ");
				prompt = 0;
			}
		}
	}
}
