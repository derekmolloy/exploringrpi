#include <iostream>
#include <unistd.h>
#include "motor/StepperMotor.h"
using namespace std;
using namespace exploringRPi;

int main(){
   cout << "Starting RPi Stepper Motor Example:" << endl;
   //Using 5 GPIOs, RPM=60 and 200 steps per revolution
   //MS1=17, MS2=24, STEP=27, SLP=4, DIR=22
   StepperMotor m(17,24,27,4,22,60,200);
   m.setDirection(StepperMotor::COUNTERCLOCKWISE);
   m.setStepMode(StepperMotor::STEP_FULL);
   m.setSpeed(100);  //rpm
   cout << "Rotating 10 times at 100 rpm anti-clockwise, full step..." << endl;
   m.rotate(3600.0f);   //in degrees
   cout << "Finished regular (non-threaded) rotation)" << endl;
   m.setDirection(StepperMotor::CLOCKWISE);
   cout << "Performing 1 threaded revolution in 5 seconds using micro-stepping:" << endl;
   m.setStepMode(StepperMotor::STEP_EIGHT);
   if(m.threadedStepForDuration(1600, 5000)<0){
      cout << "Failed to start the Stepper Thread" << endl;
   }
   cout << "Thread should now be running..." << endl;
      for(int i=0; i<10; i++){ // sleep for 10 seconds.
      usleep(1000000);
      cout << i+1 << " seconds has passed..." << endl;
   }
   m.sleep();   // cut power to the stepper motor
   cout << "End of Stepper Motor Example" << endl;
}
