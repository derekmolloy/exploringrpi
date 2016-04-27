/*
 * Servo.h  Created on: 17 June 2015
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

#ifndef Servo_H_
#define Servo_H_
#include"pwmDriver.h"

namespace exploringRPi {

/**
 * @class Servo
 * @brief A class that allows you to control a Servo motor object using the PWM Driver
 */
class Servo {

private:
   PWMDriver *pwmDriver;                             ///< The pointer to the PCA9685 driver
   int outputNumber;                                 ///< The output on the PCA9685 breakout
   float minDutyCycle, maxDutyCycle, zeroDutyCycle;  ///< The calibrated duty cycles
   float plusMinusRange;                             ///< The range of the servo (e.g., +/- 90)
   float angleStepSize;                              ///< Calculated from other duty cycles and range

public:
   Servo(PWMDriver *pwmDriver, int outputNum, float plusMinusRange=90.0f);
   virtual int calibrate(float minDutyCycle, float maxDutyCycle);
   virtual int setAngle(float angle);
   virtual ~Servo();
};

} /* namespace exploringRPi */

#endif /* Servo_H_ */
