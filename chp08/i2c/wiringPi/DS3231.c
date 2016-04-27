#include<wiringPiI2C.h>
#include<stdio.h>

int main(){
   int fd = wiringPiI2CSetup(0x68);
   int secs  = wiringPiI2CReadReg8(fd, 0x00);
   int mins  = wiringPiI2CReadReg8(fd, 0x01);
   int hours = wiringPiI2CReadReg8(fd, 0x02);
   printf("The RTC time is %2d:%02d:%02d\n", hours, mins, secs);
   return 0;
}
