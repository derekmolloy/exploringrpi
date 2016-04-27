#include<iostream>
#include"GPIO.h"
using namespace exploringRPi;
using namespace std;

int main(){
   GPIO outGPIO(17), inGPIO(27);
   inGPIO.setDirection(INPUT);    //button is an input
   outGPIO.setDirection(OUTPUT);  //LED is an output
   inGPIO.setEdgeType(RISING);    //wait for rising edge
   outGPIO.streamOpen();          //fast write, ready file
   outGPIO.streamWrite(LOW);      //turn the LED off
   cout << "Press the button:" << endl;
   inGPIO.waitForEdge();          //will wait forever
   outGPIO.streamWrite(HIGH);     //button pressed, light LED
   outGPIO.streamClose();         //close the output stream
   return 0;
}
