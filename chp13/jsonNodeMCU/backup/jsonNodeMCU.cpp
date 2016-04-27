#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <strings.h>
#include <sstream>
#include <iostream>
using namespace std;

int main(int argc, char *argv[]){
   stringstream message;
   int    socketfd, portNumber, length;
   char   readBuffer[2000];
   struct sockaddr_in serverAddress; //describes endpoint to connect a socket
   struct hostent *server;           //stores information about a host name

   message << "GET / HTTP/1.1\r\nHost: %s\r\nConnection: close\r\n\r\n" << endl;
   cout << "Sending the message: " <<  message << endl;
   if (argc<=1){  // must pass the hostname
      cout << "Incorrect usage, use: ./webBrowser hostname" << endl;
      return 2;
   }
   // gethostbyname accepts a string name and returns a host name structure
   server = gethostbyname(argv[1]);
   if (server == NULL) {
      cout << "Socket Client: error - unable to resolve host name" << endl;
      return 1;
   }
   // Create the socket of IP address type, SOCK_STREAM is for TCP connections
   socketfd = socket(AF_INET, SOCK_STREAM, 0);
   if (socketfd < 0){
      cout << "Socket Client: error opening TCP IP-based socket." << endl;
      return 1;
   }
   // clear the data in the serverAddress sockaddr_in struct
   bzero((char *) &serverAddress, sizeof(serverAddress));
   portNumber = 80;
   serverAddress.sin_family = AF_INET; //set the address family to be IP
   serverAddress.sin_port = htons(portNumber);   //set the port number to 80
   bcopy((char *)server->h_addr,(char *)&serverAddress.sin_addr.s_addr,
      server->h_length);  //set the address to the resolved hostname address

   // try to connect to the server
   if (connect(socketfd, (struct sockaddr *) &serverAddress, sizeof(serverAddress)) < 0){
      cout << "Socket Client: error connecting to the server." << endl;
      return 1;
   }
   // send the HTTP request string
   if (write(socketfd, message, sizeof(message)) < 0){
      cout << "Socket Client: error writing to socket" << endl;
      return 1;
   }
   // read the HTTP response to a maximum of 2000 characters
   if (read(socketfd, readBuffer, sizeof(readBuffer)) < 0){
      cout << "Socket Client: error reading from socket" << endl;
      return 1;
   }
   cout << readBuffer << endl;  // display the response
   close(socketfd);             // close the socket
   return 0;
}
