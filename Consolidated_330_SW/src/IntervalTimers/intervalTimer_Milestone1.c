#include "intervalTimer.h"
#include "xil_io.h"
#include <stdio.h>

#define TCR0_OFFSET 0x08
#define TCR1_OFFSET 0x18

u32 readTimer1Register(uint32_t registerOffset) {
  uint32_t address = XPAR_AXI_TIMER_0_BASEADDR + registerOffset;
  return Xil_In32(address);
}

#define DELAY_COUNT 3
void waitALongTime() {
  volatile int32_t a = 0;
  int32_t i, j;
  for (i=0; i<DELAY_COUNT; i++)
    for (j=0; j<INT32_MAX; j++)
      a++;
}

int main() {
  intervalTimer_init(0);
  intervalTimer_reset(0);
  // Show that the timer is reset.
  printf("timer_0 TCR0 should be 0 at this point:%ld\n\r", readTimer1Register(TCR0_OFFSET));
  printf("timer_0 TCR1 should be 0 at this point:%ld\n\r", readTimer1Register(TCR1_OFFSET));
  intervalTimer_start(0);
  // Show that the timer is running.
  printf("The following register values should be changing while reading them.\n\r");
  printf("timer_0 TCR0 should be changing at this point:%ld\n\r", readTimer1Register(TCR0_OFFSET));
  printf("timer_0 TCR0 should be changing at this point:%ld\n\r", readTimer1Register(TCR0_OFFSET));
  printf("timer_0 TCR0 should be changing at this point:%ld\n\r", readTimer1Register(TCR0_OFFSET));
  printf("timer_0 TCR0 should be changing at this point:%ld\n\r", readTimer1Register(TCR0_OFFSET));
  printf("timer_0 TCR0 should be changing at this point:%ld\n\r", readTimer1Register(TCR0_OFFSET));
  // Wait about 2 minutes so that you roll over to TCR1.
  // If you don't see a '1' in TCR1 after this long wait you probably haven't programmed the timer correctly.
  waitALongTime();
  printf("timer_0 TCR0 value after wait:%lx\n\r", readTimer1Register(TCR0_OFFSET));
  printf("timer_0 TCR1 should have changed at this point:%ld\n\r", readTimer1Register(TCR1_OFFSET));
}