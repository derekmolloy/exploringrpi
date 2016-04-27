var util = require('util');
var SerialPort = require('serialport').SerialPort;
var xbee_api = require('../../lib/xbee-api.js');

var C = xbee_api.constants;

var xbeeAPI = new xbee_api.XBeeAPI({
  api_mode: 1
});

var serialport = new SerialPort("COM5", {
  baudrate: 57600,
  parser: xbeeAPI.rawParser()
});

serialport.on("open", function() {
  console.log("Serial port open... sending some data to a node...");
  var frame = {
    type: C.FRAME_TYPE.TX_REQUEST_64,
    destination64: "0013a200400a0127",
    options: 0x00, // optional, 0x00 is default
    data: "TxData0A" // Can either be string or byte array.
  };

  serialport.write(xbeeAPI.buildFrame(frame), function(err, res) {
    if (err) throw(err);
    else     console.log("written bytes: "+util.inspect(res));
  });
});


xbeeAPI.on("frame_object", function(frame) {
  console.log("OBJ> "+util.inspect(frame));
});
