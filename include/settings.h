#pragma once

#include <ArduinoJson.h>
#include <FS.h>
#include <SPIFFS.h>

namespace settings {

struct Settings {
  String remoteIp;
  int remotePort;
  int localPort;

  bool load();
  bool save();
};

bool initialize();

// Singleton.
Settings& instance();

}