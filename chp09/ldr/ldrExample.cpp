/* LDR Example -- measuring light level */
#include <iostream>
#include "bus/SPIDevice.h"
using namespace exploringRPi;

int main(){
   std::cout << "Starting the RPi LDR ADC Example" << std::endl;
   SPIDevice *busDevice = new SPIDevice(0,0);
   busDevice->setSpeed(5000000);
   busDevice->setMode(SPIDevice::MODE0);
   unsigned char send[3], receive[3];
   send[0] = 0b00000110;         // Start Bit followed by SGL/Diff=1
   send[1] = 0b00000000;         // MSB 000 is D1=0, D0=0 for channel 0
   busDevice->transfer(send, receive, 3);
   // MCP320X: use full second byte and the four LSBs of the first byte
   int value = ((receive[1]&0b00001111)<<8)|receive[2];
   std::cout << "LDR value is " << value << " out of 4095." << std::endl;
}
