#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/mman.h>
#include <stdint.h>   // for uint32_t - 32-bit unsigned integer

// GPIO_BASE is 0x20000000 on RPi models other than the RPi 2
#define GPIO_BASE    0x3F200000   // on the RPi 2
#define GPSET0       0x1c         // from Figure 6-X
#define GPCLR0       0x28
#define GPLVL0       0x34
static volatile uint32_t *gpio;   // pointer to the gpio (*int)

int main() {
   int fd, x;
   printf("Start of GPIO memory-manipulation test program.\n");
   if(getuid()!=0) {
      printf("You must run this program as root. Exiting.\n");
      return -EPERM;
   }
   if ((fd = open("/dev/mem", O_RDWR | O_SYNC)) < 0) {
      printf("Unable to open /dev/mem: %s\n", strerror(errno));
      return -EBUSY;
   }
   // get a pointer that points to the peripheral base for the GPIOs
   gpio = (uint32_t *) mmap(0, getpagesize(), PROT_READ|PROT_WRITE,
          MAP_SHARED, fd, GPIO_BASE);
   if ((int32_t) gpio < 0) {
      printf("Memory mapping failed: %s\n", strerror(errno));
      return -EBUSY;
   }
   // at this point gpio points to the GPIO peripheral base address
   // set up the LED GPIO FSEL17 mode = 001 at addr GPFSEL1 (0004)
   // remember that adding one 32-bit value moves the addr by 4 bytes
   // writing NOT 7 (i.e., ~111) clears bits 21, 22 and 23.
   *(gpio + 1) = (*(gpio + 1) & ~(7 << 21) | (1 << 21));
   // set up the button GPIO FSEL27 mode = 000 at addr GPFSEL2 (0008)
   // both FSEL17 and FSEL27 are 21 bits in, but on different registers
   *(gpio + 2) = (*(gpio + 2) & ~(7 << 21) | (0 << 21));
   // writing the 000 is not necessary but is there for clarity

   do {
      // turn the LED on using the bit 17 on the GPSET0 register
      *(gpio + (GPSET0/4)) = 1 << 17;
//    usleep(10);          // cannot use sleep as it is non-blocking
      for(x=0;x<50;x++){}  // blocking delay hack using a simple loop
      *(gpio + (GPCLR0/4)) = 1 << 17;  // turn the LED off
      for(x=0;x<49;x++){}  // delay hack -- balanced for while()
   }
   while((*(gpio+(GPLVL0/4))&(1<<27))==0); // only true if bit 27 high

   printf("Button was pressed - end of example program.\n");
   close(fd);
   return 0;
}
