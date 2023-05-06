#include "shared.h"

String wifiSsid;
String wifiPassword;
String nodeName;
String nodeZone;
String nodeType;

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

void probeLoop() {
  if (canSendRequest && !token.isEmpty()) {
    response = sendHttpGet();
    Serial.println(response);
  }
}

void actuatorLoop() {
  if (canSendRequest && !token.isEmpty()) {
    String humidity = String(analogRead(A0));
    response = sendHttpPost(humidity);
    Serial.println(response);
  }
}

void loop() {
  handleClient();
  if (nodeType == "probe") {
    probeLoop();
  } else if (nodeType == "actuator") {
    actuatorLoop();
  }
}
