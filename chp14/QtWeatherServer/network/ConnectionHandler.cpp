#include "ConnectionHandler.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <wiringPi.h>
#include <unistd.h>
#include "SocketServer.h"
using namespace std;

#define USING_DHT11      false   // The DHT11 uses only 8 bits
#define DHT_GPIO         22      // Using GPIO 22 for this example
#define LH_THRESHOLD     26      // Low=~14, High=~38 - pick avg.

namespace exploringRPi {

ConnectionHandler::ConnectionHandler(SocketServer *parent, sockaddr_in *client, int clientSocketfd) {
   this->parent = parent;
   this->client = client;
   this->running = true;
   this->clientSocketfd = clientSocketfd;
   //cout << "Created a Connection Handler" << endl;
}

ConnectionHandler::~ConnectionHandler() {
   delete this->client;
   cout << "Destroyed a Connection Handler" << endl;
}

int ConnectionHandler::start(){
   cout << "Starting the Connection Handler thread" << endl;
   return (pthread_create(&(this->thread), NULL, threadHelper, this)==0);
}

void ConnectionHandler::wait(){
	(void) pthread_join(this->thread, NULL);
}

int ConnectionHandler::send(std::string message){
	const char *writeBuffer = message.data();
	int length = message.length();
    int n = write(this->clientSocketfd, writeBuffer, length);
    if (n < 0){
       perror("Connection Handler: error writing to server socket.");
       return 1;
    }
    return 0;
}

string ConnectionHandler::receive(int size=1024){
    char readBuffer[size];
    int n = read(this->clientSocketfd, readBuffer, sizeof(readBuffer));
    if (n < 0){
       perror("Connection Handler: error reading from server socket.");
    }
    return string(readBuffer);
}

void ConnectionHandler::threadLoop(){
    cout << "*** Created a Connection Handler threaded Function" << endl;
    string rec = this->receive(1024);
    if (rec == "getReading"){
       cout << "Received from the client [" << rec << "]" << endl;
       if (this->readDHTSensor()<0) {
          cout << "Failed to make a reading from the DHT sensor" << endl;
       }
       stringstream ss;
       ss << " { \"sample\": { \"temperature\" : " << temperature;
       ss << ", \"humidity\": " << humidity << " } } ";
       this->send(ss.str());
       cout << "Sent [" << ss.str() << "]" << endl;
    }
    else {
       cout << "Received from the client [" << rec << "]" << endl;
       this->send(string("Unknown Command"));
    }
    cout << "*** End of the Connection Handler Function" << endl;
    this->parent->notifyHandlerDeath(this);
}

int ConnectionHandler::readDHTSensor(){
   int humid=0, temp=0;
   unsigned char data[5] = {0,0,0,0,0};
   pinMode(DHT_GPIO, OUTPUT);                 // gpio starts as output
   digitalWrite(DHT_GPIO, LOW);               // pull the line low
   usleep(18000);                             // wait for 18ms
   digitalWrite(DHT_GPIO, HIGH);              // set the line high
   pinMode(DHT_GPIO, INPUT);                  // now gpio is an input

   // need to ignore the first and second high after going low
   do { delayMicroseconds(1); } while(digitalRead(DHT_GPIO)==HIGH);
   do { delayMicroseconds(1); } while(digitalRead(DHT_GPIO)==LOW);
   do { delayMicroseconds(1); } while(digitalRead(DHT_GPIO)==HIGH);
   // Remember the highs, ignore the lows -- a good philosophy!
   for(int d=0; d<5; d++) {       // for each data byte
      // read 8 bits
      for(int i=0; i<8; i++) {    // for each bit of data
         do { delayMicroseconds(1); } while(digitalRead(DHT_GPIO)==LOW);
         int width = 0;           // measure width of each high
         do {
            width++;
            delayMicroseconds(1);
            if(width>1000) break; // missed a pulse -- data invalid!
         } while(digitalRead(DHT_GPIO)==HIGH);    // time it!
         // shift in the data, msb first if width > the threshold
         data[d] = data[d] | ((width > LH_THRESHOLD) << (7-i));
      }
   }
   if (USING_DHT11){
      humid = data[0] * 10;            // one byte - no fractional part
      temp = data[2] * 10;             // multiplying to keep code concise
   }
   else {                              // for DHT22 (AM2302/AM2301)
      humid = (data[0]<<8 | data[1]);  // shift MSBs 8 bits left and OR LSBs
      temp = (data[2]<<8 | data[3]);   // same again for temperature
   }
   unsigned char chk = 0;   // the checksum will overflow automatically
   for(int i=0; i<4; i++){ chk+= data[i]; }
   if(chk==data[4]){
      temperature = ((float)temp)/10;
      humidity = ((float)humid)/10;
   }
   else {
      return -1;
   }
   return 0;
}


}
