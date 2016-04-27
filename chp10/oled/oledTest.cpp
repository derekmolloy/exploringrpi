#include "ArduiPi_OLED_lib.h"
#include "ArduiPi_OLED.h"
#include "Adafruit_GFX.h"
#include <stdio.h>
#include <ctime>

int main(){
   ArduiPi_OLED display;
   if(!display.init(OLED_I2C_RESET, OLED_ADAFRUIT_I2C_128x64)){
      perror("Failed to set up the display\n");
      return -1;
   }
   printf("Setting up the I2C Display output\n");
   display.begin();
   display.clearDisplay();
   display.setTextSize(1);
   display.setTextColor(WHITE);
   display.setCursor(27,5);
   display.print("Exploring RPi");
   time_t t = time(0);
   struct tm *now = localtime(&t);
   display.setCursor(35,18);
   display.printf("%2d/%2d/%2d", now->tm_mon, now->tm_mday, (now->tm_year+1900));
   display.setCursor(21,37);
   display.setTextSize(3);
   display.printf("%02d:%02d", now->tm_hour, now->tm_min );
   display.drawRect(16, 32, 96, 32, WHITE);
   display.display();
   display.close();
   printf("End of the I2C Display program\n");
   return 0;
}
