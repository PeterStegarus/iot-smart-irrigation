#include "shared.h"

String wifiSsid;
String wifiPassword;
String nodeName;
String nodeZone;

String token;
String response;
bool canSendRequest;

void setupVariables() { canSendRequest = false; }

void setupSerial() {
  Serial.begin(9600);
  Serial.println("Initialized");
}

void setup() {
  setupVariables();
  setupSerial();
  setupTimerInterrupt();
  setupAp();
  setupWebServer();
  setupHttpClient();
}

void loop() {
  handleClient();
  if (canSendRequest && !token.isEmpty()) {
    response = sendHttpGet();
    Serial.println(response);
  }
}
