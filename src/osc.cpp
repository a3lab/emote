#include <osc.h>
#include <comm.h>

#define DEBUG_MODE false

namespace osc {

// UDP object.
WiFiUDP udp;

IPAddress remoteIp;  // remote IP of your computer
unsigned int remotePort = 9999;       // remote port to receive OSC
unsigned int localPort  = 8888;     // local port to listen for OSC packets (actually not used for sending)

bool initialize(const String& remoteIp, unsigned int remotePort, unsigned int localPort) {
  osc::remotePort = remotePort;
  osc::localPort = localPort;
  osc::remoteIp.fromString(remoteIp);
  return true;
}

void update() {
  OSCMessage messIn;
  IPAddress remote;

  if (receiveMessage(messIn, &remote)) {
    if (DEBUG_MODE) {
      Serial.println(F("OSC message received:"));
      messIn.send(Serial);
    }
  }
}

bool receiveMessage(OSCMessage& messIn, IPAddress* returnRemoteIP) {
    // if there's data available, read a packet
  int packetSize = udp.parsePacket();
  bool messageReceived = false;

  if (packetSize)
  {
    if (DEBUG_MODE) {
      Serial.print("Received packet of size ");
      Serial.println(packetSize);
      Serial.print("From ");
    }
    IPAddress remote = udp.remoteIP();
    if (DEBUG_MODE) {
      for (int i = 0; i < 4; i++)
      {
        Serial.print(remote[i], DEC);
        if (i < 3)
        {
          Serial.print(".");
        }
      }
      Serial.print(", port ");
      Serial.println(udp.remotePort());
    }

    // Read the packet
    while (packetSize--) messIn.fill(udp.read());

    switch(messIn.getError()) {
      case  OSC_OK:
        int messSize;
        if (DEBUG_MODE) Serial.println("no errors in packet");
        messSize = messIn.size();
        if (DEBUG_MODE) {
          Serial.print("messSize: ");
          Serial.println(messSize);
        }
        if (DEBUG_MODE) {
          char addressIn[64];
          messSize = messIn.getAddress(addressIn, 0, 64);
          Serial.print("messSize: ");
          Serial.println(messSize);
          Serial.print("address: ");
          Serial.println(addressIn);
        }
        messageReceived = true;

        Serial.println("Message received");
        // bndl.add("/received").add(boardName).add(remote[3]);
        // sendOscBundle(false, true); // force send so as to always respond

        break;
      case BUFFER_FULL:
        if (DEBUG_MODE) Serial.println("BUFFER_FULL error");
        break;
      case INVALID_OSC:
        if (DEBUG_MODE) Serial.println("INVALID_OSC error");
        break;
      case ALLOCFAILED:
        if (DEBUG_MODE) Serial.println("ALLOCFAILED error");
        break;
      case INDEX_OUT_OF_BOUNDS:
        if (DEBUG_MODE) Serial.println("INDEX_OUT_OF_BOUNDS error");
        break;
    }

    if (returnRemoteIP)
      *returnRemoteIP = remote;
      
  } //if (packetSize)
  
  return messageReceived;
}

void sendMessage(OSCMessage& messOut) {
  udp.beginPacket(remoteIp, remotePort);
  messOut.send(udp);
  udp.endPacket();
  messOut.empty();
}

}