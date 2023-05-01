#include <Ticker.h>

Ticker blinker;
bool isHello;
int helloCount;

void setHello() {
  isHello = true;
}

void hello() {
  isHello = false;
  Serial.println("Hello " + String(++helloCount));
}

void setup() {
  isHello = false;
  helloCount = 0;

  Serial.begin(9600);
  Serial.println("Init");
  blinker.attach(5, hello);
}

void loop() {
  if (isHello) {
    hello();
  }
}

