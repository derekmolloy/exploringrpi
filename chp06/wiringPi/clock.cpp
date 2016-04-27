#include <iostream>
#include <wiringPi.h>
using namespace std;

#define GPCLK0      4                // this is pin 7 GPIO4
#define GPCLK1      5                // pin 29, GPIO5 -- Do not use!
#define GPCLK2      6                // pin 31, GPIO6 -- RPi A+,B+,2

int main() {                         // must be run as root
   wiringPiSetupGpio();              // use the GPIO numbers
   pinMode(GPCLK0, GPIO_CLOCK);      // set up the clock from 19.2MHz base
   gpioClockSet(GPCLK0, 2400000);    // Output a clean 4.8MHz clock
   cout << "The clock output is enabled on GPIO" << GPCLK0 << endl;
   return 0;                         // Clock persists after exit
}
