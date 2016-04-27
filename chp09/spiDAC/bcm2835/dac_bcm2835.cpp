// Based on the spi.c example at www.airspayce.com/mikem/bcm2835/
#include <bcm2835.h>
#include <iostream>
#include <string.h>
#include <math.h>
#include <sys/mman.h>
#define SAMPLES 2000
using namespace std;

int main() {
   // mask = (MSB) 0 (BUF) 0 (GA) 1 (SHDN) 1
   char mask = 0b00110000;
   if (!bcm2835_init()) {
      cout << "Failed to intialize the bcm2835 module" << endl;;
      return 1;
   }
   // Set the maximum possible priority and switch from regular Linux
   // round-robin to FIFO fixed-priority scheduling.
   struct sched_param sp;
   sp.sched_priority = sched_get_priority_max(SCHED_FIFO);
   if (sched_setscheduler(0, SCHED_FIFO, &sp)<0) { // change scheduling
      cout << "Failed to switch from SCHED_RR to SCHED_FIFO" << endl;
      return 1;
   }
   // lock the process' memory into RAM, preventing page swapping.
   if (mlockall(MCL_CURRENT|MCL_FUTURE)<0) { // lock cur & future pages
      std::cout << "Failed to lock the memory." << std::endl;
      return 1;
   }
   bcm2835_spi_begin();
   bcm2835_spi_setBitOrder(BCM2835_SPI_BIT_ORDER_MSBFIRST);
   bcm2835_spi_setDataMode(BCM2835_SPI_MODE3);
   bcm2835_spi_setClockDivider(BCM2835_SPI_CLOCK_DIVIDER_64); // limit!
   bcm2835_spi_chipSelect(BCM2835_SPI_CS0);
   bcm2835_spi_setChipSelectPolarity(BCM2835_SPI_CS0, LOW);

   // calculate a 12-bit sine wave using 100 samples per period
   unsigned short fn[100];                  // using 16-bit data
   float gain  = 2047.0f;                   // gain of 1.65V
   float phase = 0.0f;                      // phase not important here
   float bias  = 2048.0f;                   // center on 1.65V
   float freq  = 2.0f * 3.14159f / 100.0f;  // 2 * PI / period
   for (int i=0; i<100; i++){               // calculate sine waveform
      fn[i] = (unsigned short)(bias + (gain * sin((i * freq) + phase)));
   }
   char send[2];                            // sending 16-bits in total
   for(int x=0; x<10000; x++){              // send 10,000 periods
      for(int i=0; i<100; i++){             // 100 samples per period
         send[0] = mask | fn[i]>>8;         // first 4 bits as above
         send[1] = fn[i] & 0x00FF;          // remaining 8 lsbs of sample
         bcm2835_spi_transfern(send, 2);
      }
   }
   bcm2835_spi_end();       // clean up SPI
   bcm2835_close();         // close the driver
   munlockall();            // unlock the process memory
   return 0;
}
