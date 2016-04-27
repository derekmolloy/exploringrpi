/* Using the ADXL335 sensor with the MCP3208 */
#include <iostream>
#include "bus/SPIDevice.h"
using namespace exploringRPi;
using namespace std;

int main(){
   cout << "Starting the RPi ADXL335 example" << endl;
   SPIDevice *busDevice = new SPIDevice(0,0);
   busDevice->setSpeed(5000000);
   busDevice->setMode(SPIDevice::MODE0);
   unsigned char send[3], receive[3];
   send[0] = 0b00000110;     // Reading single-ended input from channel 0
   send[1] = 0b00000000;
   busDevice->transfer(send, receive, 3);
   int raw = ((receive[1]&0b00001111)<<8)|receive[2];
   cout << "The raw value is: " << raw << endl;
   float angle = (0.0528634 * raw) - 30.0528;
   cout << "The angle is " << angle << " degrees" << endl;
   busDevice->close();
   return 0;
}
