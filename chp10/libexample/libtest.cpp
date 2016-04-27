#include <iostream>
#include <sstream>
#include "display/LCDCharacterDisplay.h"
using namespace exploringRPi;
using namespace std;

int main() {
   cout << "Testing the ERPi library" << endl;
   SPIDevice *busDevice = new SPIDevice(0,0);
   busDevice->setSpeed(1000000);      // access to SPI Device object
   ostringstream s;                   // using to combine text and ints
   LCDCharacterDisplay display(busDevice, 20, 4); // a 20x4 display
   display.clear();                   // Clear the character LCD module
   display.home();                    // Move to the (0,0) position
   display.print("   Exploring RPi");
   cout << "End of the ERPi library test" << endl;
   return 0;
}
