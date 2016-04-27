#include <iostream>
#include <wiringPi.h>
using namespace std;

#define PWM0       12                    // this is physical pin 12
#define PWM1       33                    // only on the RPi B+/A+/2

int main() {                             // must be run as root
   wiringPiSetupPhys();                  // use the physical pin numbers
   pinMode(PWM0, PWM_OUTPUT);            // use the RPi PWM output
   pinMode(PWM1, PWM_OUTPUT);            // only on recent RPis

   // Setting PWM frequency to be 10kHz with a full range of 128 steps
   // PWM frequency = 19.2 MHz / (divisor * range)
   // 10000 = 19200000 / (divisor * 128) => divisor = 15.0 = 15
   pwmSetMode(PWM_MODE_MS);              // use a fixed frequency
   pwmSetRange(128);                     // range is 0-128
   pwmSetClock(15);                      // gives a precise 10kHz signal
   cout << "The PWM Output is enabled" << endl;
   pwmWrite(PWM0, 32);                   // duty cycle of 25% (32/128)
   pwmWrite(PWM1, 64);                   // duty cycle of 50% (64/128)
   return 0;                             // PWM output stays on after exit
}
