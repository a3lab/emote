#pragma once

#include <WiFiManager.h>  // https://github.com/tzapu/WiFiManager

#if defined(ESP8266)
#include <ESP8266WiFi.h>
#else
#include <WiFi.h>
#endif

namespace comm {

bool initialize(bool resetSettings=false);

void update();

}