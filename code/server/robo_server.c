//Griffin Saiia, gjs64
//Dumb Thermometer, OS Final Project

//Executed on computer or phone acting as server.
//Allows for Pi to stream back temperature data.
//Takes the number of data points you want as parameter, but defaults to 15.
//Counts number of data points until you have reached limit and then terminates
//the client that's reading the Sensor.
//Also checks for bad data that has been streamed back from the client - it omits,
//this data and does not include it in limit count.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <time.h>


int main(int argc, char *argv[])
{
    //sets up server
    int sockfd, newsockfd, portno;
    int limit;
    socklen_t clilen;
    char *buffer[256];
    struct sockaddr_in serv_addr, cli_addr;
    int n;
    //checks arguments
    if (argc < 2) {
      limit = 15;
    }
    else{
      //checks if the limit number exceeds array size
      if(atoi(argv[1]) <= 100){
        limit = atoi(argv[1]);
      }
      else{
        fprintf(stderr, "Limit exceeds maximum data limit. \n");
        limit = 15;
      }
    }
    //sets up array to store temperature data in
    int tempData[100];
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
      fprintf(stderr,"ERROR opening socket. \n");
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    //made pi and computer always communicate through port 2000
    portno = 2000;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);
    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0){
      fprintf(stderr,"ERROR on binding. \n");
    }
    listen(sockfd,5);
    clilen = sizeof(cli_addr);
    newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
    if (newsockfd < 0) {
      fprintf(stderr,"ERROR on accept. \n");
    }
    //counter for data points
    int i = 0;
    //runs until data point limit is reached
    printf("DumbThermometer is collecting data to calculate the room temperature: \n");
    while(1){
      bzero(buffer, 256);
      //reads data
      n = read( newsockfd, buffer, 256);
      if (n < 0){
        fprintf(stderr,"ERROR reading from socket\n");
      }

      //checks if data is bad (atoi returns 0 if the argument is a character)
      if(atoi((char *)&buffer) == 0){
        ;
      }
      else{
        tempData[i] = atoi((char *)&buffer);
        printf("Data entry %d: %d*F\n", i, tempData[i]);
        i++;
      }
      bzero(buffer, 256);
      //checks limit
      if(i >= limit){
        //writes terminate code
        sprintf((char *)&buffer, "%d", -1);
        n = write(newsockfd, buffer, strlen((char *)&buffer));
        if (n < 0){
          fprintf(stderr,"ERROR writing to socket\n");
        }
        sleep(2);
        break;
      }
      else{
        //writes anything other than the terminate code
        sprintf((char *)&buffer, "%d", 5);
        n = write(newsockfd, buffer, strlen((char *)&buffer));
        if (n < 0){
          fprintf(stderr,"ERROR writing to socket\n");
        }
      }
    }
    printf("DumbThermometer is done collecting data.\n");
    //calculates average of data
    double average = 0;
    for(int j = 0; j < limit; j++){
      average = average + (double)tempData[j];
    }
    average = average / (double)limit;
    //prints the room temperature
    printf("The room temperature is %.02f*C\n", average);
    //closes socket
    close(newsockfd);
    close(sockfd);
    return 0;
}
