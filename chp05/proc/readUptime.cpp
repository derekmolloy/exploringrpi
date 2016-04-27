/** Simple /proc/uptime parsing program
* Written by Derek Molloy for the book "Exploring Raspberry Pi*/

#include<iostream>
#include<fstream>
#include<string>
using namespace std;

int main(int argc, char* argv[]){
   cout << "Starting the read uptime program" << endl;
   std::fstream fs;
   fs.open("/proc/uptime", std::fstream::in);
   float uptime, idletime;
   fs >> uptime >> idletime;
   cout << "The system up time is " << uptime/60 << " minutes.\n";
   cout << "It was idle for " << idletime/60 << " minutes, or "
        <<  100*(idletime/uptime) << "%\n";
   fs.close();
   return 0;
}

