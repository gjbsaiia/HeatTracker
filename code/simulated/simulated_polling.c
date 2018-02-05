//Griffin Saiia, Gjs64
//Dumb Thermometer, OS Final Project

//Simulated data reading and client program
//feeds fake data back to server


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

int main(void)
{
    //sets up client
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    char *buffer[256];
    int fakeData[15] = {45, 42, 42, 43, 44, 45, 44, 43, 41, 40, 47, 45, 47, 43, 46};
    //this is just the port I dedicated to this
    portno = 2000;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0){
      fprintf(stderr,"ERROR opening socket\n");
    }
    //sends data right back to yourself
    server = gethostbyname("127.0.0.1");
    if (server == NULL) {
      fprintf(stderr,"ERROR, no such host\n");
      exit(0);
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
    serv_addr.sin_port = htons(portno);
    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0){
      fprintf(stderr,"ERROR connecting\n");
    }
    //to iterate through fake data
    int i = 0;
    //loops through data and sends it back
    while(1){
      bzero(buffer,256);
      //sends fake data
      sprintf((char *)&buffer, "%d", fakeData[i]);
      n = write(sockfd, buffer, strlen((char *)&buffer));
      //iterates fake data variable
      i++;
      if (n < 0){
        fprintf(stderr,"ERROR writing to socket\n");
      }
      //checks for terminate signal from server
      n = read( sockfd, buffer, 256);
      if (n < 0){
        fprintf(stderr,"ERROR reading from socket\n");
      }
      if( atoi((char *)&buffer) == -1 ){
        break;
      }
      //waits 2 seconds
      sleep(2);
    }
    //closes socket
    close(sockfd);
    return 0;
}
