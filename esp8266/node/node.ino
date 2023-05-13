#include "shared.h"

String wifiSsid;
String wifiPassword;
String nodeName;
String nodeZone;
String nodeType;

String token;
String response;
int humidity;
bool canSendRequest;

void setupVariables() {
  canSendRequest = false;
  pinMode(5, OUTPUT);
}

void setupSerial() {
  Serial.begin(9600);
  Serial.println("Initialized");
}

void setupCredentialsFromFile() {
  SPIFFS.begin();
  File file = SPIFFS.open("credentials.txt", "r");
  if (!file) {
    Serial.println("Failed to open file for reading!");
    return;
  }

  wifiSsid = file.readStringUntil('\n');
  wifiSsid.trim();
  wifiPassword = file.readStringUntil('\n');
  wifiPassword.trim();
  nodeName = file.readStringUntil('\n');
  nodeName.trim();
  nodeZone = file.readStringUntil('\n');
  nodeZone.trim();
  nodeType = file.readStringUntil('\n');
  nodeType.trim();
  file.close();

  setupWifi();
}

void setup() {
  Serial.println("Init");
  setupVariables();
  setupSerial();
  setupTimerInterrupt();
  setupAp();
  setupCredentialsFromFile();
  setupWebServer();
  setupHttpClient();
}

void probeLoop() {
  humidity = analogRead(A0);
  Serial.print(humidity);
  Serial.print(" ");
  response = sendHttpPost(String(humidity));
  Serial.println(response);
}

void actuatorLoop() {
  humidity = sendHttpGet();
  Serial.println(humidity);
  digitalWrite(5, humidity > 1000 ? HIGH : LOW);
  delay(500);
  digitalWrite(5, LOW);
}

void loop() {
  handleClient();
  if (canSendRequest && !token.isEmpty())
  if (nodeType == "probe") {
    probeLoop();
  } else if (nodeType == "actuator") {
    actuatorLoop();
  }
}
