// Based on example code that is provided by Nick Gammon
// Please see: http://www.gammon.com.au/ for further details
#include <SPI.h>
#define MISO 12
volatile int count, lastcount;

void setup () {
   Serial.begin (115200);     // for serial output debug
   SPCR |= _BV(SPE);          // turn on SPI in slave mode
   pinMode(MISO, OUTPUT);     // Send on the MISO line
   SPI.setClockDivider(SPI_CLOCK_DIV16);   // A 1 MHz clock
   SPI.attachInterrupt();     // now turn on interrupts
   Serial.println("Setup complete");   // debug message
}

void loop() {
  if (count>lastcount) {
    Serial.print("Count is now: ");
    Serial.println(count);
    //SPI.transfer(count);
    lastcount=count;
  }
}

ISR (SPI_STC_vect) {          // The SPI interrupt routine
   Serial.print("ISR invoked: Received (int)");
   byte c = SPDR;             // get a byte from the SPI Data Register
   Serial.println((int)c);
   count++;
   Serial.println("End of ISR");
}




