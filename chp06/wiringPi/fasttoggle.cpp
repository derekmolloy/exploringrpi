// Do not optimize this code using -O3 as it will remove the delay hack
#include <wiringPi.h>
#include <iostream>
using namespace std;
#define LED_GPIO  17                   // this is GPIO17, pin 11

int main() {
   wiringPiSetupGpio();                // use GPIO not WPi labels
   cout << "Starting fast GPIO toggle on GPIO" << LED_GPIO << endl;
   cout << "Press CTRL+C to quit..." << endl;
   pinMode(LED_GPIO, OUTPUT);          // GPIO17 is an output pin
   while(1) {                          // loop forever
      digitalWrite(LED_GPIO, HIGH);    // LED on
      for(int i=0; i<50; i++) { }      // blocking delay
      digitalWrite(LED_GPIO, LOW);     // LED off
      for(int i=0; i<49; i++) { }      // blocking delay
   }
   return 0;                           // program will not reach here
}
