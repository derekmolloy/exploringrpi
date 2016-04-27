// Based on the spi.c example at www.airspayce.com/mikem/bcm2835/
#include <bcm2835.h>
#include <iostream>
#include <string.h>
#include <sys/mman.h>
#define SAMPLES 2000
using namespace std;

int main() {
   short data[SAMPLES];
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
   for(int i=0; i<SAMPLES; i++) {
      char msg[3] = { 0b00000110, 0x00, 0x00 };
      for(int x=0; x<700; x++) { };   // hacked delay - do not optimize
      bcm2835_spi_transfern(msg, 3);
      data[i]=((msg[1]&0b00001111)<<8)|msg[2];
   }
   for(int i=0; i<SAMPLES; i++) {
      cout << i << " " << data[i] << endl;
   }
   bcm2835_spi_end();       // clean up SPI
   bcm2835_close();         // close the driver
   munlockall();            // unlock the process memory
   return 0;
}
