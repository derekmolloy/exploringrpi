var xbee_api = require('xbee-api');
var util = require('util');
var Buffer = require('buffer').Buffer;
var C = xbee_api.constants;
var serial = chrome.serial;

var xbeeApp = angular.module('xbeeApp', []).controller("XBeeController", XBeeController);

xbeeApp.filter('reverse', function() {
  return function(items) {
    return items.slice().reverse();
  };
});

function XBeeController($scope) {
  $scope.connectionId = -1;

  $scope.atCMD = "NI";
  $scope.atPARAMS = "";
  $scope.atFRAME = [];

  $scope.port = "";
  $scope.ports = [];
  $scope.api_modes = [ "1", "2" ];
  $scope.api_mode = $scope.api_modes[0];
  $scope.bitrates = [ "1200", "2400", "4800", "9600", "19200", "38400", "57600", "115200" ];
  $scope.bitrate = $scope.bitrates[6];


  $scope.xbeeAPI = new xbee_api.XBeeAPI({
    api_mode: parseInt($scope.api_mode)
  });

  $scope.frame_objects = [];

  $scope.buildATFrame = function() {
    var data = $scope.xbeeAPI.buildFrame({
        type: C.FRAME_TYPE.AT_COMMAND,
        command: $scope.atCMD,
        commandParameter: $scope.atPARAMS
    });

    $scope.atFRAME = data;
  }

  $scope.send = function() {
    $scope.write($scope.atFRAME);
    $scope.buildATFrame();
  }

  $scope.setApiMode = function() {
    return [];
    if (!$scope.xbeeAPI.buildFrame) return;
    $scope.xbeeAPI.options.api_mode = $scope.api_mode;
  }

  $scope.refreshPorts = function() {
    serial.getPorts(function(ports) {
      $scope.$apply(function() {
        $scope.ports = ports;
        if ($scope.port == "" || ports.indexOf($scope.port) < 0) {
          if (ports.length > 0) $scope.port = ports[0];
          else $scope.port = "";
        }
      });
    });
  }

  $scope.open = function() {
    $scope.xbeeAPI.on("frame_object", function(frame) {
      $scope.$apply(function() {
        $scope.frame_objects.push(frame);
      });
    });

    var options = {
      bitrate: parseInt($scope.bitrate),
    };

    serial.open($scope.port, options, function(openInfo) {
      $scope.$apply(function() {
        $scope.connectionId = openInfo.connectionId;
      });
    })
  };

  $scope.close = function() {
    serial.close($scope.connectionId, function (closeInfo) {
      $scope.$apply(function() {
        $scope.connectionId = -1;
      });
    })
  };

  $scope.read = function() {
    if ($scope.connectionId > -1) {
      serial.read($scope.connectionId, 1, function(readInfo) {
        if (readInfo.bytesRead > 0) {
          //console.log(readInfo);
          $scope.readInterva
          $scope.xbeeAPI.parseRaw(toBuffer(readInfo.data));
          setTimeout($scope.read, 5);
        } else {
          setTimeout($scope.read, 20);
        }
      });
    } else {
      setTimeout($scope.read, 200);
    }
  };

  $scope.write = function(data) {
    serial.write($scope.connectionId, toArrayBuffer(data), function(writeInfo) {
      $scope.$apply(function() {
        //console.log(writeInfo);
      });
    });
  };

  $scope.init = function() {
    $scope.refreshPorts();
    $scope.read();
  };
}

function onError(e) {
  console.log(e);
}

function toArrayBuffer(buffer) {
    var ab = new ArrayBuffer(buffer.length);
    var view = new Uint8Array(ab);
    for (var i = 0; i < buffer.length; ++i) {
        view[i] = buffer[i];
    }
    return ab;
}

function toBuffer(ab) {
    var buffer = new Buffer(ab.byteLength);
    var view = new Uint8Array(ab);
    for (var i = 0; i < buffer.length; ++i) {
        buffer[i] = view[i];
    }
    return buffer;
}
