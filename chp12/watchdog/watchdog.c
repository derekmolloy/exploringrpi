#include<stdio.h>
#include<fcntl.h>
#include<linux/watchdog.h>
#define WATCHDOG "/dev/watchdog"

int main(){
   int fd, interval=15, state;
   printf("Please ensure that you have shut down the watchdog service:\n");
   printf("For example, sudo systemctl disable watchdog\n\n");
   if(getuid()!=0){
      printf("You must run this program as root. Exiting.\n");
      return 1;
   }
   if ((fd = open(WATCHDOG, O_RDWR))<0){
     perror("Watchdog: Failed to open watchdog device\n");
     return 1;
   }
   // set the timing interval to 15 seconds
   if (ioctl(fd, WDIOC_SETTIMEOUT, &interval)!=0){
     perror("Watchdog: Failed to set the watchdog interval\n");
     return 1;
   }
   printf("The RPi will now reboot after %d seconds\n", interval);
   printf("Press k <enter> to kick the dog to delay the reboot\n");
   printf("Press h <enter> to say hello (does not delay reboot)\n");
   do{
      state = getchar();
      switch(state){
      case 'k':
         printf("[kick!]");
         ioctl(fd, WDIOC_KEEPALIVE, NULL);
         break;
      case 'h':
         printf("[hello]");
         break;
      }
   } while (state!='q');
   printf("Closing down the application\n");
   close(fd);
   return 0;
}
