/*
 * SocketServer.h  Created on: 13 Jul 2015
 * Copyright (c) 2015 Derek Molloy (www.derekmolloy.ie)
 * Made available for the book "Exploring Raspberry Pi"
 * See: www.exploringrpi.com
 * Licensed under the EUPL V.1.1
 *
 * This Software is provided to You under the terms of the European 
 * Union Public License (the "EUPL") version 1.1 as published by the 
 * European Union. Any use of this Software, other than as authorized 
 * under this License is strictly prohibited (to the extent such use 
 * is covered by a right of the copyright holder of this Software).
 * 
 * This Software is provided under the License on an "AS IS" basis and 
 * without warranties of any kind concerning the Software, including 
 * without limitation merchantability, fitness for a particular purpose, 
 * absence of defects or errors, accuracy, and non-infringement of 
 * intellectual property rights other than copyright. This disclaimer 
 * of warranty is an essential part of the License and a condition for 
 * the grant of any rights to this Software.
 * 
 * For more details, see http://www.derekmolloy.ie/
 */

#ifndef SOCKETSERVER_H_
#define SOCKETSERVER_H_

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <string>

namespace exploringRPi {

/**
 * @class SocketServer
 * @brief A class that encapsulates a server socket for network communication
 */
class SocketServer {
private:
	int 		portNumber;
    int 	    socketfd, clientSocketfd;
    struct 	    sockaddr_in   serverAddress;
    struct 	    sockaddr_in   clientAddress;
    bool		clientConnected;

public:
	SocketServer(int portNumber);
	virtual int listen();
	virtual int send(std::string message);
	virtual std::string receive(int size);

	virtual ~SocketServer();
};

} /* namespace exploringRPi */

#endif /* SOCKETSERVER_H_ */
