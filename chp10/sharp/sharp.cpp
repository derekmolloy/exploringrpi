/* Using the Sharp GP2D12 sensor with the MCP3208 */
#include <iostream>
#include <unistd.h>
#include <cmath>
#include "bus/SPIDevice.h"
using namespace exploringRPi;
using namespace std;

int main(){
   cout << "Starting the RPi GP2D12 sensor example" << endl;
   SPIDevice *busDevice = new SPIDevice(0,0);
   busDevice->setSpeed(5000000);
   busDevice->setMode(SPIDevice::MODE0);
   for(int i=0; i<1000; i++) {
      unsigned char send[3], receive[3];
      send[0] = 0b00000110;     // Reading single-ended input from channel 0
      send[1] = 0b00000000;
      busDevice->transfer(send, receive, 3);
      int raw = ((receive[1]&0b00001111)<<8)|receive[2];
      float distance = 115.804f * exp(-0.000843107f * (float)raw);
      cout << "The distance is: " << distance << " cm" << '\r' << flush;
      usleep(100000);
   }
   busDevice->close();
   return 0;
}
