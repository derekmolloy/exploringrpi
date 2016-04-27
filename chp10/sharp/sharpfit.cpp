/* Using the Sharp GP2D12 sensor with the MCP3208 */
#include <iostream>
#include <cmath>
#include <unistd.h>
#include "bus/SPIDevice.h"
using namespace exploringRPi;
using namespace std;

int main(){
   cout << "Starting the RPi GP2D12 sensor example" << endl;
   SPIDevice *busDevice = new SPIDevice(0,0);
   busDevice->setSpeed(5000000);
   busDevice->setMode(SPIDevice::MODE0);
   unsigned char send[3], receive[3];
   send[0] = 0b00000110;     // Reading single-ended input from channel 0
   send[1] = 0b00000000;
   busDevice->transfer(send, receive, 3);
   int raw = ((receive[1]&0b00001111)<<8)|receive[2];

   cout << "Raw value is " << (int)raw << endl;
   float distance = 115.804f * exp(-0.000843107f * (float)raw);
   cout << "Estimate 1 (Wolfram): " << distance << " cm" << endl;
   distance = 1.19894f + (42466.4f / (float)raw);
   cout << "Estimate 2 (1/x)    : " << distance << " cm" << endl;
   distance = 115.0f * exp(-0.000836107f * (float)raw);
   cout << "Estimate 3 (exp dec): " << distance << " cm" << endl;

   busDevice->close();
   return 0;
}
