#ifndef CONNECTIONHANDLER_H_
#define CONNECTIONHANDLER_H_
#include<sys/socket.h>
#include<netinet/in.h>
#include<unistd.h>
#include<iostream>

namespace exploringRPi {

class SocketServer;

class ConnectionHandler {
public:
   ConnectionHandler(SocketServer *parent, sockaddr_in *client, int clientSocketfd);
   virtual ~ConnectionHandler();

   int start();
   void wait();
   void stop() { this->running = false; }

   virtual int send(std::string message);
   virtual std::string receive(int size);

protected:
	virtual void threadLoop();

private:
   sockaddr_in *client;
   int 	       clientSocketfd;
   pthread_t   thread;
   SocketServer *parent;
   bool        running;

   float temperature, humidity;
   int readDHTSensor();

   static void * threadHelper(void * handler){
   	  ((ConnectionHandler *)handler)->threadLoop();
   	  return NULL;
   }
};

} /* namespace exploringRPi */

#endif /* CONNECTIONHANDLER_H_ */
