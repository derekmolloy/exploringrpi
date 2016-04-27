#include <iostream>
#include <sstream>
#include "display/LCDCharacterDisplay.h"
using namespace std;
using namespace exploringRPi;

int main(){
   cout << "Starting LCD Character Display Example" << endl;
   SPIDevice *busDevice = new SPIDevice(0,0);
   busDevice->setSpeed(1000000);      // Have access to SPI Device object
   ostringstream s;                   // Using this to combine text and int data
   LCDCharacterDisplay display(busDevice, 20, 4); // Construct 20x4 LCD Display
   display.clear();                   // Clear the character LCD module
   display.home();                    // Move the cursor to the (0,0) position
   display.print("   Exploring RPi");
   display.setCursorPosition(1,3);
   display.print("by Derek Molloy");
   display.setCursorPosition(2,0);
   display.print("www.exploringrpi.com");
   for(int x=0; x<=10000; x++){       // Do this 10,000 times
      s.str("");                      // clear the ostringstream object s
      display.setCursorPosition(3,7); // move the cursor to second row
      s << "X=" << x;                 // construct a string that has an int value
      display.print(s.str());         // print the string X=*** on the LCD module
   }
   cout << "End of LCD Character Display Example" << endl;
   return 0;
}
