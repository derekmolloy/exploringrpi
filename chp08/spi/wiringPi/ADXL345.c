#include<wiringPiSPI.h>
#include<stdio.h>
#include<string.h>

int main(){
   unsigned char data[0x41];    // a buffer to store the write/read data
   int i;
   memset(data, 0x00, 0x41);    // clear the full memory buffer
   data[0]=0xC0;                         // continuous read of the data
   wiringPiSPISetupMode(0, 1000000, 3);  // SPI channel, speed, mode
   wiringPiSPIDataRW(0, data, 0x40);     // read all 0x40 registers

   // Shift the data back by one address for the ADXL345
   memmove(data, data+1, 0x40);
   printf("The DEVID is %d\n", data[0x00]); // display register 0x00
   printf("The full set of 0x40 registers are:\n");
   for(i=0; i<0x40; i++){                // display all 0x40 registers
      printf("%02X ", data[i]);          // display value in hexadecmial
      if(i%16==15) printf("\n");         // place \n after each 15th value
   }
   return 0;
}
