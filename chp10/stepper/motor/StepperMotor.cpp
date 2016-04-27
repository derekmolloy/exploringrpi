#include "StepperMotor.h"
#include<iostream>
#include<unistd.h>
#include<cmath>
using namespace std;

namespace exploringRPi {

StepperMotor::StepperMotor(GPIO *ms1, GPIO *ms2, GPIO *step, GPIO *sleep,
		 GPIO *dir, int speedRPM, int stepsPerRev){
	this->gpio_MS1  = ms1;
	this->gpio_MS2  = ms2;
	this->gpio_STEP = step;
	this->gpio_SLP  = sleep;
	this->gpio_DIR  = dir;
	// the default speed in rpm
	this->setSpeed(speedRPM);
	this->stepsPerRev = stepsPerRev;
	this->init(speedRPM, stepsPerRev);
}

StepperMotor::StepperMotor(int ms1, int ms2, int step, int sleep,
 			int dir, int speedRPM, int stepsPerRev){
	this->gpio_MS1 = new GPIO(ms1);
	this->gpio_MS2 = new GPIO(ms2);
	this->gpio_STEP = new GPIO(step);
	this->gpio_SLP = new GPIO(sleep);
	this->gpio_DIR = new GPIO(dir);
	this->gpio_MS1->setDirection(OUTPUT);
	this->gpio_MS2->setDirection(OUTPUT);
	this->gpio_STEP->setDirection(OUTPUT);
	this->gpio_SLP->setDirection(OUTPUT);
	this->gpio_DIR->setDirection(OUTPUT);
	this->init(speedRPM, stepsPerRev);
}

void StepperMotor::init(int speedRPM, int stepsPerRev){
	this->gpio_MS1->setDirection(OUTPUT);
	this->gpio_MS2->setDirection(OUTPUT);
	this->gpio_STEP->setDirection(OUTPUT);
	this->gpio_SLP->setDirection(OUTPUT);
	this->gpio_DIR->setDirection(OUTPUT);
	this->threadRunning = false;

	this->stepsPerRev = stepsPerRev;
	// default to clockwise direction
	this->setDirection(StepperMotor::CLOCKWISE);
	// default to full stepping
	setStepMode(STEP_FULL);
	// the default number of steps per revolution
	setStepsPerRevolution(stepsPerRev);
	// the default speed in rpm
	this->setSpeed(speedRPM);
	//wake up the controller - holding torque..
	this->wake();
}

void StepperMotor::setStepMode(STEP_MODE mode) {
	this->stepMode = mode;
	switch(stepMode){
	case STEP_FULL:
		this->gpio_MS1->setValue(LOW);
		this->gpio_MS2->setValue(LOW);
		this->delayFactor = 1;
		break;
	case STEP_HALF:
		this->gpio_MS1->setValue(HIGH);
		this->gpio_MS2->setValue(LOW);
		this->delayFactor = 2;
		break;
	case STEP_QUARTER:
		this->gpio_MS1->setValue(LOW);
		this->gpio_MS2->setValue(HIGH);
		this->delayFactor = 4;
		break;
	case STEP_EIGHT:
		this->gpio_MS1->setValue(HIGH);
		this->gpio_MS2->setValue(HIGH);
		this->delayFactor = 8;
		break;
	}
}

void StepperMotor::setSpeed(float rpm) {
	this->speed = rpm;
	float delayPerSec = (60/rpm)/stepsPerRev;    // delay per step in seconds
	this->uSecDelay = (int)(delayPerSec * 1000 * 1000); // in microseconds
}

void StepperMotor::step(int numberOfSteps){
	//cout << "Doing "<< numberOfSteps << " steps and going to sleep for " << uSecDelay/delayFactor << "uS\n";
	int sleepDelay = uSecDelay/delayFactor;
	if(numberOfSteps<0) {
		this->reverseDirection();
		numberOfSteps = -numberOfSteps;
	}
	for(int i=0; i<numberOfSteps; i++){
		this->step();
		usleep(sleepDelay);
	}
}

void StepperMotor::step(){
    this->gpio_STEP->setValue(LOW);
    this->gpio_STEP->setValue(HIGH);
}


int  StepperMotor::threadedStepForDuration(int numOfSteps, int dur_ms){
   this->threadedStepNumber = numOfSteps;
   this->threadedStepPeriod = dur_ms/numOfSteps;
   this->threadRunning = true;
   if(pthread_create(&this->thread, NULL, &threadedStep, static_cast<void*>(this))){
      perror("StepperMotor: Failed to create the stepping thread");
      this->threadRunning = false;
      return -1;
   }
   return 0;
}

void StepperMotor::setDirection(DIRECTION direction){
   this->direction = direction;
   if(this->direction==CLOCKWISE) this->gpio_DIR->setValue(HIGH);
   else this->gpio_DIR->setValue(LOW);
}

void StepperMotor::reverseDirection(){
   if(this->direction==CLOCKWISE){
 	this->setDirection(COUNTERCLOCKWISE);
   }
   else this->setDirection(CLOCKWISE);
}

void StepperMotor::rotate(float degrees){
	float degreesPerStep = 360.0f/getStepsPerRevolution();
	int numberOfSteps = floor(((this->delayFactor*degrees)/degreesPerStep)+0.5);
	//cout << "The number of steps is " << numberOfSteps << endl;
	//cout << "The delay factor is " << delayFactor << endl;
	step(numberOfSteps);
}

void StepperMotor::sleep(){
	this->asleep = true;
	this->gpio_SLP->setValue(LOW);
}

void StepperMotor::wake(){
	this->asleep = false;
	this->gpio_SLP->setValue(HIGH);
}

StepperMotor::~StepperMotor() {}

// This thread function is a friend function of the class
void* threadedStep(void *value){
	StepperMotor *stepper = static_cast<StepperMotor*>(value);
	while(stepper->threadRunning){
		stepper->step();
		usleep(stepper->threadedStepPeriod * 1000);  // convert from ms to us
		if(stepper->threadedStepNumber>0) stepper->threadedStepNumber--;
		if(stepper->threadedStepNumber==0) stepper->threadRunning = false;
	}
	return 0;
}

} /* namespace exploringRPi */
