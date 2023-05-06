#include <ArduinoJson.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>
#include <FS.h>

#define ONE_SECOND_TICKS 312500  // 312500 ticks * 3.2 us / tick = 1 second
#define WIFI_AP_SSID "Probe"
#define WIFI_AP_PASSWORD ""

const String SWARM_URL = "https://irrigation-system.blospera.live/api";

ESP8266WebServer server(80);
WiFiClientSecure client;
HTTPClient http;

String wifiSsid;
String wifiPassword;
String nodeName;
String nodeZone;

bool canSendRequest;
String response;
String token;

void ICACHE_RAM_ATTR onTimerISR() { canSendRequest = true; }

void setupVariables() { canSendRequest = false; }

void setupSerial() {
  Serial.begin(9600);
  Serial.println("Initialized");
}

void setupTimerInterrupt() {
  timer1_attachInterrupt(onTimerISR);
  timer1_enable(TIM_DIV256, TIM_EDGE, TIM_LOOP);  // 1 tick = 3.2us
  timer1_write(5 * ONE_SECOND_TICKS);
}

void setupAp() {
  WiFi.mode(WIFI_AP_STA);
  WiFi.softAP(WIFI_AP_SSID, WIFI_AP_PASSWORD);
  Serial.print("AP IP: ");
  Serial.println(WiFi.softAPIP());
}

void setupWebServer() {
  SPIFFS.begin();
  server.serveStatic("/index.html", SPIFFS, "/index.html");
  server.serveStatic("/style.css", SPIFFS, "/style.css");
  server.on("/", handleRoot);
  server.on("/submit", handleSubmit);
  server.begin();
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

void setupHttpClient() { client.setInsecure(); }

void handleRoot() {
  server.sendHeader("Location", String("/index.html"), true);
  server.send(302, "text/plain", "");
}

void setTokenFromServer() {
  String payload;
  StaticJsonDocument<200> doc;
  doc["name"] = nodeName;
  doc["zone"] = nodeZone;
  serializeJson(doc, payload);

  http.begin(client, SWARM_URL + "/nodes/login");
  http.addHeader("Content-Type", "application/json");
  int httpCode = http.POST(payload);
  payload = http.getString();
  http.end();

  deserializeJson(doc, payload);
  token = String(doc["token"]);
}

void handleSubmit() {
  wifiSsid = server.arg("ssid");
  wifiPassword = server.arg("password");
  nodeName = server.arg("name");
  nodeZone = server.arg("zone");

  setupWifi();
  setTokenFromServer();

  server.send(200, "text/plain", "");
}

void setup() {
  setupVariables();
  setupSerial();
  setupTimerInterrupt();
  setupAp();
  setupWebServer();
  setupHttpClient();

  // Temporary
  wifiSsid = "Steag";
  wifiPassword = "";
  nodeName = "probe-1";
  nodeZone = "zone-1";
  setupWifi();
  setTokenFromServer();
}

String sendHttpGet() {
  canSendRequest = false;

  http.begin(client, SWARM_URL + "/nodes/humidity");
  http.addHeader("Authorization", "Bearer " + token);
  int httpCode = http.GET();
  String payload = http.getString();
  http.end();

  return payload;
}

void loop() {
  server.handleClient();
  if (canSendRequest && !token.isEmpty()) {
    response = sendHttpGet();
    Serial.println(response);
  }
}
