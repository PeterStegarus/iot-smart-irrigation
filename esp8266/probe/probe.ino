#include <ESP8266HTTPClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>
#include <FS.h>

#define WIFI_AP_SSID "Probe"
#define WIFI_AP_PASSWORD ""

#define URL "https://bamboohut.studio/api/properties"

#define ONE_SECOND_TICKS 312500  // 312500 ticks * 3.2 us / tick = 1 second

ESP8266WebServer server(80);

WiFiClientSecure client;
HTTPClient http;

int status;  // 0 - AP mode; 1 - connecting to wifi; 2 - connected to wifi
String wifiSsid;
String wifiPassword;

bool isHello;
int helloCount;

void hello() {
  isHello = false;
  Serial.println("Hello " + String(++helloCount));
}

void ICACHE_RAM_ATTR onTimerISR() { isHello = true; }

void setupVariables() {
  status = 0;
  isHello = false;
  helloCount = 0;
}

void setupSerial() {
  Serial.begin(9600);
  Serial.println("Init");
}

void setupTimerInterrupt() {
  timer1_attachInterrupt(onTimerISR);
  timer1_enable(TIM_DIV256, TIM_EDGE, TIM_LOOP);  // 1 tick = 3.2us
  timer1_write(5 * ONE_SECOND_TICKS);
}

void setupWifi() {
  server.handleClient();
  WiFi.begin(wifiSsid, wifiPassword);

  while (WiFi.status() != WL_CONNECTED) {
    server.handleClient();
    delay(250);
    Serial.println("Connecting...");
  }

  status = 2;
  server.close();
  Serial.println("Connected.");
}

void setupHttpClient() { client.setInsecure(); }

void setupAp() {
  WiFi.mode(WIFI_AP);
  WiFi.softAP(WIFI_AP_SSID, WIFI_AP_PASSWORD);
  Serial.print("IP: ");
  Serial.println(WiFi.softAPIP());
}

void handleRoot() {
  server.sendHeader("Location", String("/index.html"), true);
  server.send(302, "text/plain", "");
}

void handleSubmit() {
  wifiSsid = server.arg("ssid");
  wifiPassword = server.arg("password");
  status = 1;
  server.sendHeader("Location", String("/connecting.html"), true);
  server.send(302, "text/plain", "");
}

void setupWebServer() {
  SPIFFS.begin();
  server.serveStatic("/index.html", SPIFFS, "/index.html");
  server.serveStatic("/connecting.html", SPIFFS, "/connecting.html");
  server.serveStatic("/style.css", SPIFFS, "/style.css");
  server.on("/", handleRoot);
  server.on("/submit", handleSubmit);
  server.begin();
}

void setup() {
  setupVariables();
  setupSerial();
  setupTimerInterrupt();
  setupAp();
  setupWebServer();
  setupHttpClient();
}

String sendHttp() {
  String payload;
  http.begin(client, URL);
  int httpCode = http.GET();

  Serial.println("HTTP Code: " + String(httpCode));
  if (httpCode > 0) {
    payload = http.getString();
  }

  http.end();

  return payload;
}

void loop() {
  switch (status) {
    case 0:
      server.handleClient();
      break;
    case 1:
      setupWifi();
    default:
      if (isHello) {
        hello();
        Serial.println(sendHttp());
      }
  }
}
