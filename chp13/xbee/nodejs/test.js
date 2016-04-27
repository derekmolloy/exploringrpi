// From the example code at www.npmjs.com/package/xbee-api
var util       = require('util');
var SerialPort = require('serialport').SerialPort;
var xbee_api   = require('xbee-api');
var C          = xbee_api.constants;
 
var xbeeAPI = new xbee_api.XBeeAPI({
  api_mode: 1
});
 
var serialport = new SerialPort("/dev/ttyUSB0", {
  baudrate: 9600,
  parser: xbeeAPI.rawParser()
});
 
serialport.on("open", function() {
  var frame_obj = {                 // AT Request to be sent to  
    type: C.FRAME_TYPE.AT_COMMAND,  // Prepare for an AT command
    command: "NI",                  // Node identifer command
    commandParameter: [],           // No parameters needed
  };
  serialport.write(xbeeAPI.buildFrame(frame_obj)); 
});
 
// The data frames are outputted by this function
xbeeAPI.on("frame_object", function(frame) {
    console.log(">>", frame);
});
