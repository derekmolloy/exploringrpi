/*
 * xbee-api
 * https://github.com/jouz/xbee-api
 *
 * Copyright (c) 2013 Jan Kolkmeier
 * Licensed under the MIT license.
 */

'use strict';

var C = require('./constants.js');

var frame_parser = module.exports = {};

frame_parser[C.FRAME_TYPE.NODE_IDENTIFICATION] = function(frame, reader) {
  frame.sender64 = reader.nextString(8, 'hex');
  frame.sender16 = reader.nextString(2, 'hex');
  frame.receiveOptions = reader.nextUInt8();
  frame_parser.parseNodeIdentificationPayload(frame, reader);
};

frame_parser[C.FRAME_TYPE.ZIGBEE_RECEIVE_PACKET] = function(frame, reader) {
  frame.remote64 = reader.nextString(8, 'hex');
  frame.remote16 = reader.nextString(2, 'hex');
  frame.receiveOptions = reader.nextUInt8();
  frame.data = reader.nextAll();
};

frame_parser[C.FRAME_TYPE.ZIGBEE_EXPLICIT_RX] = function(frame, reader) {
  frame.remote64 = reader.nextString(8, 'hex');
  frame.remote16 = reader.nextString(2, 'hex');
  frame.sourceEndpoint = reader.nextString(1, 'hex');
  frame.destinationEndpoint = reader.nextString(1, 'hex');
  frame.clusterId = reader.nextString(2, 'hex');
  frame.profileId = reader.nextString(2, 'hex');
  frame.receiveOptions = reader.nextUInt8();
  frame.data = reader.nextAll();
};

frame_parser[C.FRAME_TYPE.XBEE_SENSOR_READ] = function(frame, reader) {
  frame.remote64 = reader.nextString(8, 'hex');
  frame.remote16 = reader.nextString(2, 'hex');
  frame.receiveOptions = reader.nextUInt8();
  frame.sensors = reader.nextUInt8();
  frame.sensorValues = {
      AD0: Math.round(1000 * (reader.nextUInt16BE() * 5.1) / 255.0),
      AD1: Math.round(1000 * (reader.nextUInt16BE() * 5.1) / 255.0),
      AD2: Math.round(1000 * (reader.nextUInt16BE() * 5.1) / 255.0),
      AD3: Math.round(1000 * (reader.nextUInt16BE() * 5.1) / 255.0),
      T:   reader.nextUInt16BE(),
      temperature: undefined,
      relativeHumidity: undefined,
      trueHumidity: undefined,
      waterPresent: frame.sensors === 0x60
  };

  if (frame.sensors === 2 || frame.sensors === 3) {
    if (frame.sensorValues.T < 2048) {
      frame.sensorValues.temperature = frame.sensorValues.T / 16;
    } else {
      frame.sensorValues.temperature = -(frame.sensorValues.T & 0x7ff) / 16;
    }
  }

  if (frame.sensors === 1 || frame.sensors === 3) {
    frame.sensorValues.relativeHumidity = Math.round(100 *
        (((frame.sensorValues.AD3 / frame.sensorValues.AD2) -
            0.16) / (0.0062))) / 100;
  }

  if (frame.sensors === 3) {
    frame.sensorValues.trueHumidity = Math.round(100 *
        (frame.sensorValues.relativeHumidity / (1.0546 -
            (0.00216 * frame.sensorValues.temperature)))) / 100;
  }

};

frame_parser[C.FRAME_TYPE.MODEM_STATUS] = function(frame, reader) {
  frame.modemStatus = reader.nextUInt8();
};

frame_parser[C.FRAME_TYPE.ZIGBEE_IO_DATA_SAMPLE_RX] = function(frame, reader) {
  frame.remote64 = reader.nextString(8, 'hex');
  frame.remote16 = reader.nextString(2, 'hex');
  frame.receiveOptions = reader.nextUInt8();
  frame_parser.ParseIOSamplePayload(frame, reader);
};

frame_parser[C.FRAME_TYPE.AT_COMMAND_RESPONSE] = function(frame, reader) {
  frame.id = reader.nextUInt8();
  frame.command = reader.nextString(2, 'ascii');
  frame.commandStatus = reader.nextUInt8();
  if (frame.command === "ND") {
    frame.nodeIdentification = {};
    frame_parser.parseNodeIdentificationPayload(frame.nodeIdentification, reader);
  } else {
    frame.commandData = reader.nextAll();
  }
};

