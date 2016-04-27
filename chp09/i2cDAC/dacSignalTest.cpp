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
#include <math.h>
#include "DACDriver.h"

using namespace std;
using namespace exploringRPi;

int main() {
   unsigned int signal = 0;

   // Generate a periodic sine wave in an array of 100 values - using 100 ints
   unsigned int sineWaveform[100];
   float gain  = 2047.0f;
   float phase = 0.0f;
   float bias  = 2048.0f;
   float freq  = 2.0f * 3.14159f / 100.0f;
   for (int i=0; i<100; i++){
      sineWaveform[i] = (unsigned int)(bias + (gain * sin((i * freq) + phase)));
   }
   unsigned int sawWaveform[2] = {0, 4095};
   cout << "A sine wave (100 samples biased by +50% Vmax)" << endl;
   DACDriver *driver = new DACDriver(1,0x62);
   driver->setOutput(sineWaveform, 100, 500);
   cout << "Press ENTER for the next waveform..." << endl;
   getchar();
   driver->setOutput(sawWaveform, 2, 50000);
   return 0;
}
