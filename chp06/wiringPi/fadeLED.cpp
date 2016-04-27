#include <iostream>
#include <wiringPi.h>
#include <unistd.h>
using namespace std;

#define PWM_LED       18      // this is PWM0, pin 12
#define BUTTON_GPIO   27      // this is GPIO27, pin 13
bool running = true;          // fade in/out until button pressed

void buttonPress(void) {      // ISR on button press - not debounced
   cout << "Button was pressed -- start graceful end." << endl;
   running = false;           // the while() loop should end soon
}

int main() {                             // must be run as root
   wiringPiSetupGpio();                  // use the GPIO numbering
   pinMode(PWM_LED, PWM_OUTPUT);         // the PWM LED - PWM0
   pinMode(BUTTON_GPIO, INPUT);          // the button input
   wiringPiISR(BUTTON_GPIO, INT_EDGE_RISING, &buttonPress);
   pwmSetRange(1000);

   cout << "Fading the LED in/out until the button is pressed" << endl;
   while(running) {
      for(int i=1; i<1000; i++) {       // Fade fully on
         pwmWrite(PWM_LED, i);
         usleep(1000);
      }
      for(int i=999; i>=0; i--) {       // Fade fully off
         pwmWrite(PWM_LED, i);
         usleep(1000);
      }
   }
   cout << "LED Off: Program has finished gracefully!" << endl;
   return 0;
}
