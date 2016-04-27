/* Written by Derek Molloy for the book "Exploring Raspberry Pi" */

#include <iostream>
#include <sstream>
#include "SPIDevice.h"
#include "ADXL345.h"
using namespace std;
using namespace exploringRPi;

int main(){
   cout << "Starting RPi ADXL345 SPI Test" << endl;
   SPIDevice *spiDevice = new SPIDevice(0,0);   // Using SPI bus 0
   spiDevice->setSpeed(500000);                 // Have access to SPI Device object
   spiDevice->setMode(SPIDevice::MODE3);
   ADXL345 acc(spiDevice);
   acc.displayPitchAndRoll(100);
   cout << "End of RPi ADXL345 SPI Test" << endl;
}
