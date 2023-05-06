#include <ArduinoJson.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>
#include <FS.h>

#define ONE_SECOND_TICKS 312500  // 312500 ticks * 3.2 us / tick = 1 second
#define WIFI_AP_SSID "Probe"
#define WIFI_AP_PASSWORD ""
#define SWARM_URL "https://irrigation-system.blospera.live/api"

#ifndef IRRIGATION_SYSTEM_SHARED
#define IRRIGATION_SYSTEM_SHARED

extern String wifiSsid;
extern String wifiPassword;
extern String nodeName;
extern String nodeZone;
extern String nodeType;

extern String token;
extern bool canSendRequest;

void setupTimerInterrupt();
void setupAp();
void setupWebServer();
void setupHttpClient();
bool setupWifi();

void handleClient();

void setTokenFromServer();
String sendHttpGet();
String sendHttpPost(String humidity);

#endif