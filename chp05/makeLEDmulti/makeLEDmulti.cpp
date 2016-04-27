/** Simple GPIO LED flashing program - written in C++ by Derek Molloy
*    simple functional struture for the Exploring Raspberry Pi book
*    changed to a multicall binary form
*
*    This program uses GPIO4 and can be executed by calling each of the
*    symbolic links. You MUST execute the build script to create the
*    symbolic links.
*/

#include<iostream>
#include<fstream>
#include<string>
#include<unistd.h>
using namespace std;

#define GPIO_NUMBER "4"
#define GPIO4_PATH "/sys/class/gpio/gpio4/"
#define GPIO_SYSFS "/sys/class/gpio/"

void writeGPIO(string path, string filename, string value){
   fstream fs;
   fs.open((path + filename).c_str(), fstream::out);
   fs << value;
   fs.close();
}

bool endsWith(string const &in, string const &comp){
   return (0 == in.compare(in.length()-comp.length(), comp.length(), comp));
}

int main(int argc, char* argv[]){
   if(argc!=1){
      cout << "You must use the symbolic links to call this multi-call binary program" << endl;
      return 2;
   }
   string cmd(argv[0]);
   cout << "Starting the makeLED multi program" << endl;
   cout << "The current LED Path is: " << GPIO4_PATH << endl;

   if(endsWith(cmd,"onLED")){
      cout << "Turning the LED on" << endl;
      writeGPIO(string(GPIO4_PATH), "value", "1");
   }
   else if (endsWith(cmd,"offLED")){
      cout << "Turning the LED off" << endl;
      writeGPIO(string(GPIO4_PATH), "value", "0");
   }
   else if (endsWith(cmd,"setupLED")){
      cout << "Setting up the GPIO" << endl;
      writeGPIO(string(GPIO_SYSFS), "export", GPIO_NUMBER);
      usleep(100000);
      writeGPIO(string(GPIO4_PATH), "direction", "out");
   }
   else if (endsWith(cmd,"closeLED")){
      cout << "Unexporting the GPIO" << endl;
      writeGPIO(string(GPIO_SYSFS), "unexport", GPIO_NUMBER);
   }
   else if (endsWith(cmd,"statusLED")){
      std::fstream fs;
      fs.open( GPIO4_PATH "value", std::fstream::in);
      string line;
      while(getline(fs,line)) cout << "The state is: " << line << endl;
      fs.close();
   }
   else{
      cout << "Invalid command!" << endl;
   }
   cout << "Finished the makeLED Program" << endl;
   return 0;
}
