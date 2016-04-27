/*
 * Servo.cpp  Created on: 17 May 2015
 * Copyright (c) 2015 Derek Molloy (www.derekmolloy.ie)
 * Made available for the book "Exploring Raspberry Pi"
 * See: www.exploringrpi.com
 * Licensed under the EUPL V.1.1
 *
 * This Software is provided to You under the terms of the European
 * Union Public License (the "EUPL") version 1.1 as published by the
 * European Union. Any use of this Software, other than as authorized
 * under this License is strictly prohibited (to the extent such use
 * is covered by a right of the copyright holder of this Software).
 *
 * This Software is provided under the License on an "AS IS" basis and
 * without warranties of any kind concerning the Software, including
 * without limitation merchantability, fitness for a particular purpose,
 * absence of defects or errors, accuracy, and non-infringement of
 * intellectual property rights other than copyright. This disclaimer
 * of warranty is an essential part of the License and a condition for
 * the grant of any rights to this Software.
 *
 * For more details, see http://www.derekmolloy.ie/
 */

#include "Servo.h"
#include <iostream>
#include <unistd.h>
#include <math.h>
#include <stdio.h>

using namespace std;

namespace exploringRPi {

/**
 * The constructor for the Servo class. This constructor requires a PCA9685 device, an
 * output number (0-15), and an angle range for the physical servo motor (e.g., +/-90)
 * @param pwmDriver a pointer to the PCA9685 driver
 * @param outputNumber the output number on the PCA9685 driver board (0-15)
 * @param plusMinusRange the angle range of the servo motor -- e.g., 90 = +/-90
 */
Servo::Servo(PWMDriver *pwmDriver, int outputNum, float plusMinusRange):
   pwmDriver(pwmDriver), outputNumber(outputNum), plusMinusRange(plusMinusRange){
   this->calibrate(2.85f, 11.75f); // Typical values
}

/**
 * A calibrate function that allows you to set the full useful range for the servo motor
 * using the duty cycle values. This allows you to calibrate the motor using a linear scaling
 * range.
 * @param minDutyCycle the minimum duty cycle that represents the -range value
 * @param maxDutyCycle the maximum duty cycle that represents the +range value
 * @return return 0 if successful
 */
int Servo::calibrate(float minDutyCycle, float maxDutyCycle){
   if(maxDutyCycle<=minDutyCycle){
      perror("Servo: Error, there is something wrong with the duty cycle values.");
      return -1;
   }
   this->minDutyCycle = minDutyCycle;
   this->maxDutyCycle = maxDutyCycle;
   this->zeroDutyCycle = (minDutyCycle + maxDutyCycle)/2.0f;
   this->angleStepSize = (maxDutyCycle - minDutyCycle) / (2 * plusMinusRange);
   return 0;
}

/**
 * Set the servo angle using +/-angle value (zero is the center)
 * @param angle the servo angle (value must be within +/- range)
 * @return return 0 if successful
 */
int Servo::setAngle(float angle){
   if((angle < (-plusMinusRange))||(angle > (+plusMinusRange))){
      perror("Servo: Error, the angle selected is outsid of the servo operation range.");
      return -1;
   }
   float angleDutyCycle = zeroDutyCycle + (angle * angleStepSize);
   this->pwmDriver->setOutput(outputNumber, angleDutyCycle);
   return 0;
}

Servo::~Servo() {}

} /* namespace exploringRPi */
