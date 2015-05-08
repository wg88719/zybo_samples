//*****************************************************************************
// Luke Hsiao
// 4 May 2015
// Driver file for controlling with the clock
//*****************************************************************************

#include <stdio.h>
#include "clockControl.h"
#include "clockDisplay.h"
#include "supportFiles/display.h"
#include "intervalTimer.h"

// States for the controller state machine.
enum clockControl_st {
  init_st,                // Start here, stay in this state for just one tick.
  never_touched_st,       // Wait for first touch - clock is disabled until set.
  waiting_for_touch_st,    // waiting for touch, clock is enabled and running.
  ad_timer_running_st,    // waiting for the touch-controller ADC to settle.
  auto_timer_running_st,  // waiting for the auto-update delay to expire
                              // (user is holding down button for auto-inc/dec)
  rate_timer_running_st,  // waiting for the rate-timer to expire to know when
                              // to perform the auto inc/dec.
  rate_timer_expired_st,  // when rate-timer expires, perform inc/dec function.
  add_second_to_clock_st  // add a second to the time and reset the ms counter.
} currentState = init_st; // Initialize to init_st

// Global variables representing the timers that will be used
uint32_t adTimer = 0;   // Time to wait for touch-controller ADC to settle.
uint32_t autoTimer = 0; // Time before auto-updating when user holds button
uint32_t rateTimer = 0; // Time between auto inc/dec calls
uint32_t msCounter = 0; // Time between normal second updates

double duration_max = 0.0;

/**
 * This is a debug state print routine. It will print names of the states each
 * time tick() is called. It only prints states if they are different than the
 * previous state.
 */
void clockControl_debugStatePrint() {
  static clockControl_st previousState;
  static bool firstPass = true;
  // Only print the message if:
  // 1. This the first pass and the value for previousState is unknown.
  // 2. previousState != currentState - this prevents reprinting the same stat.
  if (previousState != currentState || firstPass) {
    firstPass = false;     // previousState will be defined, firstPass is false.
    previousState = currentState; // keep track of the last state you were in.
    printf("msCounter:%d\n\r", (int)msCounter);
    switch(currentState) {  // prints info based on the state that you're in.
      case init_st:
        printf("init_st\n\r");
        break;
      case never_touched_st:
        printf("never_touched_st\n\r");
        break;
      case waiting_for_touch_st:
        printf("waiting_for_touch_st\n\r");
        break;
      case ad_timer_running_st:
        printf("ad_timer_running_st\n\r");
        break;
      case auto_timer_running_st:
        printf("auto_timer_running_st\n\r");
        break;
      case rate_timer_running_st:
        printf("rate_timer_running_st\n\r");
        break;
      case rate_timer_expired_st:
        printf("rate_timer_expired_st\n\r");
        break;
      case add_second_to_clock_st:
        printf("add_second_to_clock_st\n\r");
        break;
      default:
        printf("Shouldn't have hit this default case\n");
        break;
     }
  }
}

