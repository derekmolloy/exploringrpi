#include <iostream>
#include <wiringPi.h>
using namespace std;

#define LED_GPIO      17      // this is GPIO17, pin 11
#define BUTTON_GPIO   27      // this is GPIO27, pin 13

int main() {                             // must be run as root
   wiringPiSetupGpio();                  // use the GPIO numbering
   pinMode(LED_GPIO, OUTPUT);            // the LED set as an output
   pinMode(BUTTON_GPIO, INPUT);          // the Button set as input

   int model, rev, mem, maker, overVolted;
   piBoardId(&model, &rev, &mem, &maker, &overVolted);
   cout << "This is an RPi: " << piModelNames[model] << endl;
   cout << " with revision number: " << piRevisionNames[rev] << endl;
   cout << " manufactured by: " << piMakerNames[maker] << endl;
   cout << " it has: " << mem << " RAM and o/v: " << overVolted << endl;

   cout << "Button GPIO has ALT mode: " << getAlt(BUTTON_GPIO);
   cout << "  and value: " << digitalRead(BUTTON_GPIO) << endl;
   cout << "LED GPIO has ALT mode: " << getAlt(LED_GPIO);
   cout << "  and value: " << digitalRead(LED_GPIO) << endl;
   return 0;
}
