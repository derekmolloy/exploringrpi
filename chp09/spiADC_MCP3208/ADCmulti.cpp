/* Using an SPI ADC (e.g., the MCP3208) */
#include <iostream>
#include "bus/SPIDevice.h"
#define SAMPLES 200
using namespace exploringRPi;

int main(){
   short data[SAMPLES];
   std::cout << "# Starting RPi SPI ADC Example" << std::endl;
   SPIDevice *busDevice = new SPIDevice(0,0);
   busDevice->setSpeed(5000000);
   busDevice->setMode(SPIDevice::MODE0);
   unsigned char send[3], receive[3];
   send[0] = 0b00000110;     // Reading single-ended input from channel 0
   send[1] = 0b00000000;
   for(int i=0; i<SAMPLES; i++) {
      busDevice->transfer(send, receive, 3);
      data[i] = ((receive[1]&0b00001111)<<8)|receive[2];
   }
   for(int i=0; i<SAMPLES; i++) {      // print after data captured
      std::cout << i << " " << data[i] << std::endl;
   }
   busDevice->close();
   std::cout << "# End of RPi SPI ADC Example" << std::endl;
   return 0;
}
