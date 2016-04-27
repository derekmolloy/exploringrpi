#include <iostream>
#include "network/SocketServer.h"
using namespace std;
using namespace exploringRPi;

int main(int argc, char *argv[]){
   cout << "Starting RPi Server Example" << endl;
   SocketServer server(54321);
   cout << "Listening for a connection..." << endl;
   server.listen();
   string rec = server.receive(1024);
   cout << "Received from the client [" << rec << "]" << endl;
   string message("The Server says thanks!");
   cout << "Sending back [" << message << "]" << endl;
   server.send(message);
   cout << "End of RPi Server Example" << endl;
   return 0;
}
