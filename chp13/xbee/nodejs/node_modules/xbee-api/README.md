# xbee-api [![Build Status](https://travis-ci.org/jankolkmeier/xbee-api.svg?branch=master)](https://travis-ci.org/jankolkmeier/xbee-api)

The [xbee-api](http://github.com/jankolkmeier/xbee-api/) [Node.js](http://nodejs.org/) module helps you
parsing and building API frames that are used to communicate with radio modules talking
the [802.15.4](http://en.wikipedia.org/wiki/IEEE_802.15.4) and [ZigBee](http://en.wikipedia.org/wiki/ZigBee) (including **ZNet**) protocol stacks.
These are most commonly used in Digi's [xbee](http://www.digi.com/xbee) radio modules, hence the name of this node module.
However, other devices with the mentioned protocol stacks should also be supported.
For details on supported firmwares and devices, see [here](#supported-firmwares-and-devices).

[xbee-api](http://github.com/jankolkmeier/xbee-api/) does *not* take care of the serial connection
itself, but it is easy to hook it up to modules such as [serialport](https://github.com/voodootikigod/node-serialport/).

Note that higher-level abstraction as currently provided in [svd-xbee](https://github.com/jankolkmeier/svd-xbee/) is not part of this module anymore. Consider 101100's promise-based [xbee-promise](https://github.com/101100/xbee-promise) module! Some work has been done towards more abstract, stream-based modules:  [xbee-stream](https://github.com/jankolkmeier/xbee-stream/) and [xbee-stream-nodes](https://github.com/jankolkmeier/xbee-stream-nodes/) (both WiP)

## GETTING STARTED
Install the module with: **[npm](https://npmjs.org/) install xbee-api**

```javascript
var xbee_api = require('xbee-api');
var C = xbee_api.constants;
var xbeeAPI = new xbee_api.XBeeAPI();

// Something we might want to send to an XBee...
var frame_obj = {
  type: C.FRAME_TYPE.AT_COMMAND,
  command: "NI",
  commandParameter: [],
};
console.log(xbeeAPI.buildFrame(frame_obj));
// <Buffer 7e 00 04 08 01 4e 49 5f>


// Something we might receive from an XBee...
var raw_frame = new Buffer([
	0x7E, 0x00, 0x13, 0x97, 0x55, 0x00, 0x13, 0xA2, 0x00, 0x40, 0x52, 0x2B,
	0xAA, 0x7D, 0x84, 0x53, 0x4C, 0x00, 0x40, 0x52, 0x2B, 0xAA, 0xF0
]);

console.log(xbeeAPI.parseFrame(raw_frame));
// { type: 151,
//   id: 85,
//   remote64: '0013a20040522baa',
//   remote16: '7d84',
//   command: 'SL',
//   commandStatus: 0,
//   commandData: [ 64, 82, 43, 170 ] }
```

**See the [Examples](#examples) section for more useful/practical examples!**

## SUPPORTED FIRMWARES AND DEVICES
This module supports the [802.15.4](http://en.wikipedia.org/wiki/IEEE_802.15.4) and [ZigBee](http://en.wikipedia.org/wiki/ZigBee) (including **ZNet**) protocol stacks.

From the XBee family, Series 1 (802.15.4) and Series 2 (ZNet 2.5 and ZigBee) modules are supported, since they come with firmwares talking either one of these stacks.

These documents are used as reference: [90000976_M.pdf (for Series 2)](http://ftp1.digi.com/support/documentation/90000976_M.pdf) and 
[90000982_M.pdf (for Series 1)](http://ftp1.digi.com/support/documentation/90000982_M.pdf). Some frame types are 802.15.4, ZNet or ZigBee specific. Be sure to use the correct ones for your module (as described in the documents and the list below). Also check out this [utility from Digi](http://ftp1.digi.com/support/utilities/digi_apiframes2.htm).

Modules must run in API mode. Both AP=1 (without escaping) and AP=2 (with escaping) modes are supported (set the api_mode parameter accordingly).

Since ZigBee is more robust and offers more features than ZNet (none of which are yet implemented here, though!), you might be interested in upgrading your Series 2 modules from ZNet 2.5 to ZigBee: [upgradingfromznettozb.pdf](ftp://ftp1.digi.com/support/documentation/upgradingfromznettozb.pdf).  

## SUPPORTED FRAME TYPES
The following frame types are implemented:

- 0x00: TX (Transmit) Request: 64-bit address (802.15.4)
- 0x01: TX (Transmit) Request: 16-bit address (802.15.4)
- 0x08: AT Command (802.15.4, ZNet, ZigBee)
- 0x09: AT Command Queue Parameter Value (802.15.4, ZNet, ZigBee)
- 0x17: Remote Command Request (802.15.4, ZNet, ZigBee)
- 0x21: Create Source Route (ZigBee)
- 0x80: RX (Receive) Packet: 64-bit Address (802.15.4)
- 0x81: RX (Receive) Packet: 16-bit Address (802.15.4)
- 0x82: RX (Receive) Packet: 64-bit Address IO (802.15.4)
- 0x83: RX (Receive) Packet: 16-bit Address IO (802.15.4)
- 0x88: AT Command Response (802.15.4, ZNet, ZigBee)
- 0x89: TX (Transmit) Status (802.15.4)
- 0x8A: Modem Status (802.15.4, ZNet, ZigBee)
- 0x97: Remote Command Response (802.15.4, ZNet, ZigBee)
- 0x10: ZigBee Transmit Request (ZNet, ZigBee)
- 0x11: Explicit Addressing ZigBee Command Frame (ZNet, ZigBee)
- 0x8B: ZigBee Transmit Status (ZNet, ZigBee)
- 0x90: ZigBee Receive Packet (AO=0) (ZNet, ZigBee)
- 0x91: ZigBee Explicit Rx Indicator (AO=1) (ZNet, ZigBee)
- 0x92: ZigBee IO Data Sample Rx Indicator (ZNet, ZigBee)
- 0x94: XBee Sensor Read Indicator (AO=0) (ZNet, ZigBee)
- 0x95: Node Identification Indicator (AO=0) (ZNet, ZigBee)
- 0xA1: Route Record Indicator (ZigBee)

### NOT IMPLEMENTED YET
These (more esoteric) frame types have not been implemented yet, [Open a new issue](https://github.com/jankolkmeier/xbee-api/issues/new) if you need something in particular: 

- 0x24: Register Joining Device (ZigBee)
- 0xA0: Over-the-Air Firmware Update Status (ZigBee)
- 0xA2: Device Authenticated Indicator (ZigBee)
- 0xA3: Many-to-One Route Request Indicator (ZigBee)
- 0xA4: Register Joining Device Status (ZigBee)
- 0xA5: Join Notification Status (ZigBee)

## DOCUMENTATION

### THE XBeeAPI CLASS

To get an instance `xbeeAPI` of the `XBeeAPI` class:
```javascript
var xbee_api = require('xbee-api');
var xbeeAPI = new xbee_api.XBeeAPI({
    // default options:
    api_mode: 1,      // [1, 2]; 1 is default, 2 is with escaping (set ATAP=2)
    module: "Any",    // ["802.15.4", "ZNet", "ZigBee", "Any"]; This does nothing, yet!
    raw_frames: false // [true, false]; If set to true, only raw byte frames are
                      // emitted (after validation) but not parsed to objects. 
});
```
In the following a documentation of all class methods.

#### xbeeAPI.buildFrame(frame)
Returns an API frame (buffer) created from the passed frame object. See ***Creating frames from objects to write to the XBee*** for details on how these passed objects are specified.

#### xbeeAPI.canParse(buffer)
Returns whether the library implements a parser for the frame contained in the provided buffer. The buffer only needs to contain up to the frame type segment to determine if it can be parsed, but `parseFrame()` will need a complete frame.

#### xbeeAPI.parseFrame(buffer)
Parses and returns a frame object from the buffer passed. Note that the buffer must be a complete frame, starting with the start byte and ending with the checksum byte. See ***Objects created from received API Frames*** for details on how the retured objects are specified.

#### xbeeAPI.rawParser()
Returns a parser function with the profile `function(emitter, buffer) {}`. This can be passed to a serial reader such as serialport. Note that XBeeAPI will not use the emitter to emit a parsed frame, but it's own emitter (see Event: 'frame_object').

#### xbeeAPI.parseRaw(buffer)
Parses data in the buffer, assumes it is comming directly from the XBee. If a complete frame is collected, it is emitted as Event: 'frame_object'.

#### Event: 'frame_object'
Is emitted whenever a complete frame is collected and parsed.

#### xbeeAPI.nextFrameId()
Increments the internal `frameId` counter and returns it. Useful for building requests where we want to identify the respective response frame later on.

### CREATING FRAMES FROM OBJECTS TO WRITE TO THE XBEE
There are four basic requests we can make to the XBee of which two are essentially the same. We can make (queued) command requests (0x08 and 0x09) and *remote* command request (0x17). These all behave pretty much the same as writing AT commands in AT mode. Lastly, there are transmit requests which can be used to send your own data to other devices (0x10, 0x00, 0x01 - depending on your firmware).

Note that many parameters have default values. For example, `destination16/64` default to their unknown/broadcast values. For non-single-byte parameters, you can choose to pass a string (`"fffe"`) or a byte array (`[0xff, 0xfe]`). We'd expect the latter to be slightly faster (but no benchmarks have been made).

#### 0x08: AT Command (802.15.4, ZNet, ZigBee)
```javascript
{
	type: 0x08, // xbee_api.constants.FRAME_TYPE.AT_COMMAND
	id: 0x52, // optional, nextFrameId() is called per default
	command: "NJ",
	commandParameter: [],
}
```
Execute the AT command set in `command`, optionally set a `comandParameter` value. An empty parameter usually queries the AT parameter value.

#### 0x09: AT Command Queue Parameter Value (802.15.4, ZNet, ZigBee)
```javascript
{
	type: 0x09, // xbee_api.constants.FRAME_TYPE.AT_COMMAND_QUEUE_PARAMETER_VALUE
	id: 0x01, // optional, nextFrameId() is called per default
	command: "BD",
	commandParameter: [ 0x07 ]
}
```
Pretty much the same as AT Command Requests, except that the commands are queued and applied at once when either an `AC` command is queued or a regular AT command request is sent.

#### 0x17: Remote Command Request (802.15.4, ZNet, ZigBee)
```javascript
{
	type: 0x17, // xbee_api.constants.FRAME_TYPE.REMOTE_AT_COMMAND_REQUEST
	id: 0x01, // optional, nextFrameId() is called per default
	destination64: "0013a20040401122",
	destination16: "fffe", // optional, "fffe" is default
	remoteCommandOptions: 0x02, // optional, 0x02 is default
	command: "BH",
	commandParameter: [ 0x01 ] // Can either be string or byte array.
}
```
Behaves just as AT Command Requests, with additional `destination64/16` parameters set to the remote node on which the AT command is to be executed.

#### 0x10: ZigBee Transmit Request (ZNet, ZigBee)
```javascript
{
	type: 0x10, // xbee_api.constants.FRAME_TYPE.ZIGBEE_TRANSMIT_REQUEST
	id: 0x01, // optional, nextFrameId() is called per default
	destination64: "0013a200400a0127",
	destination16: "fffe", // optional, "fffe" is default
	broadcastRadius: 0x00, // optional, 0x00 is default
	options: 0x00, // optional, 0x00 is default
	data: "TxData0A" // Can either be string or byte array.
}
```
Transmit your own `data` to a remote node.


#### 0x00: TX (Transmit) Request: 64-bit address (802.15.4)
```javascript
{
	type: 0x00, // xbee_api.constants.FRAME_TYPE.TX_REQUEST_64 
	id: 0x01, // optional, nextFrameId() is called per default
	destination64: "0013a200400a0127",
	options: 0x00, // optional, 0x00 is default
	data: "TxData0A" // Can either be string or byte array.
}
```
Transmit your own `data` to a remote node using a 64 bit address. This is for Series 1 / 802.15.4 modules only!

#### 0x01: TX (Transmit) Request: 16-bit address (802.15.4)
```javascript
{
	type: 0x01, // xbee_api.constants.FRAME_TYPE.TX_REQUEST_16 
	id: 0x01, // optional, nextFrameId() is called per default
	destination16: "ab00",
	options: 0x00, // optional, 0x00 is default
	data: "TxData0A" // Can either be string or byte array.
}
```
Transmit your own `data` to a remote node using a 16 bit address. This is for Series 1 / 802.15.4 modules only!

#### 0x11: Explicit Addressing ZigBee Command Frame (ZNet, ZigBee)
```javascript
{
    type: 0x11, // xbee_api.constants.FRAME_TYPE.ZIGBEE_TRANSMIT_REQUEST
    id: 0x01, // optional, nextFrameId() is called per default
    destination64: "0013a200400a0127", // default is broadcast address
    destination16: "fffe", // default is "fffe" (unknown/broadcast)
    sourceEndpoint: 0xA0,
    destinationEndpoint: 0xA1,
    clusterId: "1554",
    profileId: "C105",
    broadcastRadius: 0x00, // optional, 0x00 is default
    options: 0x00, // optional, 0x00 is default
    data: "TxData0A" // Can either be string or byte array.
}
```
Allows ZigBee application layer fields (endpoint and cluster ID) to be specified for a data transmission. Similar to the ZigBee Transmit Request, but also requires ZigBee application layer addressing fields to be specified (endpoints, cluster ID, profile ID). An Explicit Addressing Request API frame causes the module to send data as an RF packet to the specified destination, using the specified source and destination endpoints, cluster ID, and profile ID.

#### 0x21: Create Source Route (ZNet, ZigBee)
```javascript
{
    type: 0x21, // xbee_api.constants.FRAME_TYPE.CREATE_SOURCE_ROUTE
    destination64: "deadbeefcafebabe", // Must be a unicast address
    destination16: "adad", // Must be a unicast address
	addresses: [ // List of hops to destination (usually get these from route record indicator frames)
		'babe', // Hop closes to destination
		'beef',
		'cafe'  // Hop closest to source
	]
}
```
This sets a source route for sending a packet using a hard-coded route. See the ZigBee documentation on using source routing.

### OBJECTS CREATED FORM RECEIVED API FRAMES
Objects created from API frames that the XBee would recieve contain a `type` property that identifies the frame type. If the frame is a response to a query made earlier, the `id` that was used for that request is also included.

#### 0x88: AT Command Response (802.15.4, ZNet, ZigBee)
```javascript
{
	type: 0x88, // xbee_api.constants.FRAME_TYPE.AT_COMMAND_RESPONSE 
	id: 0x01,
	command: "BD",
	commandStatus: 0x00,
	commandData: []
}
```
This is a response to a AT command request, for example to query or change an AT parameter value on the XBee module. The command was, in this case, setting the `BD` parameter of module. The command status `0` means `OK` (see [Constants](#constants) for more), which means that the baud rate was changed successfully. 

#### 0x97: Remote Command Response (802.15.4, ZNet, ZigBee)
```javascript
{
	type: 0x97, // xbee_api.constants.FRAME_TYPE.REMOTE_COMMAND_RESPONSE 
	id: 0x01,
	remote64: "0013a20040522baa",
	remote16: "7d84",
	command: "SL",
	commandStatus: 0x00,
	commandData: [ 0x40, 0x52, 0x2b, 0xaa ]
}
```
This is a response to a *remote* AT command request, for example to query or change an AT parameter value on another device in the network. This seems to be a response from the node with the address `0013a20040522baa`. The requested command was, in this case, `SL`. The command status `0` means `OK`.


#### 0x8B: ZigBee Transmit Status (ZNet, ZigBee)
```javascript
{
	type: 0x8B, // xbee_api.constants.FRAME_TYPE.ZIGBEE_TRANSMIT_STATUS 
	id: 0x01,
	remote16: "7d84",
	transmitRetryCount: 0,
	deliveryStatus: 0,
	discoveryStatus: 1
}
```
This status is received after sending out a transmit request to the XBee (i.e. to send some text data to another module). The status contains the 16bit network address `remote16`, the number of transmission retries, information about whether delivery was successful and about any discoveries made.

#### 0x8A: Modem Status (802.15.4, ZNet, ZigBee)
```javascript
{
	type: 0x8a, // xbee_api.constants.FRAME_TYPE.MODEM_STATUS 
	modemStatus: 0x06
}
```
These statuses give information about the general operation of the XBee. See the [Constants](#constants) section for more.

#### 0x90: ZigBee Receive Packet (AO=0) (ZNet, ZigBee)
```javascript
{
	type: 0x90, // xbee_api.constants.FRAME_TYPE.ZIGBEE_RECEIVE_PACKET 
	remote64: "0013a20040522baa",
	remote16: "7d84",
	receiveOptions: 0x01,
	data: [ 0x52, 0x78, 0x44, 0x61, 0x74, 0x61 ]
}
```
This frame contains general data (such as text data) received from remote nodes.

#### 0x91: ZigBee Explicit Rx Indicator (AO=1) (ZNet, ZigBee)
```javascript
{
	type: 0x90, // xbee_api.constants.FRAME_TYPE.ZIGBEE_EXPLICIT_RX
	remote64: "0013a20040522baa",
	remote16: "7d84",
	sourceEndpoint: "e8",
	destinationEndpoint: "e8",
	clusterId: "0011",
	profileId: "c105",
	receiveOptions: 0x01,
	data: [ 0x52, 0x78, 0x44, 0x61, 0x74, 0x61 ]
}
```

#### 0x92: ZigBee IO Data Sample Rx Indicator (ZNet, ZigBee)
```javascript
{
	type: 0x92, // xbee_api.constants.FRAME_TYPE.ZIGBEE_IO_DATA_SAMPLE_RX 
	remote64: "0013a20040522baa",
	remote16: "7d84",
	receiveOptions: 0x01,
	numSamples: 1,
	digitalSamples: {
		"DIO2": 1,
		"DIO3": 0,
		"DIO4": 1
	},
	analogSamples: {
		"AD1": 644
	}
}
```
An I/O data sample that contains information about the state of the digital and analog I/O pins that are set to sample data. Here, pins `DIO2` & `DIO4` read `HIGH`, `DIO3` reads `LOW`, and `AD1` samples an analog voltage of `644mV`.

#### 0x94: XBee Sensor Read Indicator (AO=0) (ZNet, ZigBee)
```javascript
{
    type: 0x94, // xbee_api.constants.FRAME_TYPE.XBEE_SENSOR_READ 
    remote64: "0013a20040522baa",
    remote16: "7d84",
    sensors: 0x03,
    receiveOptions: 0x01,
    sensorValues: {
      AD0:   40, // A/D values in mV
      AD1: 4120,
      AD2: 4680,
      AD3: 1640,
      T: 362,
      temperature: 22.625,     // in °C, undefined if no temp sens
      relativeHumidity: 30.71, // in %, undefined if no hum sens
      trueHumidity: 30.54,     // in %, undefined if not both temp & hum
      waterPresent: false
    }
}
```
When the module receives a sensor sample (from a Digi 1-wire sensor adapter).

#### 0x95: Node Identification Indicator (AO=0) (ZNet, ZigBee)
```javascript
{
	type: 0x95, // xbee_api.constants.FRAME_TYPE.NODE_IDENTIFICATION
	sender64: "0013a20040522baa"
	sender16: "7d84"
	receiveOptions: 0x02
	remote64: "0013a20040522baa"
	remote16:"7d84"
	nodeIdentifier: "MY_ROUTER",
	remoteParent16: "fffe",
	deviceType: 0x01,
	sourceEvent: 0x01
}
```
Modules with the `JN` (Join Notification) parameter enabled will transmit a broadcast Node Identification Indicator packet on power up and when joining. This can also be sent when the D0 button is pressed. Which of these events occurred is set in the `sourceEvent` property. `sender64/16` here is the one from who the packet was received, whereas `remote64/16` is the identified node itself (may be the same).

#### 0x80: RX (Receive) Packet: 64-bit Address (802.15.4)
```javascript
{
	type: 0x80, // xbee_api.constants.FRAME_TYPE.RX_PACKET_64 
	remote64: "0013a20040522baa",
	rssi: 0x3f
	receiveOptions: 0x01,
	data: [ 0x52, 0x78, 0x44, 0x61, 0x74, 0x61 ]
}
```
This frame contains general data (such as text data) received from remote nodes(64 bit address). This is received only from Series 1 / 802.15.4 hardware.

#### 0x81: RX (Receive) Packet: 16-bit Address (802.15.4)
```javascript
{
	type: 0x81, // xbee_api.constants.FRAME_TYPE.RX_PACKET_16 
	remote64: "522b",
	rssi: 0x3f
	receiveOptions: 0x01,
	data: [ 0x52, 0x78, 0x44, 0x61, 0x74, 0x61 ]
}
```
This frame contains general data (such as text data) received from remote nodes(16 bit address). This is received only from Series 1 / 802.15.4 hardware.

#### 0x82: RX (Receive) Packet: 64-bit Address IO (802.15.4)
```javascript
{
	type: 0x82, // xbee_api.constants.FRAME_TYPE.RX_PACKET_64_IO 
	remote64: "0013a20040522baa",
	rssi: 0x3f
	receiveOptions: 0x01,
	data: [ 0x52, 0x78, 0x44, 0x61, 0x74, 0x61 ]
}
```
This frame contains I/O data received from remote nodes (64 bit address). This is received only from Series 1 / 802.15.4 hardware.

#### 0x83: RX (Receive) Packet: 16-bit Address IO (802.15.4)
```javascript
{
	type: 0x83, // xbee_api.constants.FRAME_TYPE.RX_PACKET_16_IO 
	remote64: "522b",
	rssi: 0x3f
	receiveOptions: 0x01,
	data: [ 0x52, 0x78, 0x44, 0x61, 0x74, 0x61 ]
}
```
This frame contains I/O data received from remote nodes (16 bit address). This is received only from Series 1 / 802.15.4 hardware.

#### 0x89: TX (Transmit) Status (802.15.4)
```javascript
{
	type: 0x89, // xbee_api.constants.FRAME_TYPE.TX_STATUS 
	id: 0x01,
	deliveryStatus: 0
}
```
When a TX Request is completed, the module sends a TX Status message. This message will indicate if the packet was transmitted successfully or if there was a failure (`deliveryStatus`).

### CONSTANTS
You don't have to remember the hex-numbers of the frame types, command options, status types, etc. Everything is defined in two-way constants. See the examples below:

```javascript
var xbee_api = require('xbee-api');
var C = xbee_api.constants;

// Frame types (frame.type):
C.FRAME_TYPE.ZIGBEE_TRANSMIT_REQUEST; // 0x10
C.FRAME_TYPE[0x10] // "ZigBee Transmit Request (0x10)";

// Command Status (frame.commandStatus)
C.COMMAND_STATUS.ERROR; // 0x01
C.COMMAND_STATUS[0x01]; // "(Error (0x01)"

// Discovery Status (frame.discoveryStatus)
C.DISCOVERY_STATUS.ADDRESS_DISCOVERY // 0x01
C.DISCOVERY_STATUS[0x01] // "Address Discovery (0x01)"

// Delivery Status (frame.deliveryStatus)
C.DELIVERY_STATUS.ADDRESS_NOT_FOUND // 0x24
C.DELIVERY_STATUS[0x24] // "Address Not Found (0x24)"

// Modem Status (frame.modemStatus)
C.MODEM_STATUS.JOINED_NETWORK // 0x02
C.MODEM_STATUS[0x02] // "Joined Network (0x02)"

// Receive Options (frame.receiveOptions)
C.RECEIVE_OPTIONS.PACKET_ACKNOWLEDGED // 0x01;
C.RECEIVE_OPTIONS[0x01] // "Packet Acknowledged (0x01)"

// Device Type (frame.deviceType)
C.DEVICE_TYPE.END_DEVICE // 0x02
C.DEVICE_TYPE[0x02] // "End Device (0x02)"
```

Please refer to `lib/constants.js` for a complete list, and the module documentation [here](http://ftp1.digi.com/support/documentation/90000976_M.pdf "http://ftp1.digi.com/support/documentation/90000976_M.pdf") for more explanation.

## EXAMPLES
To combine with [serialport](https://github.com/voodootikigod/node-serialport/), we use the rawParser(). Make sure to set your baudrate, AP mode and COM port. 
```javascript
var util = require('util');
var SerialPort = require('serialport').SerialPort;
var xbee_api = require('xbee-api');

var C = xbee_api.constants;

var xbeeAPI = new xbee_api.XBeeAPI({
  api_mode: 1
});

var serialport = new SerialPort("COM19", {
  baudrate: 57600,
  parser: xbeeAPI.rawParser()
});

serialport.on("open", function() {
  var frame_obj = { // AT Request to be sent to 
    type: C.FRAME_TYPE.AT_COMMAND,
    command: "NI",
    commandParameter: [],
  };

  serialport.write(xbeeAPI.buildFrame(frame_obj));
});

// All frames parsed by the XBee will be emitted here
xbeeAPI.on("frame_object", function(frame) {
	console.log(">>", frame);
});
```

To link a received frame object to a request we earlier sent, we have to set and remember the `frame.id` of our request. Then, when a new frame object is emitted, we could look it up and route the response back.

```javascript

var frameId = xbeeAPI.nextFrameId();
var frame_obj = {
	type: C.FRAME_TYPE.AT_COMMAND,
	id: frameId,
	command: "NI",
	commandParameter: []
};

serialport.write(xbeeAPI.buildFrame(frame_obj));

// All frames parsed by the XBee will be emitted here
xbeeAPI.on("frame_object", function(frame) {
	if (frame.id == frameId &&
	    frame.type == C.FRAME_TYPE.AT_COMMAND_RESPONSE) {
		// This frame is definitely the response!
		console.log("Node identifier:",
			String.fromCharCode(frame.commandData));
	} else {
		// This is some other frame
	}
});
```

See the [examples folder](https://github.com/jankolkmeier/xbee-api/tree/master/examples) in the repository for more examples.

### Use in Google Chrome
See this [example](http://github.com/jankolkmeier/xbee-api/tree/master/examples/chrome-tool) for how to use [xbee-api](https://github.com/jankolkmeier/xbee-api/) in Google Chrome using the [chrome.serial](http://developer.chrome.com/apps/serial.html) API.

## CONTRIBUTE
Feel free to send a pull request. There are nodeunit test in the `test/` folder (clone; run `npm install`; run `grunt nodeunit`).

## LICENSE
Copyright (c) 2013 Jan Kolkmeier  
Licensed under the MIT license.