void clockControl_tick() {
  intervalTimer_start(TIMER0);
  // Print out state changes for reference
  //clockControl_debugStatePrint();

  /////////////////////////////////
  // Perform state action first. //
  /////////////////////////////////
  switch(currentState) {
    case init_st:
      break;  // Do nothing in init_st
    case never_touched_st:
      break;  // Do nothing in never_touched_st
    case waiting_for_touch_st:  // reset all of the timers
      adTimer = 1;
      autoTimer = 1;
      rateTimer = 1;
      msCounter++;  // increment msCounter while waiting
      break;
    case ad_timer_running_st: // increment adTimer
      adTimer++;
      msCounter = 1;  // reset the second advancement counter
      break;
    case auto_timer_running_st: //increment autoTimer
      autoTimer++;
      msCounter = 1;  // reset the second advancement counter
      break;
    case rate_timer_running_st: // increment rateTimer
      rateTimer++;
      msCounter = 1;  // reset the second advancement counter
      break;
    case rate_timer_expired_st: // set rateTimer to 0
      rateTimer = 1;
      msCounter = 1;  // reset the second advancement counter
      break;
    case add_second_to_clock_st:
      msCounter = 1;  // reset the msCounter
      // Add one second
      clockDisplay_advanceTimeOneSecond();
      break;
     default:
      printf("clockControl_tick state action: hit default\n\r");
      break;
  }

  ////////////////////////////////
  // Perform state update next. //
  ////////////////////////////////
  switch(currentState) {
    case init_st:
      currentState = never_touched_st;  // moved to never_touched state
      break;
    case never_touched_st:
      // If the display gets touched, leave this state
      if (display_isTouched()) {
        currentState = waiting_for_touch_st; // move to waiting_for_touch_st
        display_clearOldTouchData();  // clear old touch data for fresh start
      }
      else {  // otherwise, keep waiting here. Clock will NOT change.
        currentState = never_touched_st;
      }
      break;
    case waiting_for_touch_st:
      // If the display is touched, move on to next state
      if (display_isTouched()) {
        currentState = ad_timer_running_st;
        display_clearOldTouchData();  // clear old touch data for fresh start
      }
      // Otherwise, if a second of time has passed, increment a second
      else if (msCounter >= SECOND_WAIT){
        currentState = add_second_to_clock_st;
        display_clearOldTouchData();  // clear old touch data for fresh start
      }
      // Otherwise, keep waiting here
      else {
        currentState = waiting_for_touch_st;
      }
      break;
    case ad_timer_running_st:
      // if the display is only tapped, inc/dec once
      if (!display_isTouched() && adTimer >= ADC_WAIT) {
        currentState = waiting_for_touch_st;
        clockDisplay_performIncDec(); // just inc/dec once based on touch
      }
      // if they are still holding it after the ADC settles, move to auto_timer
      else if (display_isTouched() && adTimer >= ADC_WAIT) {
        currentState = auto_timer_running_st;
      }
      // Otherwise, stay in this state
      else {
        currentState = ad_timer_running_st;
      }
      break;
    case auto_timer_running_st:
      // If display is let go, perform a single inc/dec
      if (!display_isTouched()) {
        currentState = waiting_for_touch_st;
        clockDisplay_performIncDec();
      }
      // If they continue to hold the arrow for 0.5 seconds, move on to
      // auto increment/decrement
      else if (display_isTouched() && autoTimer >= HALF_SECOND_WAIT) {
        currentState = rate_timer_running_st;
        clockDisplay_performIncDec();
      }
      // Otherwise, remain in this state
      else {
        currentState = auto_timer_running_st;
      }
      break;
    case rate_timer_running_st:
      // If they let go of the display, go back to waiting
      if (!display_isTouched()) {
        currentState = waiting_for_touch_st;
      }
      // If they keep holding the display, increment at 10 inc/dec per sec
      else if (display_isTouched() && rateTimer >= TENTH_SECOND_WAIT) {
        currentState = rate_timer_expired_st;
      }
      // Otherwise, wait here
      else {
        currentState = rate_timer_running_st;
      }
      break;
    case rate_timer_expired_st:
      // if the display is let go, go back to waiting
      if (!display_isTouched()) {
        currentState = waiting_for_touch_st;
      }
      // Otherwise, keep incrementing
      else {
        clockDisplay_performIncDec();
        currentState = rate_timer_running_st;
      }
      break;
    case add_second_to_clock_st:
      // Go back to waiting
      currentState = waiting_for_touch_st;
      break;
    default:  // Shouldn't ever hit this state
      printf("clockControl_tick state update: hit default\n\r");
      break;
  }

  intervalTimer_stop(TIMER0);
  double duration;
  intervalTimer_getTotalDurationInSeconds(TIMER0, &duration);
  if (duration_max < duration) {
    duration_max = duration;
    printf("Duration:%lf\n", duration);
  }
  intervalTimer_resetAll();

}
