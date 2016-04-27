#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>
#include<termios.h>
#include<string.h>

int main(int argc, char *argv[]){
   int file, count;
   if(argc!=2){
       printf("Please pass a string to the program, exiting!\n");
       return -2;
   }
   // use /dev/ttyS0 for the RPi3 and /dev/ttyAMA0 for other models
   if ((file = open("/dev/ttyAMA0", O_RDWR | O_NOCTTY | O_NDELAY))<0){
      perror("UART: Failed to open the device.\n");
      return -1;
   }
   struct termios options;
   tcgetattr(file, &options);
   options.c_cflag = B115200 | CS8 | CREAD | CLOCAL;
   options.c_iflag = IGNPAR | ICRNL;
   tcflush(file, TCIFLUSH);
   tcsetattr(file, TCSANOW, &options);
   if ((count = write(file, argv[1], strlen(argv[1])))<0){
      perror("UART: Failed to write to the output\n");
      return -1;
   }
   write(file, "\n\r", 2);
   close(file);
   return 0;
}
