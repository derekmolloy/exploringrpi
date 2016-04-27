#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>
#include<termios.h>
#include<string.h>
#include<stdlib.h>
#include<wiringPi.h>
#define  LED_GPIO    17

// Sends a message to the client and displays the message on the console
int message(int client, char *message){
   int size = strlen(message);
   printf("Server>>>%s\n", (message+1));   // print message with new line
   if (write(client, message, size)<0){
      perror("Error: Failed to write to the client\n");
      return -1;
   }
   write(client, "\n\rERPi>", 7);          // display a simple prompt
   return 0;                               // \r for a carriage return
}

// Checks to see if the command is one that is understood by the server
int processCommand(int client, char *command){
   int val = -1;
   if (strcmp(command, "LED on")==0){
      val = message(client, "\r[Turning the LED on]");
      digitalWrite(LED_GPIO, HIGH);        // turn the physical LED on
   }
   else if(strcmp(command, "LED off")==0){
      val = message(client, "\r[Turning the LED off]");
      digitalWrite(LED_GPIO, LOW);         // turn the physical LED off
   }
   else if(strcmp(command, "quit")==0){    // shutting down server!
      val = message(client, "\r[goodbye]");
   }
   else { val = message(client, "\r[Unknown command]"); }
   return val;
}

int main(int argc, char *argv[]){
   int client, count=0;
   unsigned char c;
   char *command = malloc(255);
   wiringPiSetupGpio();                    // initialize wiringPi
   pinMode(LED_GPIO, OUTPUT);              // the LED is an output

   if ((client = open("/dev/ttyAMA0", O_RDWR | O_NOCTTY | O_NDELAY))<0){
      perror("UART: Failed to open the file.\n");
      return -1;
   }
   struct termios options;
   tcgetattr(client, &options);
   options.c_cflag = B115200 | CS8 | CREAD | CLOCAL;
   options.c_iflag = IGNPAR | ICRNL;
   tcflush(client, TCIFLUSH);
   fcntl(STDIN_FILENO, F_SETFL, O_NONBLOCK);  // make reads non-blocking
   tcsetattr(client, TCSANOW, &options);
   if (message(client, "\n\rRPi Serial Server running")<0){
      perror("UART: Failed to start server.\n");
      return -1;
   }

   // Loop forever until the quit command is sent from the client or
   //  Ctrl-C is pressed in the server's terminal window
   do {
      if(read(client,&c,1)>0){
          write(STDOUT_FILENO,&c,1);
          command[count++]=c;
          if(c=='\n'){
             command[count-1]='\0';  // replace /n with /0
             processCommand(client, command);
             count=0;                // reset the command string
          }
      }
      if(read(STDIN_FILENO,&c,1)>0){ // can send from stdin to client
          write(client,&c,1);
      }
   }
   while(strcmp(command,"quit")!=0);
   close(client);
   return 0;
}
