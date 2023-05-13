#include "shared.h"

WiFiClientSecure client;
HTTPClient http;

void setupHttpClient() { 
  client.setInsecure();

  if (WiFi.status() == WL_CONNECTED) {
    setTokenFromServer();
  }
}

void setTokenFromServer() {
  String payload;
  StaticJsonDocument<200> doc;
  doc["name"] = nodeName;
  doc["zone"] = nodeZone;
  serializeJson(doc, payload);

  String url = String(SWARM_URL);
  url += "/nodes/login";
  http.begin(client, url);
  http.addHeader("Content-Type", "application/json");
  http.POST(payload);
  payload = http.getString();
  http.end();

  deserializeJson(doc, payload);
  token = String(doc["token"]);
}

int sendHttpGet() {
  canSendRequest = false;

  StaticJsonDocument<200> doc;
  String payload;

  String url = String(SWARM_URL);
  url += "/nodes/humidity";
  http.begin(client, url);
  http.addHeader("Authorization", "Bearer " + token);
  http.GET();
  payload = http.getString();
  http.end();

  deserializeJson(doc, payload);
  return doc["humidity"].as<int>();
}

String sendHttpPost(String humidity) {
  canSendRequest = false;

  String payload;
  StaticJsonDocument<200> doc;
  doc["humidity"] = humidity;
  serializeJson(doc, payload);

  String url = String(SWARM_URL);
  url += "/nodes/humidity";
  http.begin(client, url);
  http.addHeader("Authorization", "Bearer " + token);
  http.addHeader("Content-Type", "application/json");
  http.POST(payload);
  payload = http.getString();
  http.end();

  return payload;
}