frame_parser[C.FRAME_TYPE.REMOTE_COMMAND_RESPONSE] = function(frame, reader) {
  frame.id = reader.nextUInt8();
  frame.remote64 = reader.nextString(8, 'hex');
  frame.remote16 = reader.nextString(2, 'hex');
  frame.command = reader.nextString(2, 'ascii');
  frame.commandStatus = reader.nextUInt8();
  if(frame.command === "IS") {
    frame_parser.ParseIOSamplePayload(frame, reader);
  }
  else {
    frame.commandData = reader.nextAll();
  }
};

frame_parser[C.FRAME_TYPE.ZIGBEE_TRANSMIT_STATUS] = function(frame, reader) {
  frame.id = reader.nextUInt8();
  frame.remote16 = reader.nextString(2, 'hex');
  frame.transmitRetryCount = reader.nextUInt8();
  frame.deliveryStatus = reader.nextUInt8();
  frame.discoveryStatus = reader.nextUInt8();
};

frame_parser[C.FRAME_TYPE.ROUTE_RECORD] = function(frame, reader) {
  frame.remote64 = reader.nextString(8, 'hex');
  frame.remote16 = reader.nextString(2, 'hex');
  frame.receiveOptions = reader.nextUInt8();
  frame.hopCount = reader.nextUInt8();
  frame.addresses = [];
  for (var i=0; i<frame.hopCount; i++) {
    frame.addresses.push(reader.nextUInt16BE());
  }
};

frame_parser[C.FRAME_TYPE.AT_COMMAND] = 
frame_parser[C.FRAME_TYPE.AT_COMMAND_QUEUE_PARAMETER_VALUE] = function(frame, reader) {
  frame.id = reader.nextUInt8();
  frame.command = reader.nextString(2, 'ascii');
  frame.commandParameter = reader.nextAll();
};

frame_parser[C.FRAME_TYPE.REMOTE_AT_COMMAND_REQUEST] = function(frame, reader) {
  frame.id = reader.nextUInt8();
  frame.destination64 = reader.nextString(8, 'hex');
  frame.destination16 = reader.nextString(2, 'hex');
  frame.remoteCommandOptions = reader.nextUInt8();
  frame.command = reader.nextString(2, 'ascii');
  frame.commandParameter = reader.nextAll();
};

frame_parser[C.FRAME_TYPE.ZIGBEE_TRANSMIT_REQUEST] = function(frame, reader) {
  frame.id = reader.nextUInt8();
  frame.destination64 = reader.nextString(8, 'hex');
  frame.destination16 = reader.nextString(2, 'hex');
  frame.broadcastRadius = reader.nextUInt8();
  frame.options = reader.nextUInt8();
  frame.data = reader.nextAll();
};

frame_parser[C.FRAME_TYPE.EXPLICIT_ADDRESSING_ZIGBEE_COMMAND_FRAME] = function(frame, reader) {
  frame.id = reader.nextUInt8();
  frame.destination64 = reader.nextString(8, 'hex');
  frame.destination16 = reader.nextString(2, 'hex');
  frame.sourceEndpoint = reader.nextUInt8();
  frame.destinationEndpoint = reader.nextUInt8();
  frame.clusterId = reader.nextUInt16BE();
  frame.profileId = reader.nextUInt16BE();
  frame.broadcastRadius = reader.nextUInt8();
  frame.options = reader.nextUInt8();
  frame.data = reader.nextAll();
};

frame_parser[C.FRAME_TYPE.TX_REQUEST_64] = function(frame, reader) {
  frame.id = reader.nextUInt8();
  frame.destination64 = reader.nextString(8, 'hex');
  frame.options = reader.nextUInt8();
  frame.data = reader.nextAll();
};

frame_parser[C.FRAME_TYPE.TX_REQUEST_16] = function(frame, reader) {
  frame.id = reader.nextUInt8();
  frame.destination16 = reader.nextString(2, 'hex');
  frame.options = reader.nextUInt8();
  frame.data = reader.nextAll();
};

frame_parser.parseNodeIdentificationPayload = function(frame, reader) {
  frame.remote16 = reader.nextString(2, 'hex');
  frame.remote64 = reader.nextString(8, 'hex');

  // Extract the NI string from the buffer
  frame.nodeIdentifier = reader.nextStringZero('ascii');

  frame.remoteParent16 = reader.nextString(2, 'hex');
  frame.deviceType = reader.nextUInt8();
  frame.sourceEvent = reader.nextUInt8();
  frame.digiProfileID = reader.nextString(2, 'hex');
  frame.digiManufacturerID = reader.nextString(2, 'hex');
};

