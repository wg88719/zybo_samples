//*****************************************************************************
// Luke Hsiao
// 11 May 2015
// Main driver for Tic Tac Toe
//*****************************************************************************

#include "ticTacToeDisplay.h"
#include "minimax.h"
#include "ticTacToeControl.h"
#include <stdio.h>
#include "supportFiles/leds.h"
#include "supportFiles/globalTimer.h"
#include "supportFiles/interrupts.h"
#include "supportFiles/utils.h"
#include <stdbool.h>
#include <stdint.h>
#include "supportFiles/display.h"
#include "xparameters.h"
#include "intervalTimer.h"

#define TOTAL_SECONDS 60
// This period was chosen because the tick function function takes
// 367ms to run when running on an empty board
#define TIMER_PERIOD .2  // 100ms period
#define TIMER_CLOCK_FREQUENCY ((XPAR_CPU_CORTEXA9_0_CPU_CLK_FREQ_HZ) / 2)
#define TIMER_LOAD_VALUE (((TIMER_PERIOD) * (TIMER_CLOCK_FREQUENCY)) - 1.0)

/**
 * More advanced test that uses timer interrupts
 */
void test_Full() {
  // Initialize the GPIO LED driver and print out an error message if it fails (argument = true).
  // You need to init the LEDs so that LED4 can function as a heartbeat.
  leds_init(true);
  // Init all interrupts (but does not enable the interrupts at the devices).
  // Prints an error message if an internal failure occurs because the argument = true.
  interrupts_initAll(true);
  interrupts_setPrivateTimerLoadValue(TIMER_LOAD_VALUE);
  u32 privateTimerTicksPerSecond = interrupts_getPrivateTimerTicksPerSecond();
  printf("private timer ticks per second: %ld\n\r", privateTimerTicksPerSecond);
  // Allow the timer to generate interrupts.
  interrupts_enableTimerGlobalInts();
  // Initialization of the clock display is not time-dependent, do it outside of the state machine.
  //ticTacToeDisplay_init();
  // Keep track of your personal interrupt count. Want to make sure that you don't miss any interrupts.
  int32_t personalInterruptCount = 0;
  // Start the private ARM timer running.
  interrupts_startArmPrivateTimer();
  // Enable interrupts at the ARM.
  interrupts_enableArmInts();
  // interrupts_isrInvocationCount() returns the number of times that the timer ISR was invoked.
  // This value is maintained by the timer ISR. Compare this number with your own local
  // interrupt count to determine if you have missed any interrupts.
  double duration_max = 0.0;
  while (interrupts_isrInvocationCount() < (TOTAL_SECONDS * privateTimerTicksPerSecond)) {
    if (interrupts_isrFlagGlobal) {  // This is a global flag that is set by the timer interrupt handler.
      // Count ticks.
      personalInterruptCount++;


      intervalTimer_initAll();
      intervalTimer_start(INTERVALTIMER_TIMER0);

      ticTacToeControl_tick();

      intervalTimer_stop(INTERVALTIMER_TIMER0);   // Note that the transition for 12:59:59 took the longest time when
      double duration;   // measured with the intervalTimer, totalling 27ms to complete this tick
      intervalTimer_getTotalDurationInSeconds(INTERVALTIMER_TIMER0, &duration);    // function due to having to redraw all 6 characters.
      if (duration_max < duration) {
        duration_max = duration;
        printf("Duration:%lf\n", duration);
      }
      intervalTimer_resetAll();

      interrupts_isrFlagGlobal = 0;
    }
  }
  interrupts_disableArmInts();
  printf("isr invocation count: %ld\n\r", interrupts_isrInvocationCount());
  printf("internal interrupt count: %ld\n\r", personalInterruptCount);

}

int main() {
  test_Full();
  return 0;
}
