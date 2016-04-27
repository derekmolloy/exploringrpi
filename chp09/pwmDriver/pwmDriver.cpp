/*
 * pwmDriver.cpp  Created on: 17 May 2015
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

#include "pwmDriver.h"
#include <iostream>
#include <unistd.h>
#include <math.h>
#include <stdio.h>

using namespace std;

namespace exploringRPi {

//From Table 4 (7.3 Register Definitions) of the PCA9685 Data sheet
#define MODE1          0x00   //The Mode register 1 - sleep/wake
#define MODE2          0x01   //The Mode register 2 - open-drain/totem pole
#define SUBADR1        0x02   //The i2c subaddress 1
#define SUBADR2        0x03   //The i2c subaddress 2
#define SUBADR3        0x04   //The i2c subaddress 3
#define ALLCALLADR     0x05   //LED All Call i2c-bus address
#define LED0_ON_L      0x06   //LED0 output and brightness control byte 0
#define LED0_ON_H      0x07   //LED0 output and brightness control byte 1
#define LED0_OFF_L     0x08   //LED0 output and brightness control byte 0
#define LED0_OFF_H     0x09   //LED0 output and brightness control byte 1
//The next 15 entries are not required as we can determine the four bytes
//automatically by ofsetting from the LED0 entries (4 bytes for each output)
#define ALL_LED_ON_L   0xFA   //load the LEDn_ON registers, byte 0
#define ALL_LED_ON_H   0xFB   //load the LEDn_ON registers, byte 0
#define ALL_LED_OFF_L  0xFC   //load the LEDn_ON registers, byte 0
#define ALL_LED_OFF_H  0xFD   //load the LEDn_ON registers, byte 0
#define PRE_SCALE      0xFE   //prescaler for the PWM output frequency
//Testmode register not required (0xFF) as not for user use

#define DEFAULT_MODE1  0b10100001   //See pg. 14 of the datasheet -- use autoincrement
#define CALIB_FACTOR   0.89   //the calibration factor -- manually calculated

/**
 * Helper method to combine two 8-bit registers into a single short, which is 16-bits on the
 * Raspberry Pi. It shifts the MSB 8-bits to the left and then ORs the result with the LSB.
 * @param msb an unsigned character that contains the most significant byte
 * @param lsb an unsigned character that contains the least significant byte
 */
short PWMDriver::combineRegisters(unsigned char msb, unsigned char lsb){
   //shift the MSB left by 8 bits and OR with LSB
   return ((short)msb<<8)|(short)lsb;
}

/**
 * The constructor for the PWMDriver object. It passes the bus number and the device address
 * (which is 0x40 by default) to the constructor of I2CDevice. All of the states
 * are initialized and the registers are updated.
 * @param I2CBus The bus number that the PCA9685 device is on - defaults to 1
 * @param I2CAddress The address of the PCA9685 device (default 0x40, but can be altered)
 */
PWMDriver::PWMDriver(unsigned int I2CBus, unsigned int I2CAddress):
   I2CDevice(I2CBus, I2CAddress){   // this member initialisation list calls the parent constructor
   this->I2CAddress = I2CAddress;
   this->I2CBus = I2CBus;
   this->setFrequency(50.0);
}

/**
 * This method resets the outputs clearing the SLEEP bit (bit 4) and the RESTART bit (bit 7)
 * on the MODE1 register (0x00). It uses the DEFAULT_MODE as defined above which uses the
 * internal oscillator only and enables register auto-increment addressing.
 * @return 0 if successful
 */
int PWMDriver::reset(){
   this->open();
   int status = this->writeRegister(MODE1, DEFAULT_MODE1);
   this->close();
   return status;
}

/**
 * This method sets the SLEEP bit (bit 4) on the MODE1 register, disabling the outputs. This allows
 * you to remove power from motors and LEDs and does not affect the present PWM settings of each
 * of the individual outputs. Use the wake()/reset() method to return power to the outputs.
 * @return 0 if successful
 */
int PWMDriver::sleep(){
   this->open();
   unsigned char mode1_sleep_state = this->readRegister(MODE1);
   int status = this->writeRegister(MODE1, (mode1_sleep_state|(1<<4)));
   this->close();
   return status;
}

/* The setOutput method allow you to set individual output PWM cycles for each of the outputs
 * on the PCA9685 breakout board.
 *
 * There are two 12-bit registers for each output (i.e., each stores 0-4095). One stores
 * the on time and the other stores the off time. Having an on and off time allows a phase
 * shift to be introduced. Important! The "on time" is the time after which the output turns
 * on and the "off time" is the time after which the output turns off. It is NOT the time
 * for which the output is on and the time for which the output is off. See Fig 7 on pg. 17
 * of the PCA9685 datasheet.
 * @param outputNumber The output number (a value between 0 and 15)
 * @param dutyCycle The positive duty cycle percentage (0.0 to 100.0)
 * @param phaseOffset If a cycle offset is required as a percentage of a cycle (default=0.0)
 */
