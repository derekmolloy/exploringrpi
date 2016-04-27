/*
 * DACDriver.cpp  Created on: 17 May 2015
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

#include "DACDriver.h"
#include <iostream>
#include <unistd.h>
#include <math.h>
#include <stdio.h>

using namespace std;

namespace exploringRPi {

//From Table 6-2 of the MCP4725 Data sheet

/**
 * The constructor for the DACDriver object. It passes the bus number and the device address
 * (which is 0x62 by default) to the constructor of I2CDevice. All of the states
 * are initialized and the registers are updated.
 * @param I2CBus The bus number that the MCP4725 device is on - defaults to 1
 * @param I2CAddress The address of the MCP4725 device (default 0x62)
 */
DACDriver::DACDriver(unsigned int I2CBus, unsigned int I2CAddress):
   I2CDevice(I2CBus, I2CAddress){   // this member initialisation list calls the parent constructor
   this->I2CAddress = I2CAddress;
   this->I2CBus = I2CBus;
}

/**
 * Use the Power Down Modes (11,10,01) to set the equivalent GND resistor values. See the
 * header file for the values to use. This function calls the private setOutput() function.
 * @return 0 if successful
 */
int DACDriver::powerDown(DACDriver::PD_MODE mode){
   return setOutput(lastValue, mode);
}

/**
 * Wakes the DAC by disabling the power down value
 * @return 0 if successful
 */
int DACDriver::wake(){
   return setOutput(lastValue, DISABLE);
}

/**
 * Set the output as a percentage of Vcc (typically 3.3V)
 * @param percentage the percentage value (0.0 to 100.0)
 * @return 0 if successful
 */
int DACDriver::setOutput(float percentage){
   if((percentage < 0.0f)||(percentage > 100.0f)){
      perror("PWMDriver: Invalid percentage value used");
      return -1;
   }
   return setOutput((unsigned int)roundf(40.95f*percentage));
}

/**
 * Set the output as a 12-bit value
 * @param output the 12-bit unsigned integer value (0-4095)
 * @return 0 if successful
 */
int DACDriver::setOutput(unsigned int output){
// cout << "Setting the DAC to have decimal value: " << output << endl;
   if((output < 0)||(output > 4095)){
      perror("MCP4725: The output is outside range");
      return -1;
   }
   return setOutput(output, DISABLE);
}

/**
 * This method does all the work, but is private in order to abstract it through the
 * methods that are defined above. Keeps the other code straightforward.
 * @param output the 12-bit unsigned integer, already validated
 * @param mode the Power Down mode -- DISABLE for regular operation
 * @return 0 if successful
 */
int DACDriver::setOutput(unsigned int output, DACDriver::PD_MODE mode){
   this->open();
   unsigned char msb = output >> 8;
   unsigned char lsb = output & 0xff;
   if(mode!=DISABLE){
      msb = msb | (mode<<4);     //In fast mode write, power down bits PD1, PD0 are bits 5,4
   }
   int result = this->writeRegister(msb, lsb);
   if (result!=0){
      perror("MCP4725: Failed to write to the device");
      return -1;
   }
// cout << "Wrote decimal equiv. values: " << (int)msb << "," << (int)lsb << endl;
   this->close();
   this->lastValue = output;
   return 0;
}

int DACDriver::setOutput(unsigned int waveform[], int size, int loops){
   //fast write -- no validation or error checking
   this->open();
   for(int loop=0; loop<loops; loop++){
      for(int i=0; i<size; i++){
         this->writeRegister(waveform[i] >> 8, waveform[i] & 0xff);
      }
   }
   this->close();
   return 0;
}

DACDriver::~DACDriver() {}

} /* namespace exploringRPi */
