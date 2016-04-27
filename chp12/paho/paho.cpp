// Based on the Paho C code example
#include <iostream>
#include <sstream>
#include <fstream>
#include <string.h>
#include "MQTTClient.h"
#define  CPU_TEMP "/sys/class/thermal/thermal_zone0/temp"
using namespace std;

#define ADDRESS     "tcp://4wyix6.messaging.internetofthings.ibmcloud.com:1883"
#define CLIENTID    "d:4wyix6:RaspberryPi:erpi01"
#define AUTHMETHOD  "use-token-auth"
#define AUTHTOKEN   "5_e30j*GlG)zD(sq!V"
#define TOPIC       "iot-2/evt/status/fmt/json"
#define QOS         1
#define TIMEOUT     10000L

float getCPUTemperature() {          // get the CPU temperature
   int cpuTemp;                    // store as an int
   fstream fs;
   fs.open(CPU_TEMP, fstream::in); // read from the file
   fs >> cpuTemp;
   fs.close();
   return (((float)cpuTemp)/1000);
}

int main(int argc, char* argv[]) {
    MQTTClient client;
    MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
    MQTTClient_message pubmsg = MQTTClient_message_initializer;
    MQTTClient_deliveryToken token;

    MQTTClient_create(&client, ADDRESS, CLIENTID,
                      MQTTCLIENT_PERSISTENCE_NONE, NULL);
    conn_opts.keepAliveInterval = 20;
    conn_opts.cleansession = 1;
    conn_opts.username = AUTHMETHOD;
    conn_opts.password = AUTHTOKEN;
    int rc;
    if ((rc = MQTTClient_connect(client, &conn_opts)) != MQTTCLIENT_SUCCESS){
        cout << "Failed to connect, return code " << rc << endl;
        return -1;
    }

    stringstream message;
    message << "{\"d\":{\"Temp\":" << getCPUTemperature() << "}}";
    pubmsg.payload = (char*) message.str().c_str();
    pubmsg.payloadlen = message.str().length();
    pubmsg.qos = QOS;
    pubmsg.retained = 0;
    MQTTClient_publishMessage(client, TOPIC, &pubmsg, &token);
    cout << "Waiting for up to " << (int)(TIMEOUT/1000) <<
            " seconds for publication of " << message.str() <<
            " \non topic " << TOPIC << " for ClientID: " << CLIENTID << endl;
    rc = MQTTClient_waitForCompletion(client, token, TIMEOUT);
    cout << "Message with token " << (int)token << " delivered." << endl;
    MQTTClient_disconnect(client, 10000);
    MQTTClient_destroy(&client);
    return rc;
}
