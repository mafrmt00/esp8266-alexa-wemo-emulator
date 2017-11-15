#ifndef WemoSwitch_H
#define WemoSwitch_H

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <WiFiUdp.h>
#include "CallbackFunction.h"


class WemoSwitch {
private:
        ESP8266WebServer *server = NULL;
        WiFiUDP UDP;
        String serial;
        String persistent_uuid;
        String device_name;
        unsigned int localPort;
		bool bCurrentState;
		StatusCallbackFunction StatusCallback;
        CallbackFunction onCallback;
        CallbackFunction offCallback;

		void Init(unsigned int port);
        void startWebServer();
        void handleEventservice();
        void handleUpnpControl();
        void handleRoot();
        void handleSetupXml();
public:
        WemoSwitch();
        WemoSwitch(String alexaInvokeName, unsigned int port, CallbackFunction onCallback, CallbackFunction offCallback);
		WemoSwitch(String alexaInvokeName, unsigned int port, CallbackFunction oncb, CallbackFunction offcb, StatusCallbackFunction statecb);
        ~WemoSwitch();
        String getAlexaInvokeName();
        void serverLoop();
        void respondToSearch(IPAddress& senderIP, unsigned int senderPort);
};

#endif
