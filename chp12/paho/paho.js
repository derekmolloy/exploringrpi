// This example uses mqtt.js to upload the CPU temperature to IBM IoT
var mqtt      = require('mqtt');   // required module
var fs        = require('fs')

//var ORG       = '4wyix6';          // the organization ID
var ORG       = 'quickstart'
var TYPE      = 'RaspberryPi';     // the device type
var DEVID     = 'erpi01';          // the individual device id
var AUTHTOKEN = '5_e30j*GlG)zD(sq!V';  // the private auth token
var PORT      = 1883;              // reserved MQTT port
var BROKER    = ORG + '.messaging.internetofthings.ibmcloud.com';
var URL       = 'mqtt://' + BROKER + ':' + PORT; 
var CLIENTID  = 'd:' + ORG + ':' + TYPE + ':' + DEVID;
var AUTHMETH  = 'use-token-auth';  // using token authentication
var client    = mqtt.connect(URL, { clientId: CLIENTID, 
                username: AUTHMETH, password: AUTHTOKEN });
var TOPIC     = 'iot-2/evt/temperature/fmt/json';   // sending JSON payload
var CPUTEMP   = '/sys/class/thermal/thermal_zone0/temp'
console.log(URL);
console.log('Quickstart: https://quickstart.internetofthings.ibmcloud.com/#/device/'+DEVID+'/sensor/');

client.on('connect', function() {
   setInterval(function(){
      var tempStr = 'invalid';
      try {
         tempStr = fs.readFileSync(CPUTEMP, 'utf8');
      }
      catch (err){
         console.log('Failed to Read the CPU Temperature.');
      }
      var temp = parseFloat(tempStr) / 1000; 
      console.log('Sending Temp: ' + temp.toString() + '°C to IBM IoT');
      client.publish(TOPIC, '{"d":{"Temp":' + temp.toString() + '}}');
   }, 10000);  // publish data every ten seconds
});
