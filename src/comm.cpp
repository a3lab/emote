#include <comm.h>

namespace comm {

WiFiManager wm;

bool initialize(bool resetSettings) {
  WiFi.mode(WIFI_STA); // explicitly set mode, esp defaults to STA+AP
  // it is a good practice to make sure your code sets wifi mode how you want it.

  // reset settings - wipe stored credentials for testing
  // these are stored by the esp library
  if (resetSettings)
    wm.resetSettings();

  // Automatically connect using saved credentials,
  // if connection fails, it starts an access point with the specified name ( "AutoConnectAP"),
  // if empty will auto generate SSID, if password is blank it will be anonymous AP (wm.autoConnect())
  // then goes into a blocking loop awaiting configuration and will return success result

  bool res;
  // res = wm.autoConnect(); // auto generated AP name from chipid
  res = wm.autoConnect("emote-AP"); // anonymous ap
  // res = wm.autoConnect("emote-AP", "password");  // password protected ap

  return res;
}

void update() {
  wm.process();
}

}