int PWMDriver::setOutput(unsigned int outputNumber, float dutyCycle, float phaseOffset){
// cout << "Writing Duty cycle: " << dutyCycle << " to output " << outputNumber << endl;
   if((dutyCycle < 0.0f)||(dutyCycle > 100.0f)){
      perror("PCA9685: The duty cycle is outside range");
      return -1;
   }
   if((phaseOffset < 0.0f)||(phaseOffset > 100.0f)){
      perror("PCA9685: The phase offset is outside range");
      return -1;
   }
   if((outputNumber < 0)||(outputNumber > 15)){
      perror("PCA9685: Attempt to access invalid output");
      return -1;
   }
   unsigned int timeOn, timeOff;
   // There are special register states for fully on and fully off -- see Table 7.
   if (dutyCycle==100.0f){
      timeOn = 4096;
      timeOff = 0;
   }
   else if (dutyCycle==0.0f){
      timeOn = 0;
      timeOff = 4096;
   }
   else {  // Regular operation (i.e., >0.0f and <100.0f)
      timeOn  = (unsigned int)((4095 * phaseOffset) / 100.0f);
      timeOff = timeOn + (unsigned int)((4095 * dutyCycle)/ 100.0f);
      if (timeOff>4095){   // see Example 2 on pg. 17 of the datasheet
         timeOff = timeOff - 4096;
      }
   }
   this->open();
   int result = this->writeRegister((LED0_ON_L + (4*outputNumber)), (timeOn&0xFF));
// cout << "Writing to register " << LED0_ON_L + (4*outputNumber) << endl;
// cout << "Writing value " << (timeOn & 0xFF) << endl;
   if (result!=0){
      perror("PCA9685: Failed to write to the register");
      return -1;
   }
   this->writeRegister((LED0_ON_H  + (4*outputNumber)), (timeOn  >> 8));
   this->writeRegister((LED0_OFF_L + (4*outputNumber)), (timeOff & 0xFF));
   this->writeRegister((LED0_OFF_H + (4*outputNumber)), (timeOff >> 8));
   this->close();
   return 0;
}


/**
 * The setFrequency function. See Section 7.3.5 in the PCA9685 datasheet. The PRE_SCALE
 * register defines the frequency at which the outputs modulate. This is determined with
 * the formula: val = round(osc_clock / (4096 x update_rate)) - 1 and default frequency
 * of 200Hz
 * With internal clock: val = round(25Mhz / 4096 x 200) - 1 = 30 (ie., 0x1E)
 * Max frequency is 1526Hz = 0x03 and min frequency is 24Hz = 0xFF.
 * Calibration at 200Hz get 221.45Hz, 25Hz get 28.135Hz, 500Hz get 572Hz
 * Least-squares fit ~= 0.89x
 */
int PWMDriver::setFrequency(float frequency){
   // check frequency range -- see pg. 2 of the PCA9685 datasheet
   if((frequency < 24.0f)||(frequency > 1526.0f)){
      perror("PCA9685: Frequency is outside of the permitted range");
      return -1;
   }
   frequency = (CALIB_FACTOR * frequency); // Manually calibrated using a scope
   unsigned int prescaleVal = ((unsigned int) roundf(25000000.0f/(4096.0f*frequency))) - 1;
   //Need to set SLEEP bit in MODE1 to 1 to set the frequency (see note [1] pg. 13)
   if (prescaleVal<0x03) { prescaleVal = 0x03; } // The calibration can go outside range -- limit
   if (prescaleVal>0xFF) { prescaleVal = 0xFF; } // limit on the upside just in case
   this->sleep();
   this->open();
   this->writeRegister(PRE_SCALE, prescaleVal);
   this->close();
   this->wake();
   return 0;
}

/**
 * This method gets the frequency that is currently stored in the PCA9685 device
 * @return returns the frequency that is stored (between 24Hz and 1526Hz)
 */
float PWMDriver::getFrequency(){
   this->open();
   int value = (int) this->readRegister(PRE_SCALE);
   this->close();
   float frequency = (25000000.0f / (4096.0f * (value + 1)));
   return (frequency / CALIB_FACTOR);
}

PWMDriver::~PWMDriver() {}

} /* namespace exploringRPi */
