#include <settings.h>

#define SETTINGS_FILENAME F("/settings.json")
#define FORMAT_SPIFFS_IF_FAILED true

namespace settings {

Settings _settings;

bool initialize() {
  if (!SPIFFS.begin(FORMAT_SPIFFS_IF_FAILED)) {
    Serial.println("SPIFFS Mount Failed");
    return false;
  }
  else
    return true;
}

bool Settings::load() {
  // Open file for reading
  File file = SPIFFS.open(SETTINGS_FILENAME);

  // Allocate a temporary JsonDocument
  // Don't forget to change the capacity to match your requirements.
  // Use https://arduinojson.org/v6/assistant to compute the capacity.
  StaticJsonDocument<512> doc;

  // Deserialize the JSON document
  DeserializationError error = deserializeJson(doc, file);
  if (error)
    Serial.println(F("Failed to read file, using default configuration"));

  // Copy values from the JsonDocument to the Config
  remoteIp   = doc["remote-ip"]   | "192.168.0.10";
  remotePort = doc["remote-port"] | 9000;
  localPort  = doc["local-port"]  | 8000;

  // Close the file (Curiously, File's destructor doesn't close the file)
  file.close();

  return (bool)error;
}

bool Settings::save() {
  // Delete existing file, otherwise the configuration is appended to the file
  SPIFFS.remove(SETTINGS_FILENAME);

  // Open file for writing
  File file = SPIFFS.open(SETTINGS_FILENAME, FILE_WRITE);
  if (!file) {
    Serial.println(F("Failed to create file"));
    return false;
  }

  // Allocate a temporary JsonDocument
  // Don't forget to change the capacity to match your requirements.
  // Use https://arduinojson.org/assistant to compute the capacity.
  StaticJsonDocument<256> doc;

  // Set the values in the document
  doc["remote-ip"]   = remoteIp;
  doc["remote-port"] = remotePort;
  doc["local-port"]  = localPort;

  // Serialize JSON to file
  if (serializeJson(doc, file) == 0) {
    Serial.println(F("Failed to write to file"));
    return false;
  }

  // Close the file
  file.close();

  return true;
}

Settings& instance() { return _settings; }

}