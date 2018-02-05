//Griffin Saiia

//Executed on Pi
//Sensor reading and client program
//Polls DHT11 sensor and sends data back to server

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <wiringPi.h>
#include <stdint.h>
#include <sched.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#define TIME	85
#define PIN		7
int sensorData[5] = { 0, 0, 0, 0, 0 };
int badData = 0;


//polls temperature sensor - follows DHT11's protocol
void readSensor()
{
  uint8_t laststate	= HIGH;
  uint8_t counter		= 0;
  uint8_t j	 = 0, i;

  sensorData[0] = sensorData[1] = sensorData[2] = sensorData[3] = sensorData[4] = 0;

  // pull pin down for 18 milliseconds
  pinMode( PIN, OUTPUT );
  digitalWrite( PIN, LOW );
  delay( 18 );
  // then pull it up for 40 microseconds
  digitalWrite( PIN, HIGH );
  delayMicroseconds( 40 );
  // prepare to read the pin
  pinMode( PIN, INPUT );

  // detect change and read data
  for ( i = 0; i < TIME; i++ )
  {
    counter = 0;
    while ( digitalRead( PIN ) == laststate )
    {
      counter++;
      delayMicroseconds( 1 );
      if ( counter == 255 )
      {
        break;
      }
    }
    laststate = digitalRead( PIN );
    if ( counter == 255 )
      break;

    // ignore first 3 transitions
    if ( (i >= 4) && (i % 2 == 0) )
    {
      // shove each bit into the storage bytes
      sensorData[j / 8] <<= 1;
      if ( counter > 16 )
        sensorData[j / 8] |= 1;
      j++;
    }
  }

  // check we read 40 bits (8bit x 5 ) + verify checksum in the last byte
  // print it out if data is good
  if ( (j >= 40) &&
       (sensorData[4] == ( (sensorData[0] + sensorData[1] + sensorData[2] + sensorData[3]) & 0xFF) ) )
  {
    badData = 0;
  }
  else  {
    badData = 1;
  }
}


int main(void)
{
  //sets up client
  int sockfd, portno, n;
  struct sockaddr_in serv_addr;
  struct hostent *server;
  char *buffer[256];
  portno = 2000;
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd < 0){
    fprintf(stderr,"ERROR opening socket\n");
  }
  server = gethostbyname("172.20.41.100");  //subject to change depending on network, this IP is for casewireless
  if (server == NULL) {
    fprintf(stderr,"ERROR, no such host\n");
    exit(0);
  }
  bzero((char *) &serv_addr, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
  serv_addr.sin_port = htons(portno);
  //connecting to server
  if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0){
    fprintf(stderr,"ERROR connecting\n");
  }
  //loops until server's say so
  while(1){
    //polls sensor
    readSensor();
    bzero(buffer,256);
    //there's a chance that the sensor input is invalid, checks badData flag
    if(badData == 0){
      sprintf((char *)&buffer, "%d", sensorData[2]);
    }
    else{
      //the server knows not to count this data and won't display it
      sprintf((char *)&buffer, "bad data");
    }
    //writes to server
    n = write(sockfd, buffer, strlen((char *)&buffer));
    //checks error
    if (n < 0){
      fprintf(stderr,"ERROR writing to socket\n");
    }
    //reads server feedback for terminate command
    n = read( sockfd, buffer, 256);
    //checks error
    if (n < 0){
      fprintf(stderr,"ERROR reading from socket\n");
    }
    if( atoi((char *)&buffer) == -1 ){
      break;
    }
    //waits 7 seconds between each reading
    delay(7000);
  }
  //closes socket
  close(sockfd);
  return 0;
}
