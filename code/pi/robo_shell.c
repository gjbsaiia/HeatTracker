//Griffin Saiia, Gjs64
//shell for Dumb Thermometer
//commands: 'sample' - calls "robo_polling", starts stream of temperature data, robo_server must be open
//					'forward' - calls "drive.py", puts arduino in drive state
//					'left' - calls "left.py", puts arduino in left turn state
//					'right' - calls "right.py", puts arduino in right turn state
//					'reverse' - calls "reverse.py", puts arduino in reverse state
//					'stop' - calls "stop.py", interrupts arduino state

#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#define MAX_LINE 80 /* 80 chars per line, per command */

int main(void) {
	char line[50];
	char *args[MAX_LINE/2 + 1];	/* command line (of 80) has max of 40 arguments */
	int should_run = 1;
	pid_t pid;

	printf("DumbThermometer> ");
	while(should_run){
		fflush(stdout);
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
			if( !strcmp(args[0], "sample") || !strcmp(args[0], "forward") || !strcmp(args[0], "left") ||
		 			!strcmp(args[0], "right") || !strcmp(args[0], "reverse") || !strcmp(args[0], "stop") ){
				pid = fork();
				if( pid == 0 ){
					//starts stream of temperature data
					if(!strcmp(args[0], "sample")){
						fprintf(stdout, "Polling...\n");
						fflush(stdout);
						args[0] = "./robo_polling";
						args[1] = 0;
						execvp(args[0], args);
					}
					//moves robot forward
					if(!strcmp(args[0], "forward")){
						fprintf(stdout, "Forward...\n");
						fflush(stdout);
						args[0] = "python";
						args[1] = "drive.py";
						args[2] = 0;
						execvp(args[0], args);
					}
					//turns robot left
					if(!strcmp(args[0], "left")){
						fprintf(stdout, "Turning left...\n");
						fflush(stdout);
						args[0] = "python";
						args[1] = "left.py";
						args[2] = 0;
						execvp(args[0], args);
					}
					//turns robot right
					if(!strcmp(args[0], "right")){
						fprintf(stdout, "Turning right...\n");
						fflush(stdout);
						args[0] = "python";
						args[1] = "right.py";
						args[2] = 0;
						execvp(args[0], args);
					}
					//puts robot in reverse
					if(!strcmp(args[0], "reverse")){
						fprintf(stdout, "Reversing...\n");
						fflush(stdout);
						args[0] = "python";
						args[1] = "reverse.py";
						args[2] = 0;
						execvp(args[0], args);
					}
					//stops robot
					if(!strcmp(args[0], "stop")){
						fprintf(stdout, "Stopping...\n");
						fflush(stdout);
						args[0] = "python";
						args[1] = "stop.py";
						args[2] = 0;
						execvp(args[0], args);
					}
				}
				else{
					//ensures prompt doesn't come before output
					sleep(1);
					printf("DumbThermometer> ");
				}
			}
			else{
				//because all accepted commands are caught in if statement
				fprintf(stdout, "Error Invalid Command.\n");
				sleep(1);
				printf("DumbThermometer> ");
			}

		}
	}
}
