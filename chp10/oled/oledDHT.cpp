#include<iostream>
#include<unistd.h>
#include<iomanip>
#include "ArduiPi_OLED_lib.h"
#include "ArduiPi_OLED.h"
#include "Adafruit_GFX.h"
#include <bcm2835.h>
using namespace std;

#define USING_DHT11      false   // The DHT11 uses only 8 bits
#define DHT_GPIO         RPI_GPIO_P1_15
#define LH_THRESHOLD     22      // Low=~14, High=~38 - pick avg.

int humid=0, temp=0;

void waitDelay(int delay=100){
   for (int i=0; i<delay; i++) {}        // blocking delay
}

int getDHTReading(){
   unsigned char data[5] = {0,0,0,0,0};
   cout << "Getting a sample from the DHT Sensor\n";
   bcm2835_gpio_fsel(DHT_GPIO, BCM2835_GPIO_FSEL_OUTP);
   bcm2835_gpio_write(DHT_GPIO, LOW);
   usleep(18000);                             // wait for 18ms
   bcm2835_gpio_write(DHT_GPIO, HIGH);
   bcm2835_gpio_fsel(DHT_GPIO, BCM2835_GPIO_FSEL_INPT);
   bcm2835_gpio_set_pud(DHT_GPIO, BCM2835_GPIO_PUD_DOWN);

   do { waitDelay(); } while(bcm2835_gpio_lev(DHT_GPIO)==HIGH);
   do { waitDelay(); } while(bcm2835_gpio_lev(DHT_GPIO)==LOW);
   do { waitDelay(); } while(bcm2835_gpio_lev(DHT_GPIO)==HIGH);
   // Remember the highs, ignore the lows -- a good philosophy!
   for(int d=0; d<5; d++) {       // for each data byte
      // read 8 bits
      for(int i=0; i<8; i++) {    // for each bit of data
         do { waitDelay(); } while(bcm2835_gpio_lev(DHT_GPIO)==LOW);
         int width = 0;           // measure width of each high
         do {
            width++;
            waitDelay();
            if(width>1000) break; // missed a pulse -- data invalid!
         } while(bcm2835_gpio_lev(DHT_GPIO)==HIGH);    // time it!
         // shift in the data, msb first if width > the threshold
         data[d] = data[d] | ((width > LH_THRESHOLD) << (7-i));
      }
   }
   if (USING_DHT11){
      humid = data[0] * 10;            // one byte - no fractional part
      temp = data[2] * 10;             // multiplying to keep code concise
   }
   else {                              // for DHT22 (AM2302/AM2301)
      humid = (data[0]<<8 | data[1]);  // shift MSBs 8 bits left and OR LSBs
      temp = (data[2]<<8 | data[3]);   // same again for temperature
   }
   unsigned char chk = 0;   // the checksum will overflow automatically
   for(int i=0; i<4; i++){ chk+= data[i]; }
   if(chk==data[4]){
      cout << "The checksum is good" << endl;
      cout << "The temperature is " << (float)temp/10 << "°C" << endl;
      cout << "The humidity is " << (float)humid/10 << "%" << endl;
   }
   else {
      cout << "Checksum bad - data error - trying again!" << endl;
      usleep(2000000);   // have to delay for 1-2 seconds between readings
      return -1;
   }
   return 0;
}

int main() {
   ArduiPi_OLED display;
   bool isTemperature = true;
   if (!bcm2835_init()){
      cout << "Failed exiting " << endl;
      return 1;
   }
   if(!display.init(OLED_I2C_RESET, OLED_ADAFRUIT_I2C_128x64)){
      perror("Failed to set up the display\n");
      return -1;
   }
   printf("Setting up the I2C Display output\n");
   display.begin();
   display.setTextColor(WHITE);

   // keep updating the display until program is killed
   while(true){
      display.setTextSize(1);
      display.clearDisplay();
      display.setCursor(27,5);
      display.print("Exploring RPi");
      while (getDHTReading() ==-1) {}         // keep going until get a valid DHT reading
      printf("have reading\n");
      if(isTemperature){
         display.setCursor(30,18);
         display.print("Temperature:");
         display.setCursor(20,37);
         display.setTextSize(3);
         display.printf("%02.1fC", (float)temp/10 );
      }
      else {
         display.setCursor(40,18);
         display.print("Humidity:");
         display.setCursor(20,37);
         display.setTextSize(3);
         display.printf("%02.1f%%", (float)humid/10 ); // space important
      }
      display.display();
      printf("sleeping\n");
      usleep(5000000);
      isTemperature=!isTemperature;
   }
   display.close();
   bcm2835_close();
   return 0;
}
