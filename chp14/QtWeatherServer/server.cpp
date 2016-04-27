#include <iostream>
#include "network/SocketServer.h"
#include <wiringPi.h>
using namespace std;
using namespace exploringRPi;

int main(int argc, char *argv[]){
   wiringPiSetupGpio();
   piHiPri(99);
   cout << "Starting RPi Server Example" << endl;
   SocketServer server(5555);
   cout << "Listening for a connection..." << endl;
   server.threadedListen();
   cout << "End of RPi Server Example" << endl;
   return 0;
}
