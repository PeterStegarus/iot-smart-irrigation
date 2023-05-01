#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

#define WIFI_SSID "Steag"
#define WIFI_PASSWORD ""
#define URL "https://bamboohut.studio/api/properties"
#define ONE_SECOND_TICKS 312500 // 312500 ticks * 3.2 us / tick = 1 second


WiFiClientSecure client;
HTTPClient http;
bool isHello;
int helloCount;

void hello() {
  isHello = false;
  Serial.println("Hello " + String(++helloCount));
}

void ICACHE_RAM_ATTR onTimerISR(){
  isHello = true;
}

void setupVariables() {
  isHello = false;
  helloCount = 0;
}

void setupSerial() {
  Serial.begin(9600);
  Serial.println("Init");
}

void setupTimerInterrupt() {
  timer1_attachInterrupt(onTimerISR);
  timer1_enable(TIM_DIV256, TIM_EDGE, TIM_LOOP); // 1 tick = 3.2us
  timer1_write(5 * ONE_SECOND_TICKS);
}

void setupWifi() {
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print("Connecting..");
  }
}

void setupHttpClient() {
  client.setInsecure();
}

void setup() {
  setupVariables();
  setupSerial();
  setupTimerInterrupt();
  setupWifi();
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
  if (isHello) {
    hello();
    Serial.println(sendHttp());
  }
}

