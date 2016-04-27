#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include "MQTTClient.h"
using namespace std;

#define ADDRESS     "tcp://quickstart.messaging.internetofthings.ibmcloud.com:1883"
#define CLIENTID    "a:quickstart:RaspberryPi:erpi02"
//#define AUTHMETHOD  "use-token-auth"
//#define AUTHTOKEN   "poO(V*mc1IGM7vMZ6W"
#define TOPIC       "iot-2/type/RaspberryPi/id/erpi01/evt/temperature/fmt/json"
#define QOS         1
#define TIMEOUT     10000L

volatile MQTTClient_deliveryToken deliveredtoken;

void delivered(void *context, MQTTClient_deliveryToken dt) {
    cout << "Message with token value " << (int)dt << "delivered" << endl;
    deliveredtoken = dt;
}

int msgarrvd(void *context, char *topicName, int topicLen, MQTTClient_message *message) {
    int i;
    char* payloadptr;

    cout << "Message arrived" << endl;
    cout << "   topic: " << topicName << endl;
    cout << "   message: ";
    payloadptr = (char *) message->payload;
    for(i=0; i<message->payloadlen; i++) {
        cout << *payloadptr++;
    }
    cout << endl;
    MQTTClient_freeMessage(&message);
    MQTTClient_free(topicName);
    return 1;
}

void connlost(void *context, char *cause)
{
    printf("\nConnection lost\n");
    printf("     cause: %s\n", cause);
}

int main(int argc, char* argv[]) {
    MQTTClient client;
    MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
    int rc, ch;
    MQTTClient_create(&client, ADDRESS, CLIENTID,
        MQTTCLIENT_PERSISTENCE_NONE, NULL);
    conn_opts.keepAliveInterval = 20;
    conn_opts.cleansession = 1;
//    conn_opts.username = AUTHMETHOD;
//    conn_opts.password = AUTHTOKEN;
    MQTTClient_setCallbacks(client, NULL, connlost, msgarrvd, delivered);
    if ((rc = MQTTClient_connect(client, &conn_opts)) != MQTTCLIENT_SUCCESS){
        cout << "Failed to connect, return code" << rc << endl;
        return -1;
    }
    cout << "Subscribing to topic " << TOPIC << endl;
    cout << " for client " << CLIENTID << " using QoS " << QOS << endl;
    cout << " Press Q<Enter> to quit " << endl;
    MQTTClient_subscribe(client, TOPIC, QOS);
    do {
        ch = getchar();
    } while(ch!='Q' && ch != 'q');
    MQTTClient_disconnect(client, 10000);
    MQTTClient_destroy(&client);
    return rc;
}
