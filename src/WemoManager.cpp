#include "WemoManager.h"
#include "WemoSwitch.h"
#include <functional>

#define WemoLibDebugPrint(X)	//Serial.print(X)
#define WemoLibDebugPrintln(X)	//Serial.println(X)

// Multicast declarations
IPAddress ipMulti(239, 255, 255, 250);
const unsigned int portMulti = 1900;
char packetBuffer[512];

#define MAX_SWITCHES 14
WemoSwitch switches[MAX_SWITCHES] = {};
int numOfSwitchs = 0;

//#define numOfSwitchs (sizeof(switches)/sizeof(Switch)) //array size

//<<constructor>>
WemoManager::WemoManager(){

}

//<<destructor>>
WemoManager::~WemoManager(){/*nothing to destruct*/}

bool WemoManager::begin(){
  boolean state = false;

  WemoLibDebugPrintln("Begin multicast ..");

  if(UDP.beginMulticast(WiFi.localIP(), ipMulti, portMulti)) {
    WemoLibDebugPrint("Udp multicast server started at ");
    WemoLibDebugPrint(ipMulti);
    WemoLibDebugPrint(":");
    WemoLibDebugPrintln(portMulti);

    state = true;
  }
  else{
    WemoLibDebugPrintln("Connection failed");
  }

  return state;
}

//Switch *ptrArray;

void WemoManager::addDevice(WemoSwitch& device) {
  WemoLibDebugPrint("Adding switch : ");
  WemoLibDebugPrint(device.getAlexaInvokeName());
  WemoLibDebugPrint(" index : ");
  WemoLibDebugPrintln(numOfSwitchs);

  switches[numOfSwitchs] = device;
  numOfSwitchs++;
}

void WemoManager::serverLoop(){

  int packetSize = UDP.parsePacket();
  if (packetSize > 0)
  {
    IPAddress senderIP = UDP.remoteIP();
    unsigned int senderPort = UDP.remotePort();

    // read the packet into the buffer
    UDP.read(packetBuffer, packetSize);

    // check if this is a M-SEARCH for WeMo device
    String request = String((char *)packetBuffer);
    // WemoLibDebugPrintln("----------");
    // WemoLibDebugPrintln(request);
    // WemoLibDebugPrintln("-----------");
    if(request.indexOf('M-SEARCH') > 0) {
        if((request.indexOf("urn:Belkin:device:**") > 0) || (request.indexOf("ssdp:all") > 0) || (request.indexOf("upnp:rootdevice") > 0)) {
          WemoLibDebugPrintln("Got UDP Belkin Request..");

          // int arrSize = sizeof(switchs) / sizeof(Switch);

          for(int n = 0; n < numOfSwitchs; n++) {
              WemoSwitch &sw = switches[n];

              if (&sw != NULL) {
                sw.respondToSearch(senderIP, senderPort);
              }
          }
        }
    }
  }

  for(int i = 0; i < numOfSwitchs; i++) {
      WemoSwitch &swit = switches[i];

      if (&swit != NULL) {
        swit.serverLoop();
      }
  }
}
