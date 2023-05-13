#include "shared.h"

ESP8266WebServer server(80);

void handleRoot() {
  server.sendHeader("Location", String("/index.html"), true);
  server.send(302, "text/plain", "");
}

void handleSubmit() {
  wifiSsid = server.arg("ssid");
  wifiPassword = server.arg("password");
  nodeName = server.arg("name");
  nodeZone = server.arg("zone");
  nodeType = server.arg("type") == "on" ? "actuator" : "probe";

  File file = SPIFFS.open("credentials.txt", "w");
  if (!file) {
    Serial.println("Failed to open file for writing!");
  } else {
    file.println(wifiSsid);
    file.println(wifiPassword);
    file.println(nodeName);
    file.println(nodeZone);
    file.println(nodeType);
    file.close();
  }

  bool connected = setupWifi();
  if (connected) {
    setTokenFromServer();
    server.send(200, "text/plain", "");
  } else {
    server.send(401, "text/plain", "");
  }
}

void setupWebServer() {
  server.serveStatic("/index.html", SPIFFS, "/index.html");
  server.serveStatic("/style.css", SPIFFS, "/style.css");
  server.on("/", handleRoot);
  server.on("/submit", handleSubmit);
  server.begin();
}

void handleClient() { server.handleClient(); }