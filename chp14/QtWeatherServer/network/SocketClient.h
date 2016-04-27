#ifndef SOCKETCLIENT_H
#define SOCKETCLIENT_H

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string>

namespace exploringRPi {

class SocketClient {

private:
    int 	    socketfd;
    struct 	    sockaddr_in   serverAddress;
    struct 	    hostent       *server;
    std::string serverName;
    int         portNumber;
    bool        isConnected;

public:
	SocketClient(std::string serverName, int portNumber);
	virtual int connectToServer();
	virtual int disconnectFromServer();
	virtual int send(std::string message);
	virtual std::string receive(int size);
	//virtual std::string receiveAll();
	bool isClientConnected() { return this->isConnected; }
	virtual ~SocketClient();
};

}

#endif /* SOCKETCLIENT_H_ */
