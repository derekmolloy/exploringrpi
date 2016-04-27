/*
 * pwmDriver.h  Created on: 17 June 2015
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
 *
 * This code is built using the datasheet for the PCA9685, see:
 *   http://www.nxp.com/documents/data_sheet/PCA9685.pdf
 * This code does NOT support an external clock for the PCA9685.
 */

#ifndef PWMDriver_H_
#define PWMDriver_H_
#include"I2CDevice.h"

/// The PCA9685 has 0xFF registers (0x46 to 0xF9 are reserved and should not be accessed)
#define BUFFER_SIZE 0xFF

namespace exploringRPi {

/**
 * @class PWMDriver
 * @brief Specific class for the PCA9685 pwm driver that is a child of the I2CDevice class
 * Protected inheritance means that the public I2CDevice methods are not publicly accessible
 * by an object of the PWMDriver class.
 */
class PWMDriver:protected I2CDevice{

private:
   unsigned int I2CBus, I2CAddress;
   short combineRegisters(unsigned char msb, unsigned char lsb);  // currently unused

public:
   PWMDriver(unsigned int I2CBus=1, unsigned int I2CAddress=0x40);
   virtual int reset();
   virtual int sleep();
   virtual int wake() { reset(); }

   virtual int setOutput(unsigned int outputNumber, float dutyCycle, float phaseOffset=0.0f); // 0-15, 0.0-100.0, 0.0f
   virtual int setOutputFullyOn(unsigned int outputNumber) { setOutput(outputNumber, 100.0f); }
   virtual int setOutputFullyOff(unsigned int outputNumber){ setOutput(outputNumber, 0.0f);   }

   virtual int setFrequency(float frequency);       // a value between 24.0 and 1526.0 (Hz)
   virtual float getFrequency();

   virtual ~PWMDriver();
};

} /* namespace exploringRPi */

#endif /* PWMDriver_H_ */
