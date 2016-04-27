#include <iostream>
#include <wiringPi.h>
#include <unistd.h>
using namespace std;

#define PWM_SERVO     18      // this is PWM0, pin 12
#define BUTTON_GPIO   27      // this is GPIO27, pin 13
#define LEFT          29      // manually calibrated values
#define RIGHT         118     // for the left, right and
#define CENTER        73      // center servo positions
bool sweeping = true;         // sweep servo until button pressed

void buttonPress(void) {      // ISR on button press - not debounced
   cout << "Button was pressed -- finishing sweep." << endl;
   sweeping = false;          // the while() loop should end soon
}

int main() {                             // must be run as root
   wiringPiSetupGpio();                  // use the GPIO numbering
   pinMode(PWM_SERVO, PWM_OUTPUT);       // the PWM servo
   pinMode(BUTTON_GPIO, INPUT);          // the button input
   wiringPiISR(BUTTON_GPIO, INT_EDGE_RISING, &buttonPress);
   pwmSetMode(PWM_MODE_MS);              // use a fixed frequency
   pwmSetRange(1000);                    // 1000 steps
   pwmSetClock(384);                     // gives 50Hz precisely

   cout << "Sweeping the servo until the button is pressed" << endl;
   while(sweeping) {
      for(int i=LEFT; i<RIGHT; i++) {       // Fade fully on
         pwmWrite(PWM_SERVO, i);
         usleep(10000);
      }
      for(int i=RIGHT; i>=LEFT; i--) {        // Fade fully off
         pwmWrite(PWM_SERVO, i);
         usleep(10000);
      }
   }
   pwmWrite(PWM_SERVO, CENTER);
   cout << "Program has finished gracefully - servo centred" << endl;
   return 0;
}
