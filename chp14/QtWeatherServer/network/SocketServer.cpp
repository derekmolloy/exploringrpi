#include "SocketServer.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <algorithm>
using namespace std;

namespace exploringRPi {

SocketServer::SocketServer(int portNumber) {
	this->socketfd = -1;
	this->clientSocketfd = -1;
	this->portNumber = portNumber;
	this->clientConnected = false;
	this->connections = vector<ConnectionHandler *>();
}

int SocketServer::listen(){
    this->socketfd = socket(AF_INET, SOCK_STREAM, 0);
    if (this->socketfd < 0){
    	perror("Socket Server: error opening socket.\n");
    	return 1;
    }
    bzero((char *) &serverAddress, sizeof(serverAddress));
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(this->portNumber);
    if (bind(socketfd, (struct sockaddr *) &serverAddress, sizeof(serverAddress)) < 0){
    	perror("Socket Server: error on binding the socket.\n");
    	return 1;
    }
    ::listen(this->socketfd, 5);
    socklen_t clientLength = sizeof(this->clientAddress);
    this->clientSocketfd = accept(this->socketfd,
    		(struct sockaddr *) &this->clientAddress,
    		&clientLength);
    if (this->clientSocketfd < 0){
    	perror("Socket Server: Failed to bind the client socket properly.\n");
    	return 1;
    }
    return 0;
}

int SocketServer::threadedListen(){

    this->socketfd = socket(AF_INET, SOCK_STREAM, 0);
    if (this->socketfd < 0){
    	perror("Socket Server: error opening socket.\n");
    	return 1;
    }
    bzero((char *) &serverAddress, sizeof(serverAddress));
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(this->portNumber);
    if (bind(socketfd, (struct sockaddr *) &serverAddress, sizeof(serverAddress)) < 0){
    	perror("Socket Server: error on binding the socket.\n");
    	return 1;
    }
    ::listen(this->socketfd, 5);
    while(true){
  	struct sockaddr_in *temp = new (struct sockaddr_in)();
		socklen_t tempLength = sizeof(*temp);
		int tempSocketfd = accept(this->socketfd, (struct sockaddr *) temp, &tempLength);
		if (tempSocketfd < 0){
			perror("Socket Server: Failed to bind the threaded client socket properly.\n");
			return 1;
		}
		else {
			//printf("Valid socket\n");
			ConnectionHandler *conn = new ConnectionHandler(this, temp, tempSocketfd);
			this->connections.push_back(conn);
			conn->start();
		}
    }
}

int SocketServer::send(std::string message){
	const char *writeBuffer = message.data();
	int length = message.length();
    int n = write(this->clientSocketfd, writeBuffer, length);
    if (n < 0){
       perror("Socket Server: error writing to server socket.");
       return 1;
    }
    return 0;
}

string SocketServer::receive(int size=1024){
    char readBuffer[size];
    int n = read(this->clientSocketfd, readBuffer, sizeof(readBuffer));
    if (n < 0){
       perror("Socket Server: error reading from server socket.");
    }
    return string(readBuffer);
}

void SocketServer::notifyHandlerDeath(ConnectionHandler *connection){
   std::vector<ConnectionHandler *>::iterator it;
   for(it = this->connections.begin(); it <= this->connections.end(); it++ ){
      if ( *it == connection){
         this->connections.erase(it);
         printf("Server: Found and deleted the connection reference...\n");
      }
   }
   delete connection;
}

SocketServer::~SocketServer() {
	close(this->socketfd);
	close(this->clientSocketfd);
}

}
