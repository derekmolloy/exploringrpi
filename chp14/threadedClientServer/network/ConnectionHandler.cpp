#include "ConnectionHandler.h"
#include <stdio.h>
#include <pthread.h>
#include <iostream>
#include <string.h>
#include "SocketServer.h"
using namespace std;

namespace exploringRPi {

ConnectionHandler::ConnectionHandler(SocketServer *parent, sockaddr_in *client, int clientSocketfd) {
	this->parent = parent;
	this->client = client;
	this->clientSocketfd = clientSocketfd;
	this->running = true;
}

ConnectionHandler::~ConnectionHandler() {
	delete this->client;
	cout << "Destroyed a Connection Handler" << endl;
}

int ConnectionHandler::start(){
	//cout << "Starting the Connection Handler thread" << endl;
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
    bzero(readBuffer, size);
    int n = read(this->clientSocketfd, readBuffer, sizeof(readBuffer));
    if (n < 0){
       perror("Connection Handler: error reading from server socket.");
    }
    return string(readBuffer);
}

void ConnectionHandler::threadLoop(){
    //cout << "*** Created a Connection Handler threaded Function" << endl;
    while(this->running){
       string rec = this->receive(1024);
       cout << "Received from the client [" << rec << "]" << endl;
       string message("The Server says thanks!");
       cout << "Sending back [" << message << "]" << endl;
       cout << "  but going asleep for 5 seconds first...." << endl;
       usleep(5000000);
       this->send(message);
       this->running = false;
	}
    //cout << "*** End of the Connection Handler Function" << endl;
    this->parent->notifyHandlerDeath(this);
}

} /* namespace exploringRPi */
