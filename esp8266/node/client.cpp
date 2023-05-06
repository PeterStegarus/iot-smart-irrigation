#include "shared.h"

WiFiClientSecure client;
HTTPClient http;

void setupHttpClient() { client.setInsecure(); }

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

String sendHttpGet() {
  canSendRequest = false;

  String url = String(SWARM_URL);
  url += "/nodes/humidity";
  http.begin(client, url);
  http.addHeader("Authorization", "Bearer " + token);
  http.GET();
  String payload = http.getString();
  http.end();

  return payload;
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
