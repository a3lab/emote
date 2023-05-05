#pragma once

#include <WiFiUdp.h>

#include <OSCMessage.h>

namespace osc {


bool initialize(const String& remoteIp, unsigned int remotePort, unsigned int localPort);
void update();

bool receiveMessage(OSCMessage& messIn, IPAddress* returnRemoteIP=0);
void sendMessage(OSCMessage& messOut);

}