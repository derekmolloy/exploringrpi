#ifndef CONNECTIONHANDLER_H_
#define CONNECTIONHANDLER_H_
#include<sys/socket.h>
#include<netinet/in.h>
#include<unistd.h>
#include<iostream>

namespace exploringRPi {

class SocketServer; //class declaration, due to circular reference problem.

/***
 * This class encapsulates a thread, so when an object of this class is created so
 * a thread is created that runs according to the code in the threadLoop() function.
 * This frees up the server to accept connections from many clients at the same time
 * with one connection handler object created for each connection that is established.
 */

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

   static void * threadHelper(void * handler){
   	  ((ConnectionHandler *)handler)->threadLoop();
   	  return NULL;
   }
};

} /* namespace exploringRPi */

#endif /* CONNECTIONHANDLER_H_ */
