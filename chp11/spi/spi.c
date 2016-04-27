#include <stdio.h>
#include <wiringPiSPI.h>

int main() {
   char data[2] = {0, 99};
   wiringPiSPISetupMode(0, 1000000, 0);
   wiringPiSPIDataRW (0, data, 2) ;
   printf("Transaction complete...\n");
   return 0;
}
