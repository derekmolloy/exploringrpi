#!/usr/bin/python3
import wiringpi2 as wpi
from time import sleep

print("Starting the Python wiringPi example")
wpi.wiringPiSetupGpio()
wpi.pinMode(17,1)
wpi.pinMode(27,0)
while wpi.digitalRead(27)==0:
   wpi.digitalWrite(17,1)
   sleep(0.1)
   wpi.digitalWrite(17,0)
   sleep(0.1)
print("Button pressed: end of example")
