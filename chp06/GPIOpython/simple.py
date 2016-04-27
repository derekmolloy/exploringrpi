#!/usr/bin/python
# A Python program that uses the GPIO C++ class
import gpio
from time import sleep

print "Start of the Python Simple GPIO program"
led = gpio.GPIO(17)
button  = gpio.GPIO(27)
led.setDirection(1)
button.setDirection(0)
while button.getValue() == 0:
   led.setValue(1)
   sleep(0.1)
   led.setValue(0)
   sleep(0.1)
print "End of the GPIO program"
