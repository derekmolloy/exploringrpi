// Simple Node.js example program to set up GPIO LED4 to be turned on or off from
// the Linux console. Written by Derek Molloy for the book "Exploring
// Raspberry Pi"

// Ignore the first two arguments (nodejs and the program name)
var myArgs = process.argv.slice(2)
var GPIO4_PATH = "/sys/class/gpio/gpio4/"
var GPIO_SYSFS = "/sys/class/gpio/"
var GPIO_NUMBER = 4

function writeGPIO( filename, value, path ){
  var fs = require('fs')
  try {
     fs.writeFileSync(path+filename, value)
  }
  catch (err) {
     console.log("The Write Failed to the File: " + path+filename)
  }
}

console.log("Starting the RPi LED Node.js Program");
if (myArgs[0]==null){
   console.log("There is an incorrect number of arguments.");
   console.log("  Usage is: nodejs nodejsLED.js command")
   console.log("  where command is one of: setup, on, off, status, or close.")
   process.exit(2)   //exits with the error code 2 (incorrect usage)
}
switch (myArgs[0]) {
   case 'on':
      console.log("Turning the LED On")
      writeGPIO("value", "1", GPIO4_PATH)
      break
   case 'off':
      console.log("Turning the LED Off")
      writeGPIO("value", "0", GPIO4_PATH)
      break
   case 'setup':
      console.log("Exporting the LED GPIO")
      writeGPIO("export", GPIO_NUMBER, GPIO_SYSFS)
      // need to delay by 100ms or the GPIO will not be exported when setting the direction
      setTimeout(function(){ writeGPIO("direction", "out", GPIO4_PATH) },100)
      break
   case 'close':
      console.log("Unexporting the LED GPIO")
      writeGPIO("unexport", GPIO_NUMBER, GPIO_SYSFS)
      break
   case 'status':
      console.log("Getting the LED Status")
      fs = require('fs')
      fs.readFile(GPIO4_PATH+"value", 'utf8', function (err, data) {
         if (err) { 
            return console.log(err)
         }
         console.log(data)
      })
      break
   default:
      console.log("Invalid Command")
}
console.log("End of RPi Node.js script")
