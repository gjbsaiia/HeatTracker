//Griffin Saiia, Gjs64
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
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#define MAX_LINE 80 /* 80 chars per line, per command */

void openSerial(){
  pid_t pid = fork();
  char *args[3];
  if( pid == 0){
  }
  else{
    args[0] = "python";
    args[1] = "mode_relay.py";
    args[2] = 0;
    if(execvp(args[0], args) == -1){
      fprintf(stderr, "serial connection failed\n");
    }
  }
}

int main(void) {
	char line[50];
	char *args[MAX_LINE/2 + 1];	/* command line (of 80) has max of 40 arguments */
	int should_run = 1;
	char buf[80];
	pid_t pid;
	openSerial();
	//holds fifo object
  int fd;
  //FIFO file path
  char* myFIFO = "/tmp/myfifo";
  //create FIFO
  if(mkfifo(myFIFO, 0666) == -1){
    fprintf(stderr, "Named pipe failed.\n");
  }

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
				//moves robot forward
				if(!strcmp(args[0], "forward")){
					fd = open(myFIFO, O_WRONLY);
	        sprintf(buf, "%d", 1);
	        write(fd, buf, sizeof(buf));
		      close(fd);
					bzero(buf, sizeof(buf));
					fprintf(stdout, "Driving forward...\n");
					fflush(stdout);
				}
				//turns robot left
				if(!strcmp(args[0], "left")){
					fd = open(myFIFO, O_WRONLY);
	        sprintf(buf, "%d", 2);
	        write(fd, buf, sizeof(buf));
		      close(fd);
					bzero(buf, sizeof(buf));
					fprintf(stdout, "Turning left...\n");
					fflush(stdout);
				}
				//turns robot right
				if(!strcmp(args[0], "right")){
					fd = open(myFIFO, O_WRONLY);
	        sprintf(buf, "%d", 3);
	        write(fd, buf, sizeof(buf));
		      close(fd);
					bzero(buf, sizeof(buf));
					fprintf(stdout, "Turning right...\n");
					fflush(stdout);
				}
				//puts robot in reverse
				if(!strcmp(args[0], "reverse")){
					fd = open(myFIFO, O_WRONLY);
	        sprintf(buf, "%d", 4);
	        write(fd, buf, sizeof(buf));
		      close(fd);
					bzero(buf, sizeof(buf));
					fprintf(stdout, "Reversing...\n");
					fflush(stdout);
				}
				//stops robot
				if(!strcmp(args[0], "stop")){
					fd = open(myFIFO, O_WRONLY);
	        sprintf(buf, "%d", 5);
	        write(fd, buf, sizeof(buf));
		      close(fd);
					bzero(buf, sizeof(buf));
					fprintf(stdout, "Stopping...\n");
					fflush(stdout);
				}
			}
			else{
				pid_t pid = fork();
				if(pid == 1){
					//starts stream of temperature data
					if(!strcmp(args[0], "sample")){
						fprintf(stdout, "Polling...\n");
						fflush(stdout);
						args[0] = "./robo_polling";
						args[1] = 0;
						execvp(args[0], args);
					}
					else{
						execvp(args[0], args);
					}
				}
				else{
					sleep(1);
				}
			}
		}
	}
}