frame_parser.ParseIOSamplePayload = function(frame, reader) {
  frame.digitalSamples = {};
  frame.analogSamples = {};
  frame.numSamples = reader.nextUInt8();
  var mskD = reader.nextUInt16BE(); 
  var mskA = reader.nextUInt8();

  if (mskD > 0) {
    var valD = reader.nextUInt16BE();
    for (var dbit in C.DIGITAL_CHANNELS.MASK) {
      if ((mskD & (1 << dbit)) >> dbit) {
        frame.digitalSamples[C.DIGITAL_CHANNELS.MASK[dbit][0]] = (valD & (1 << dbit)) >> dbit;
      }
    }
  }

  if (mskA > 0) {
    for (var abit in C.ANALOG_CHANNELS.MASK) {
      if ((mskA & (1 << abit)) >> abit) {
        var valA = reader.nextUInt16BE();
        // Convert to mV, resolution is < 1mV, so rounding is OK
        frame.analogSamples[C.ANALOG_CHANNELS.MASK[abit][0]] = Math.round((valA * 1200) / 1023);
      }
    }
  }
};

// Series 1 Support
frame_parser.Recieved16BitPacketIO = function(frame, reader) {
  var hasDigital = 0;
  var data = {};
  // OFFSET 4
 //reader.move(4);
  data.sampleQuantity = reader.nextUInt8();
  data.channelMask    = reader.nextUInt16BE(); 
  data.channels       = {};
  data.analogSamples  = [];
  data.digitalSamples = [];

  //analog channels
  for( var a=0; a<=5; a++ ){
    // exponent looks odd here because analog pins start at 0000001000000000
    if( Boolean(data.channelMask & Math.pow(2,a+9)) ){
      data.channels['ADC'+a] = 1;
    }
  }

  // if any of the DIO pins are active, parse the digital samples 
  // 0x1ff = 0000000111111111
  if(data.channelMask & 0x1ff){
    hasDigital = 1;
    for( var i=0; i < data.sampleQuantity; i++ ){
      data.digitalSamples.push( reader.nextUInt16BE().toString(2) );
    }

    //digital channels
    for( var d=0; d<=8; d++ ){
      if( Boolean(data.channelMask & Math.pow(2,d)) ){
        data.channels['DIO'+d] = 1;
      }
    }
  }

  for( var si=0; si < data.sampleQuantity; si++ ){
    var sample = {};
    for( var j=0; j <= 5; j++ ){
      if( data.channels['ADC'+j] ){
        // starts at the 7th byte and moved down by the Digital Samples section
        sample['ADC'+j] = reader.nextUInt16BE();
      }
    }
    data.analogSamples.push(sample);
  }

  frame.data = data;
};

frame_parser[C.FRAME_TYPE.TX_STATUS] = function(frame, reader) {
  frame.id = reader.nextUInt8();
  frame.deliveryStatus = reader.nextUInt8();
};

frame_parser[C.FRAME_TYPE.RX_PACKET_64] = function(frame, reader) {
  frame.remote64 = reader.nextString(8, 'hex');
  frame.rssi = reader.nextUInt8();
  frame.receiveOptions = reader.nextUInt8();
  frame.data = reader.nextAll();
};

frame_parser[C.FRAME_TYPE.RX_PACKET_16] = function(frame, reader) {
  frame.remote16 = reader.nextString(2, 'hex');
  frame.rssi = reader.nextUInt8();
  frame.receiveOptions = reader.nextUInt8();
  frame.data = reader.nextAll();
};

frame_parser[C.FRAME_TYPE.RX_PACKET_64_IO] = function(frame, reader) {
  frame.remote64 = reader.nextString(8, 'hex');
  frame.rssi = reader.nextUInt8();
  frame.receiveOptions = reader.nextUInt8();
  frame.data = reader.nextAll();
  // TODO: Parse I/O Data?
};


frame_parser[C.FRAME_TYPE.RX_PACKET_16_IO] = function(frame, reader) {
  frame.remote16 = reader.nextString(2, 'hex');
  frame.rssi = reader.nextUInt8();
  frame.receiveOptions = reader.nextUInt8();
  frame_parser.Recieved16BitPacketIO(frame, reader);
};
