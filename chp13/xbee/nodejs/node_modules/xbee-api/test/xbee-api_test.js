/*
 * xbee-api 
 * https://github.com/jouz/xbee-api
 *
 * Copyright (c) 2013 Jan Kolkmeier
 * Licensed under the MIT license.
 */

'use strict';

var xbee_api = require('../lib/xbee-api.js');
var C = require('../lib/constants.js');

exports['Main'] = {
  setUp: function(done) {
    done();
  },
  tearDown: function (done) {
    done();
  },
  'Option passing': function(test) {
    test.expect(3);
    var xbeeAPI1 = new xbee_api.XBeeAPI();
    test.equal(xbeeAPI1.options.api_mode, 1, "Do default options work?");

    var options = { api_mode: 2 };
    var xbeeAPI2 = new xbee_api.XBeeAPI(options);
    // given a byte array like [3,21], convert to a decimal value.
    test.equal(xbeeAPI2.options.api_mode, 2, "Are passed options applied?");

    var xbeeAPI3 = new xbee_api.XBeeAPI();
    test.equal(xbeeAPI3.options.api_mode, 1, "Are default options left untouched?");

    test.done();
  },
  'nextFrameId': function(test) {
    // Test if frameId actually increments... assuming we dont start near 254...
    test.expect(1);
    var xbeeAPI = new xbee_api.XBeeAPI();
    var frame = {
      type: C.FRAME_TYPE.AT_COMMAND,
      id: 0x52,
      command: "NJ",
      commandParameter: [],
    };
    var xbeeFrame = function(frame) {
        frame.id = xbeeAPI.nextFrameId();
        return frame.id;
    };
    var fId1 = xbeeFrame(frame);
    var fId2 = xbeeFrame(frame);
    test.equal((fId1+1), fId2);
    test.done();
  }
};

exports['API Frame building'] = { // These have to be tested both for AP=1 and 2
  'AT Command Requests': function(test) {
    test.expect(1);

    var frame = {
      type: C.FRAME_TYPE.AT_COMMAND,
      id: 0x52,
      command: "NJ",
      commandParameter: [],
    };

    // AT Command; 0x08; Queries ATNJ
    var expected0 = new Buffer([ 0x7E, 0x00, 0x04, 0x08, 0x52, 0x4E, 0x4A, 0x0D]);

    var xbeeAPI = new xbee_api.XBeeAPI();
    test.deepEqual(expected0, xbeeAPI.buildFrame(frame), "create raw frame");
    test.done();
  },
  'AT Command Queue Requests': function(test) {
    test.expect(1);

    var frame = {
      type: C.FRAME_TYPE.AT_COMMAND_QUEUE_PARAMETER_VALUE,
      id: 0x01,
      command: "BD",
      commandParameter: [ 0x07 ]
    };

    // AT Command - Queue Param. Value; 0x09; Queues ATBD7
    var expected0 = new Buffer([ 0x7E, 0x00, 0x05, 0x09, 0x01, 0x42, 0x44, 0x07, 0x68]);

    var xbeeAPI = new xbee_api.XBeeAPI();
    test.deepEqual(expected0, xbeeAPI.buildFrame(frame), "create raw frame");
    test.done();
  },
  'AT Remote Command Requests': function(test) {
    test.expect(1);
    
    var frame = {
      type: C.FRAME_TYPE.REMOTE_AT_COMMAND_REQUEST,
      id: 0x01,
      destination64: "0013a20040401122",
      destination16: "fffe",
      remoteCommandOptions: 0x02,
      command: "BH",
      commandParameter: [ 0x01 ]
    };

    // Remote AT Command Req.; 0x17; ATBH1
    var expected0 = new Buffer([ 0x7E, 0x00, 0x10, 0x17, 0x01, 0x00, 0x13, 0xA2, 0x00, 0x40, 0x40, 0x11, 0x22, 0xFF, 0xFE, 0x02, 0x42, 0x48, 0x01, 0xF5]);

    var xbeeAPI = new xbee_api.XBeeAPI();
    test.deepEqual(expected0, xbeeAPI.buildFrame(frame), "create raw frame");
    test.done();
  },
  'Transmit Requests': function(test) {
    test.expect(1);
    
    var frame = {
      type: C.FRAME_TYPE.ZIGBEE_TRANSMIT_REQUEST,
      id: 0x01,
      destination64: "0013a200400a0127",
      destination16: "fffe",
      broadcastRadius: 0x00,
      options: 0x00,
      data: "TxData0A"
    };

    // Transmit request; 0x10; sends chars: TxData0A (AP=1)
    var expected0 = new Buffer([ 0x7E, 0x00, 0x16, 0x10, 0x01, 0x00, 0x13, 0xA2, 0x00, 0x40, 0x0A, 0x01, 0x27, 0xFF, 0xFE, 0x00, 0x00, 0x54, 0x78, 0x44, 0x61, 0x74, 0x61, 0x30, 0x41, 0x13 ]);

    var xbeeAPI = new xbee_api.XBeeAPI();
    test.deepEqual(expected0, xbeeAPI.buildFrame(frame), "create raw frame");
    test.done();
  },
  'Keep Frame ID Zero': function(test) {
    test.expect(1);

    var frame = {
      type: C.FRAME_TYPE.AT_COMMAND,
      id: 0x00,
      command: "NJ",
      commandParameter: [],
    };

    // AT Command; 0x08; Queries ATNJ
    var expected0 = new Buffer([ 0x7E, 0x00, 0x04, 0x08, 0x00, 0x4E, 0x4A, 0x5F]);

    var xbeeAPI = new xbee_api.XBeeAPI();
    test.deepEqual(expected0, xbeeAPI.buildFrame(frame), "create raw frame");
    test.done();
  },
  'Assign ID When Missing': function(test) {
    test.expect(1);
    
    var frame = {
      type: C.FRAME_TYPE.AT_COMMAND,
      command: "NJ",
      commandParameter: [],
    };

    var xbeeAPI = new xbee_api.XBeeAPI();
    var firstId = xbeeAPI.nextFrameId();
    
    var buf = xbeeAPI.buildFrame(frame);

    test.equal(firstId + 1, buf[4], "create raw frame");
    test.done();
  }
};


