#!/usr/bin/python2
# A Python program that calls C program code

import ERPiModule

print("*** Start of the Python program")
print("--> Calling the C hello() function passing Derek")
ERPiModule.hello("Derek")
print("--> Calling the C integrate() function")
val = ERPiModule.integrate(0, 3.14159, 1000000)
print("*** The result is: ", val)
print("*** End of the Python program")
