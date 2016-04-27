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
  console.log("Serial port open... sending ATND");
  var frame = {
    type: C.FRAME_TYPE.AT_COMMAND,
    command: "ND",
    commandParameter: [],
  };

  serialport.write(xbeeAPI.buildFrame(frame), function(err, res) {
    if (err) throw(err);
    else     console.log("written bytes: "+util.inspect(res));
  });
});


xbeeAPI.on("frame_object", function(frame) {
  console.log("OBJ> "+util.inspect(frame));
});
