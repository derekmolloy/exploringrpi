var SerialPort = require('serialport').SerialPort;
var xbee_api = require('../../lib/xbee-api.js');
var C = xbee_api.constants;
var Q = require('q');

// following settings work for me on my Raspberry pi, your config may differ!
var xbeeAPI = new xbee_api.XBeeAPI({
  api_mode: 1
});
var serialport = new SerialPort("/dev/ttyAMA0", {
  baudrate: 9600,
  parser: xbeeAPI.rawParser()
});

// How long are we prepared to wait for a response to our command?
var maxWait = 5000; // ms

function xbeeCommand(frame) {
    // set frame id
    frame.id = xbeeAPI.nextFrameId();

    // We're going to return a promise
    var deferred = Q.defer();

    var callback = function(receivedFrame) {
        if (receivedFrame.id == frame.id) {
            // This is our frame's response. Resolve the promise.
            deferred.resolve(receivedFrame);
        }
    };

    // Clear up: remove listener after the timeout and a bit, it's no longer needed
    setTimeout(function() {
        xbeeAPI.removeListener("frame_object", callback);
    }, maxWait + 1000);
    
    // Attach callback so we're waiting for the response
    xbeeAPI.on("frame_object", callback);

    // Pass the bytes down the serial port
    serialport.write(xbeeAPI.buildFrame(frame));

    // Return our promise with a timeout
    return deferred.promise.timeout(maxWait);

}

xbeeCommand({
    type: C.FRAME_TYPE.REMOTE_AT_COMMAND_REQUEST,
    destination64: "0013a2004086a04a",
    command: "DB",
    commandParameter: [],
}).then(function(f) {
    console.log("Command successful:", f);
}).catch(function(e) {
    console.log("Command failed:", e);
});
