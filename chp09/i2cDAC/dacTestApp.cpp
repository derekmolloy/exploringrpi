/*
 * servoTestApp.cpp  Created on: 29 Apr 2014
 * Copyright (c) 2014 Derek Molloy (www.derekmolloy.ie)
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

#include <iostream>
#include <unistd.h>
#include <pthread.h>
#include <cstdio>
#include "DACDriver.h"

using namespace std;
using namespace exploringRPi;

int main() {
   DACDriver *driver = new DACDriver(1,0x62);
   driver->setOutput(50.0f);
   cout << "The output is " << driver->getLastValue() << endl;
   cout << "Press ENTER to sleep the DAC..." << endl;
   getchar();
   driver->powerDown(DACDriver::GND_100K);  // leave blank for 500K
   cout << "Press ENTER to wake the DAC..." << endl;
   getchar();
   driver->wake();
   cout << "DAC is on and maintains value on exit" << endl;
   return 0;
}
