/*
 * dacDriver.h  Created on: 17 June 2015
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
 * This code is built using the datasheet for the MCP4725
 *  http://ww1.microchip.com/downloads/en/DeviceDoc/22039d.pdf
 */

#ifndef DACDriver_H_
#define DACDriver_H_
#include"I2CDevice.h"

namespace exploringRPi {

/**
 * @class DACDriver
 * @brief Specific class for the MCP4725 DAC driver that is a child of the I2CDevice class
 */
class DACDriver:protected I2CDevice{
public:
   enum PD_MODE { DISABLE, GND_1K, GND_100K, GND_500K };

private:
   unsigned int I2CBus, I2CAddress;
   unsigned int lastValue;
   int setOutput(unsigned int value, DACDriver::PD_MODE mode);

public:
   DACDriver(unsigned int I2CBus=1, unsigned int I2CAddress=0x62);
   virtual int powerDown(DACDriver::PD_MODE mode = GND_500K);
   virtual int wake();
   virtual int setOutput(unsigned int value);
   virtual int setOutput(float percentage);
   virtual int setOutput(unsigned int waveform[], int size, int loops=1);
   virtual unsigned int getLastValue() { return lastValue; }
   virtual ~DACDriver();
};

} /* namespace exploringRPi */

#endif /* DACDriver_H_ */
