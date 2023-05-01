#define ONE_SECOND_TICKS 312500 // 312500 ticks * 3.2 us / tick = 1 second

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

void setup() {
  setupVariables();
  setupSerial();
  setupTimerInterrupt();
}

void loop() {
  if (isHello) {
    hello();
  }
}

