#include <iostream>
#include <unistd.h>
#include <wiringPi.h>
using namespace std;

#define APHASE         15                // physical pin for GPIO22
#define AENABLE_PWM1   33                // physical pin for PWM1
#define BPHASE          7                // physical pin for GPIO4
#define BENABLE_PWM0   12                // physical pin for PWM0

int main() {                             // must be run as root
   wiringPiSetupPhys();                  // use the physical pin numbers
   pinMode(APHASE, OUTPUT);              // controls direction
   pinMode(AENABLE_PWM1, PWM_OUTPUT);    // speed - only on RPi B+/A+/2
   pinMode(BPHASE, OUTPUT);              // controls direction
   pinMode(BENABLE_PWM0, PWM_OUTPUT);    // PWM output used for speed
   pwmSetMode(PWM_MODE_MS);              // use a fixed frequency
   pwmSetRange(128);                     // range is 0-128
   pwmSetClock(15);                      // gives a precise 10kHz signal

   cout << "Motor A: Rotate forward at 50% for 5 seconds" << endl;
   digitalWrite(APHASE, LOW);            // notional foward
   pwmWrite(AENABLE_PWM1, 64);           // duty cycle of 50% (64/128)
   usleep(5000000);
   cout << "Motor A: Rotate backwards at 100% for 5 seconds" << endl;
   digitalWrite(APHASE, HIGH);           // notional backwards
   pwmWrite(AENABLE_PWM1, 128);          // duty cycle of 100% (64/128)
   usleep(5000000);
   pwmWrite(AENABLE_PWM1, 0);            // Motor A off - duty cycle of 0%
   cout << "Motor B: Rotate forward at 75% for 5 seconds" << endl;
   digitalWrite(BPHASE, LOW);            // notional foward
   pwmWrite(BENABLE_PWM0, 96);           // duty cycle of 75% (96/128)
   usleep(5000000);
   cout << "Motor B: Rotate Backwards at 25% for 5 seconds" << endl;
   digitalWrite(BPHASE, HIGH);           // notional backward
   pwmWrite(BENABLE_PWM0, 32);           // duty cycle of 25% (32/128)
   usleep(5000000);
   cout << "End of Program turn off both motors" << endl;
   pwmWrite(BENABLE_PWM0, 0);            // Motor B off - duty cycle of 0%
   return 0;                             // would keep going after exit
}
