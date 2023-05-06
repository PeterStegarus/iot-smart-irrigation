#include "shared.h"

void ICACHE_RAM_ATTR onTimerISR() { canSendRequest = true; }

void setupTimerInterrupt() {
  timer1_attachInterrupt(onTimerISR);
  timer1_enable(TIM_DIV256, TIM_EDGE, TIM_LOOP);  // 1 tick = 3.2us
  timer1_write(5 * ONE_SECOND_TICKS);
}
