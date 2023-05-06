#include "shared.h"

void setupAp() {
  WiFi.mode(WIFI_AP_STA);
  WiFi.softAP(WIFI_AP_SSID, WIFI_AP_PASSWORD);
  Serial.print("AP IP: ");
  Serial.println(WiFi.softAPIP());
}

void setupWifi() {
  WiFi.begin(wifiSsid, wifiPassword);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting...");
  }

  WiFi.setAutoReconnect(true);
  WiFi.persistent(true);

  Serial.print("Connected. WiFi IP: ");
  Serial.println(WiFi.localIP());
}
