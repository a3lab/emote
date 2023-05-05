#include <Arduino.h>
#include <BioData.h>
#include <Heart.h>

#include <comm.h>
#include <osc.h>
#include <settings.h>

#include <PlaquetteLib.h>

pq::Metro trigger(0.001);

Heart heart(A0);

void setup() {
  pq::Plaquette.begin();
  pq::beginSerial(115200);

  if (!comm::initialize()) {
    pq::println("There was a problem, rebooting.");
    ESP.restart();
  }

  pq::println("Readings settings");
  
  settings::initialize();

  settings::Settings& settingsInst = settings::instance();
  settingsInst.load();

  osc::initialize(settingsInst.remoteIp, settingsInst.remotePort, settingsInst.localPort);

  heart.reset();
}

void loop() {
  pq::Plaquette.step();
  heart.update();

  comm::update();
  // osc::update();

  // put your main code here, to run repeatedly:
  if (trigger) {
    OSCMessage msg("/data");
    msg.add(heart.getRaw());
    msg.add(heart.getNormalized());
    msg.add(heart.getBPM());
    osc::sendMessage(msg);
  }
}