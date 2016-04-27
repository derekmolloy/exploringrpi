/*
 * xbee-api
 * https://github.com/jouz/xbee-api
 *
 * Copyright (c) 2013 Jan Kolkmeier
 * Licensed under the MIT license.
 */

'use strict';

var assert = require('assert'),
    C = require('./constants');

var frame_builder = module.exports = {
  frameId: 0,
  nextFrameId: function nextFrameId() {
    this.frameId = this.frameId > 0xff ? 1 : ++this.frameId;
    return this.frameId;
  },

  getFrameId: function getFrameId(frame) {
    assert(frame, 'Frame parameter must be supplied');
    var id = frame.id || (frame.id !== 0 && this.nextFrameId()) || frame.id;
    return frame.id = id;
  }
};


// Appends data provided as Array, String, or Buffer
function appendData(data, builder) {
  if(Array.isArray(data)) {
    data = new Buffer(data);
  } else {
    data = new Buffer(data, 'ascii');
  }

  builder.appendBuffer(data);
}

frame_builder[C.FRAME_TYPE.AT_COMMAND] = 
frame_builder[C.FRAME_TYPE.AT_COMMAND_QUEUE_PARAMETER_VALUE] = function(frame, builder) {
  builder.appendUInt8(frame.type);
  builder.appendUInt8(this.getFrameId(frame));
  builder.appendString(frame.command, 'ascii');
  appendData(frame.commandParameter, builder);
};

frame_builder[C.FRAME_TYPE.REMOTE_AT_COMMAND_REQUEST] = function(frame, builder) {
  builder.appendUInt8(frame.type);
  builder.appendUInt8(this.getFrameId(frame));
  builder.appendString(frame.destination64 || C.UNKNOWN_64, 'hex');
  builder.appendString(frame.destination16 || C.UNKNOWN_16, 'hex');
  builder.appendUInt8(frame.remoteCommandOptions || 0x02);
  builder.appendString(frame.command, 'ascii');
  appendData(frame.commandParameter, builder);
};

frame_builder[C.FRAME_TYPE.ZIGBEE_TRANSMIT_REQUEST] = function(frame, builder) {
  builder.appendUInt8(frame.type);
  builder.appendUInt8(this.getFrameId(frame));
  builder.appendString(frame.destination64 || C.UNKNOWN_64, 'hex');
  builder.appendString(frame.destination16 || C.UNKNOWN_16, 'hex');
  builder.appendUInt8(frame.broadcastRadius || 0x00);
  builder.appendUInt8(frame.options || 0x00);
  appendData(frame.data, builder);
};


frame_builder[C.FRAME_TYPE.EXPLICIT_ADDRESSING_ZIGBEE_COMMAND_FRAME] = function(frame, builder) {
  builder.appendUInt8(frame.type);
  builder.appendUInt8(this.getFrameId(frame));
  builder.appendString(frame.destination64 || C.UNKNOWN_64, 'hex');
  builder.appendString(frame.destination16 || C.UNKNOWN_16, 'hex');
  builder.appendUInt8(frame.sourceEndpoint);
  builder.appendUInt8(frame.destinationEndpoint);
  builder.appendUInt16BE(frame.clusterId);
  builder.appendUInt16BE(frame.profileId);
  builder.appendUInt8(frame.broadcastRadius || 0x00);
  builder.appendUInt8(frame.options || 0x00);
  appendData(frame.data, builder);
};

frame_builder[C.FRAME_TYPE.CREATE_SOURCE_ROUTE] = function(frame, builder) {
  builder.appendUInt8(frame.type);
  builder.appendUInt8(0); // Frame ID is always zero for this
  builder.appendString(frame.destination64, 'hex');
  builder.appendString(frame.destination16, 'hex');
  builder.appendUInt8(0); // Route command options always zero
  builder.appendUInt8(frame.addresses.length); // Number of hops
  for (var i = 0; i < frame.addresses.length; i++) {
    builder.appendUInt16BE(frame.addresses[i], 'hex');
  }
};

frame_builder[C.FRAME_TYPE.TX_REQUEST_64] = function(frame, builder) {
  builder.appendUInt8(frame.type);
  builder.appendUInt8(this.getFrameId(frame));
  builder.appendString(frame.destination64 || C.UNKNOWN_64, 'hex');
  builder.appendUInt8(frame.options || 0x00);
  appendData(frame.data, builder);
};

frame_builder[C.FRAME_TYPE.TX_REQUEST_16] = function(frame, builder) {
  builder.appendUInt8(frame.type);
  builder.appendUInt8(this.getFrameId(frame));
  builder.appendString(frame.destination16 || C.BROADCAST_16_XB, 'hex');
  builder.appendUInt8(frame.options || 0x00);
  appendData(frame.data, builder);
};
