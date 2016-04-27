/* Using an SPI ADC (e.g., the MCP3008)
* Written by Derek Molloy for the book "Exploring Raspberry Pi" */

#include <iostream>
#include <sstream>
#include "bus/SPIDevice.h"

using namespace std;
using namespace exploringRPi;

short combineValues(unsigned char upper, unsigned char lower){
   return ((short)upper<<8)|(short)lower;
}

int main(){
   cout << "Starting RPi SPI ADC Example" << endl;
   SPIDevice *busDevice = new SPIDevice(0,0);
   busDevice->setSpeed(488000);      // Have access to SPI Device object
   busDevice->setMode(SPIDevice::MODE0);
   cout << "The SPI ADC is setup" << endl;

   unsigned char send[3], receive[3];
   send[0] = 0b00000001; // The Start Bit followed
   // Set the SGL/Diff and D mode -- e.g., 1000 means single ended CH0 value
   send[1] = 0b10000000; // The MSB is the Single/Diff bit and it is followed by 000 for CH0
   send[2] = 0;          // This byte doesn't need to be set, just for a clear display
   busDevice->transfer(send, receive, 3);
   cout << "Response bytes are " << (int)receive[1] << "," << (int)receive[2] << endl;

   // Use the 8-bits of the second value and the two LSBs of the first value
   int value = combineValues(receive[1]&0b00000011, receive[2]);
   cout << "This is the value " << value << " out of 1024." << endl;
   cout << "End of ERPi SPI ADC Example" << endl;
}