exports['API Frame Parsing'] = {
  'AT Remote Command Responses': function(test) {
    test.expect(6);
    var xbeeAPI = new xbee_api.XBeeAPI();
    var parser = xbeeAPI.rawParser();

    xbeeAPI.once("frame_object", function(frame) { // frame1
      test.equal(frame.id, 0x55, "Parse frameid");
      test.equal(frame.remote64, '0013a20040522baa', "Parse remote64");
      test.equal(frame.remote16, '7d84', "Parse remote16");
      test.equal(frame.command, "SL", "Parse command");
      test.equal(frame.commandStatus, 0, "Parse command status");
      test.deepEqual(frame.commandData, new Buffer([ 0x40, 0x52, 0x2b, 0xaa ]));
      test.done();
    });

    // Remote Command Response; 0x97; ATSL [OK] 40522BAA
    var rawFrame = new Buffer([ 0x7E, 0x00, 0x13, 0x97, 0x55, 0x00, 0x13, 0xA2, 0x00, 0x40, 0x52, 0x2B, 0xAA, 0x7D, 0x84, 0x53, 0x4C, 0x00, 0x40, 0x52, 0x2B, 0xAA, 0xF0 ]);
    parser(null, rawFrame);
  },
  'AT Command Responses': function(test) {
    test.expect(3);
    var xbeeAPI = new xbee_api.XBeeAPI();
    var parser = xbeeAPI.rawParser();

    xbeeAPI.once("frame_object", function(frame) { // frame0
      test.equal(frame.id, 0x01, "Parse frameid");
      test.equal(frame.command, "BD", "Parse command");
      test.equal(frame.commandStatus, 0, "Parse command status");
      test.done();
    });

    // AT Command Response; 0x88; ATBD [OK] (no data)
    var rawFrame = new Buffer([ 0x7E, 0x00, 0x05, 0x88, 0x01, 0x42, 0x44, 0x00, 0xF0 ]);
    parser(null, rawFrame);
  },
  'Transmit Status': function(test) {
    test.expect(5);
    var xbeeAPI = new xbee_api.XBeeAPI();
    var parser = xbeeAPI.rawParser();
    xbeeAPI.once("frame_object", function(frame) {
      test.equal(frame.remote16, "7d84", "Parse remote16");
      test.equal(frame.id, 0x01, "Parse id");
      test.equal(frame.transmitRetryCount, 0, "Parse retry count");
      test.equal(frame.deliveryStatus, 0, "Parse delivery status");
      test.equal(frame.discoveryStatus, 1, "Parse discovery status");
      test.done();
    });
    // ZigBee Transmit Status; 0x8B; 0 retransmit, Success, Address Discovery
    var rawFrame = new Buffer([ 0x7E, 0x00, 0x07, 0x8B, 0x01, 0x7D, 0x84, 0x00, 0x00, 0x01, 0x71 ]);
    parser(null, rawFrame);
  },
  'Modem Status': function(test) {
    test.expect(1);
    var xbeeAPI = new xbee_api.XBeeAPI();
    var parser = xbeeAPI.rawParser();
    xbeeAPI.once("frame_object", function(frame) {
      test.equal(frame.modemStatus, 6, "Parse status");
      test.done();
    });
    // Modem status; 0x8A; Coordinator Started
    var rawFrame = new Buffer([ 0x7E, 0x00, 0x02, 0x8A, 0x06, 0x6F ]);
    parser(null, rawFrame);
  }, 

  'Receive Packet': function(test) {
    test.expect(4);
    var xbeeAPI = new xbee_api.XBeeAPI();
    var parser = xbeeAPI.rawParser();
    xbeeAPI.once("frame_object", function(frame) {
      test.equal(frame.remote64, '0013a20040522baa', "Parse remote64");
      test.equal(frame.remote16, '7d84', "Parse remote16");
      test.equal(frame.receiveOptions, 1, "Parse receive options");
      test.deepEqual(frame.data, new Buffer([ 0x52, 0x78, 0x44, 0x61, 0x74, 0x61 ]));
      test.done();
    });
    // Receive Packet; 0x90; Receive packet with chars RxData
    var rawFrame = new Buffer([ 0x7E, 0x00, 0x12, 0x90, 0x00, 0x13, 0xA2, 0x00, 0x40, 0x52, 0x2B, 0xAA, 0x7D, 0x84, 0x01, 0x52, 0x78, 0x44, 0x61, 0x74, 0x61, 0x0D ]);
    parser(null, rawFrame);
  }, 

  'Leading Garbage': function(test) {
    test.expect(4);
    var xbeeAPI = new xbee_api.XBeeAPI();
    var parser = xbeeAPI.rawParser();
    xbeeAPI.once("frame_object", function(frame) {
      test.equal(frame.remote64, '0013a20040522baa', "Parse remote64");
      test.equal(frame.remote16, '7d84', "Parse remote16");
      test.equal(frame.receiveOptions, 1, "Parse receive options");
      test.deepEqual(frame.data, new Buffer([ 0x52, 0x78, 0x44, 0x61, 0x74, 0x61 ]));
      test.done();
    });
    // Receive Packet; 0x90; Receive packet with chars RxData
    var garbage = [];
    for (var i=0; i<520; i++) garbage.push(0x00);
    var garbageBuffer = new Buffer(garbage);
    var rawFrame = new Buffer([ 0x7E, 0x00, 0x12, 0x90, 0x00, 0x13, 0xA2, 0x00, 0x40, 0x52, 0x2B, 0xAA, 0x7D, 0x84, 0x01, 0x52, 0x78, 0x44, 0x61, 0x74, 0x61, 0x0D ]);
    var garbagedFrame = Buffer.concat([garbageBuffer, rawFrame]);
    parser(null, garbagedFrame);
  },
  'Receive Packet with AO=1': function(test) {
    test.expect(8);
    var xbeeAPI = new xbee_api.XBeeAPI();
    var parser = xbeeAPI.rawParser();
    xbeeAPI.once("frame_object", function(frame) {
      test.equal(frame.remote64, '0013a20040c401a9', "Parse remote64");
      test.equal(frame.remote16, '0000', "Parse remote16");
      test.equal(frame.sourceEndpoint, 'e8', 'Source Endpoint');
      test.equal(frame.destinationEndpoint, 'e8', 'Destination Endpoint');
      test.equal(frame.clusterId, '0011', 'Cluster Id');
      test.equal(frame.profileId, 'c105', 'Profile Id');
      test.equal(frame.receiveOptions, 1, "Parse receive options");
      test.deepEqual(frame.data, new Buffer([0x74, 0x65, 0x73, 0x74, 0x20, 0x6D, 0x65, 0x73, 0x73, 0x61, 0x67, 0x65]));
      test.done();
    });
    // Receive Packet; 0x90; Receive packet with chars RxData
    var rawFrame = new Buffer([0x7E, 0x00, 0x1E, 0x91, 0x00, 0x13, 0xA2, 0x00, 0x40, 0xC4, 0x01, 0xA9, 0x00, 0x00, 0xE8, 0xE8, 0x00, 0x11, 0xC1, 0x05, 0x01, 0x74, 0x65, 0x73, 0x74, 0x20, 0x6D, 0x65, 0x73, 0x73, 0x61, 0x67, 0x65, 0x9E]);
    parser(null, rawFrame);
  }, 

  'Receive Packet 16-bit IO': function(test) {
    test.expect(3);
    var xbeeAPI = new xbee_api.XBeeAPI({api_mode:1});
    var parser = xbeeAPI.rawParser();
    xbeeAPI.once("frame_object", function(frame) {
      test.equal(frame.remote16, '1234', "Parse remote16");
      test.equal(frame.data.analogSamples.length, frame.data.sampleQuantity, "Parse the right number of samples");
      test.equal(frame.data.channelMask, 0x0E58, "channel mask");
      test.done();
    });
    // Receive Packet; 0x83; Receive packet from IC or IR setting
    var rawFrame = new Buffer([0x7E, 0x00, 0x10, 0x83, 0x12, 0x34, 0x1B, 0x00, 0x01, 0x0E, 0x58, 0x00, 0x18, 0x00, 0x46, 0x01, 0x54, 0x02, 0x0A, 0xF5 ]);
    parser(null, rawFrame);
  }, 

  'Route Record': function(test) {
    test.expect(5);
    var xbeeAPI = new xbee_api.XBeeAPI();
    var parser = xbeeAPI.rawParser();
    xbeeAPI.once("frame_object", function(frame) {
      test.equal(frame.remote64, '0013a2004068f65b', "Parse remote64");
      test.equal(frame.remote16, '6d32', "Parse remote16");
      test.equal(frame.receiveOptions, 0, "Parse receive options");
      test.equal(frame.hopCount, 3, "Parse hop count");
      test.deepEqual(frame.addresses, [0x1234, 0x5678, 0x90AB], "Parse hop addresses");
      test.done();
    });
    // Receive Packet; 0xa1; Receive packet with 3 intermediate hops
    var rawFrame = new Buffer([ 0x7e, 0x00, 0x13, 0xa1, 0x00, 0x13, 0xa2, 0x00, 0x40, 0x68, 0xf6, 0x5b, 0x6d, 0x32, 0x00, 0x03, 0x12, 0x34, 0x56, 0x78, 0x90, 0xab, 0xbf ]);
    parser(null, rawFrame);
  },
  
  'ZigBee IO Data Sample Rx': function(test) {
    test.expect(6);
    var xbeeAPI = new xbee_api.XBeeAPI();
    var parser = xbeeAPI.rawParser();

    xbeeAPI.once("frame_object", function(frame) {
      test.equal(frame.remote64, '0013a20040522baa', "Parse remote64");
      test.equal(frame.remote16, '7d84', "Parse remote16");
      test.equal(frame.receiveOptions, 1, "Parse receive options");
      test.equal(frame.numSamples, 1, "Parse number of samples");
      test.deepEqual(frame.digitalSamples, {
        "DIO2": 1,
        "DIO3": 0,
        "DIO4": 1
      }, "Parsing digital samples");
      test.deepEqual(frame.analogSamples, {
        "AD1": 644
      }, "Parse analog samples");
      test.done();
    });

    // Receive IO Data Sample; 0x92; ...
    var rawFrame = new Buffer([ 0x7E, 0x00, 0x14, 0x92, 0x00, 0x13, 0xA2, 0x00, 0x40, 0x52, 0x2B, 0xAA, 0x7D, 0x84, 0x01, 0x01, 0x00, 0x1C, 0x02, 0x00, 0x14, 0x02, 0x25, 0xF5 ]);
    parser(null, rawFrame);
  },
  'XBee Sensor Read Indicator': function(test) {
    test.expect(5);
    var xbeeAPI = new xbee_api.XBeeAPI();
    var parser = xbeeAPI.rawParser();

    xbeeAPI.once("frame_object", function(frame) {
      test.equal(frame.remote64, '0013a20040522baa', "Parse remote64");
      test.equal(frame.remote16, 'dd6c', "Parse remote16");
      test.equal(frame.receiveOptions, 1, "Parse receive options");
      test.equal(frame.sensors, 0x03, "Parse receive options");
      test.deepEqual(frame.sensorValues, {
        "AD0": 40,
        "AD1": 4120,
        "AD2": 4680,
        "AD3": 1640,
        "T": 362,
        "temperature": 22.625,
        "relativeHumidity": 30.71,
        "trueHumidity": 30.54,
        "waterPresent": false
      }, "Parsing digital samples");
      test.done();
    });

    // Receive IO Data Sample; 0x94; ...
    var rawFrame = new Buffer([ 0x7E, 0x00, 0x17, 0x94, 0x00, 0x13, 0xA2, 0x00, 0x40, 0x52, 0x2b, 0xAA, 0xDD, 0x6c, 0x01, 0x03, 0x00, 0x02, 0x00, 0xCE, 0x00, 0xEA, 0x00, 0x52, 0x01, 0x6A, 0x8B ]);
    parser(null, rawFrame);
  },
  'Node Identification Indicator': function(test) {
    test.expect(9);
    var xbeeAPI = new xbee_api.XBeeAPI();
    var parser = xbeeAPI.rawParser();

    xbeeAPI.once("frame_object", function(frame) {
      test.equal(frame.sender64, '0013a20040522baa', "Parse sender64");
      test.equal(frame.sender16, '7d84', "Parse sender16");
      test.equal(frame.receiveOptions, 2, "Parse receive options");
      test.equal(frame.remote16, '7d84', "Parse remote16");
      test.equal(frame.remote64, '0013a20040522baa', "Parse remote64");
      test.equal(frame.nodeIdentifier, " ", "Parse node identifier");
      test.equal(frame.remoteParent16, 'fffe', "Parse parent16 ");
      test.equal(frame.deviceType, 1, "Parse device type");
      test.equal(frame.sourceEvent, 1, "Parse source event");
      // digi app profile...
      test.done();
    });

    // Receive IO Data Sample; 0x95; ...
    var rawFrame = new Buffer([ 0x7E, 0x00, 0x20, 0x95, 0x00, 0x13, 0xA2, 0x00, 0x40, 0x52, 0x2B, 0xAA, 0x7D, 0x84, 0x02, 0x7D, 0x84, 0x00, 0x13, 0xA2, 0x00, 0x40, 0x52, 0x2B, 0xAA, 0x20, 0x00, 0xFF, 0xFE, 0x01, 0x01, 0xC1, 0x05, 0x10, 0x1E, 0x1B ]);
    parser(null, rawFrame);
  },
  'Escaping (AP=2)': function(test) {
    test.expect(6); // 6 frames should be parsed
    var xbeeAPI = new xbee_api.XBeeAPI({ api_mode: 2 });
    var parser = xbeeAPI.rawParser();
    var parsed = 0;

    xbeeAPI.on("frame_object", function(frame) {
      if (parsed === 0) {
        test.equal(frame.id, 0x7D, "Parse id");
      } else if (parsed === 1) {
        test.equal(frame.id, 0x7E, "Parse id");
      } else if (parsed === 2) {
        test.equal(frame.id, 0x62, "Parse id");
      } else if (parsed === 3) {
        test.equal(frame.id, 0x64, "Parse id");
      } else if (parsed === 4) {
        test.equal(frame.id, 0x65, "Parse id");
      } else if (parsed === 5) {
        test.equal(frame.id, 0x66, "Parse id");
        test.done();
      }
      parsed++;
    });
    
    // ZigBee Transmit Status; 0x8B; here, frameId happens to be 7D and needs to be escaped
    var rawFrame0 = new Buffer([ 0x7e, 0x0, 0x7, 0x8b, 0x7d, 0x5d, 0x2a, 0x6a, 0x0, 0x0, 0x0, 0x63 ]);
    parser(null, rawFrame0);

    // ZigBee Transmit Status; 0x8B; here, frameId happens to be 7E and needs to be escaped
    var rawFrame1 = new Buffer([ 0x7e, 0x0, 0x7, 0x8b, 0x7d, 0x5e, 0x2a, 0x6a, 0x0, 0x0, 0x0, 0x62 ]);
    parser(null, rawFrame1);

    // ZigBee Transmit Status; 0x8B; here, checksum happebs to be 7E and needs to be escaped (frameId 62)
    var rawFrame2 = new Buffer([ 0x7e, 0x0, 0x7, 0x8b, 0x62, 0x2a, 0x6a, 0x0, 0x0, 0x0, 0x7d, 0x5e ]);
    parser(null, rawFrame2);

    // ZigBee Transmit Status; 0x8B; some frames without escaping (frameId = 0x64)
    var rawFrame3 = new Buffer([ 0x7e, 0x0, 0x7, 0x8b, 0x64, 0x2a, 0x6a, 0x0, 0x0, 0x0, 0x7c ]);
    parser(null, rawFrame3);

    // ZigBee Transmit Status; 0x8B; some frames without escaping (frameId = 0x65)
    var rawFrame4 = new Buffer([ 0x7e, 0x0, 0x7, 0x8b, 0x65, 0x2a, 0x6a, 0x0, 0x0, 0x0, 0x7b ]);
    parser(null, rawFrame4);

    // ZigBee Transmit Status; 0x8B; some frames without escaping (frameId = 0x66)
    var rawFrame5 = new Buffer([ 0x7e, 0x0, 0x7, 0x8b, 0x66, 0x2a, 0x6a, 0x0, 0x0, 0x0, 0x7a ]);
    parser(null, rawFrame5);
  }
};
