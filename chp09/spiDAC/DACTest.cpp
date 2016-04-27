#include <iostream>
#include <math.h>
#include "bus/SPIDevice.h"
using namespace exploringRPi;

int main(){
   // mask = (MSB) 0 (BUF) 0 (GA) 1 (SHDN) 1
   unsigned char mask = 0b00110000;

   std::cout << "Starting RPi SPI DAC Example" << std::endl;
   SPIDevice *busDevice = new SPIDevice(0,0);
   busDevice->setSpeed(20000000);           // max for MCP4921
   busDevice->setMode(SPIDevice::MODE0);    // using SPI mode 0

   // calculate a 12-bit sine wave using 100 samples per period
   unsigned short fn[100];                  // using 16-bit data
   float gain  = 2047.0f;                   // gain of 1.65V
   float phase = 0.0f;                      // phase not important here
   float bias  = 2048.0f;                   // center on 1.65V
   float freq  = 2.0f * 3.14159f / 100.0f;  // 2 * PI / period
   for (int i=0; i<100; i++){               // calculate sine waveform
      fn[i] = (unsigned short)(bias + (gain * sin((i * freq) + phase)));
   }
   unsigned char send[2];                   // sending 16-bits in total
   for(int x=0; x<10000; x++){              // send 10,000 periods
      for(int i=0; i<100; i++){             // 100 samples per period
         send[0] = mask | fn[i]>>8;         // first 4 bits as above
         send[1] = fn[i] & 0x00FF;          // remaining 8 lsbs of sample
         busDevice->transfer(send, NULL, 2);// send the data
      }
   }
   busDevice->close();
   std::cout << "End of RPi SPI DAC Example" << std::endl;
}
