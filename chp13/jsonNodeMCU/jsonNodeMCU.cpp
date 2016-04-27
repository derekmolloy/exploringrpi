#include <iostream>
#include "json/json.h"
#include "network/SocketClient.h"
using namespace std;
using namespace exploringRPi;

int main(int argc, char *argv[]){
   Json::Value root;
   Json::Reader reader;
   if(argc!=2){
      cout << "Usage is: jsonNodeMCU nodeMCU_IP" << endl;
      return 2;
   }
   SocketClient sc(argv[1], 80);
   sc.connectToServer();
   string message("GET / HTTP/1.1");
   //cout << "Sending [" << message << "]" << endl;
   sc.send(message);
   string rec = sc.receive(1024);
   bool success = reader.parse(rec, root, false);
   if(!success){               // has the parsing failed?
      cout << "Failed: " <<  reader.getFormattedErrorMessages() << endl;
   }
   float temperature = root.get("temperature", "UTF-8").asFloat();
   bool button = root.get("button", "UTF-8").asBool();
   cout << "The temperature is " << temperature << "°C" << endl;
   cout << "The button is " << (button ? "pressed":"not pressed") << endl;
   return 0;
}
