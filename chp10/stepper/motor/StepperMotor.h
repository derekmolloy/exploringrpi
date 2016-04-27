#ifndef STEPPERMOTOR_H_
#define STEPPERMOTOR_H_
#include "GPIO.h"

namespace exploringRPi {

/**
 * @class StepperMotor
 * @brief A class to control a stepper motor using a motor driver board, such as the
 * Easy Driver board, or compatible. The class uses five GPIOs to control each motor.
 */
class StepperMotor {

public:
	enum STEP_MODE { STEP_FULL, STEP_HALF, STEP_QUARTER, STEP_EIGHT };
	enum DIRECTION { CLOCKWISE, COUNTERCLOCKWISE };

private:
	// The GPIO pins MS1, MS2 (Microstepping options), STEP (The low->high step)
	// SLP (Sleep - active low) and DIR (Direction)
   GPIO *gpio_MS1, *gpio_MS2, *gpio_STEP, *gpio_SLP, *gpio_DIR;
   unsigned int uSecDelay;
   DIRECTION direction;
   int delayFactor;      // keep constant rpm even with microstepping
   STEP_MODE stepMode;
   float speed;
   int stepsPerRev;
   bool asleep;
   void init(int speedRPM, int stepsPerRevolution);

public:
   StepperMotor(GPIO *ms1, GPIO *ms2, GPIO *step, GPIO *sleep,
                GPIO *dir, int speedRPM = 60, int stepsPerRev=200);
   StepperMotor(int ms1, int ms2, int step, int sleep,
    		int dir, int speedRPM = 60, int stepsPerRev=200);

   virtual void  step();
   virtual void  step(int numberOfSteps);
   virtual int   threadedStepForDuration(int numOfSteps, int dur_ms);
   virtual void  threadedStepCancel() { this->threadRunning = false; }
   virtual void  rotate(float degrees);
   virtual void  setDirection(DIRECTION direction);
   virtual DIRECTION getDirection() { return this->direction; }
   virtual void  reverseDirection();
   virtual void  setStepMode(STEP_MODE mode);
   virtual STEP_MODE getStepMode() { return stepMode; }
   virtual void  setSpeed(float rpm);
   virtual float getSpeed() { return speed; }
   virtual void  setStepsPerRevolution(int steps) { stepsPerRev = steps; }
   virtual int   getStepsPerRevolution() { return stepsPerRev; }
   virtual void  sleep();
   virtual void  wake();
   virtual bool  isAsleep() { return asleep; }
   virtual ~StepperMotor();

private:
   bool threadRunning;
   pthread_t thread;
   CallbackType callbackFunction;
   int threadedStepPeriod, threadedStepNumber;
   friend void* threadedStep(void *value);
};

void* threadedStep(void *value);

} /* namespace exploringRPi */

#endif /* STEPPERMOTOR_H_ */
