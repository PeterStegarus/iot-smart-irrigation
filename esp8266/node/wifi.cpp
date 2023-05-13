#include "shared.h"

void setupAp() {
  WiFi.mode(WIFI_AP_STA);
  String apSsid = nodeName.isEmpty() ? WIFI_AP_SSID : "<" + nodeName + "> - <" + nodeZone + ">";
  WiFi.softAP(apSsid, WIFI_AP_PASSWORD);
  Serial.print("AP IP: ");
  Serial.println(WiFi.softAPIP());
}

bool setupWifi() {
  WiFi.begin(wifiSsid, wifiPassword);
  for (int i = 0; i < 15 && WiFi.status() != WL_CONNECTED; i++) {
    delay(1000);
    Serial.println("Connecting...");
  }

  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Connection failed");
    WiFi.disconnect();
    setupAp();
    return false;
  }

  WiFi.mode(WIFI_STA);
  setupAp();
  WiFi.setAutoReconnect(true);
  WiFi.persistent(true);

  Serial.print("Connected. WiFi IP: ");
  Serial.println(WiFi.localIP());
  return true;
}
