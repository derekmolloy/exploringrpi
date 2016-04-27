/*
 * application.cpp  Created on: 29 Apr 2014
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
#include "pwmDriver.h"
#include <unistd.h>
#include <pthread.h>
#include <cstdio>

using namespace std;
using namespace exploringRPi;

int main() {
   PWMDriver driver(1, 0x40);
   driver.reset();
   driver.setFrequency(100.0f);
   float frequency = driver.getFrequency();
   cout << "The frequency is currently: " << frequency << endl;
   driver.setOutput(0, 12.5);
   driver.setOutput(1, 25.0, 12.5);
   cout << "Press Enter to sleep the outputs..." << endl;
   getchar();
   driver.sleep();
   cout << "The outputs are now off" << endl;
   cout << "Press Enter to wake the outputs..." << endl;
   getchar();
   driver.wake();
   cout << "The outputs are now on" << endl;
   return 0;
}
