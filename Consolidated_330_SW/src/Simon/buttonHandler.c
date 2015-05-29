//*****************************************************************************
// Luke Hsiao
// 20 May 2015
// Implementation of the SM to handle drawing buttons and detecting touches.
//*****************************************************************************

#include <stdio.h>
#include "buttonHandler.h"
#include "globals.h"
#include "simonDisplay.h"
#include "supportFiles/display.h"
#include "supportFiles/utils.h"

enum buttonHandler_st {
  init_st,             // Initial state when disabled
  wait_for_touch_st,   // wait for button to be touched state
  debounce_st,         // wait for adc to settle
  draw_square_st,      // draw the square that is being pressed
  wait_for_release_st, // wait for the user to let go
  release_detected_st  //waits here for enable == false
} buttonHandler_state = init_st;

static bool enabled = false; // static variable used in interlock

/**
 * This is a debug state print routine. It will print names of the states each
 * time tick() is called. It only prints states if they are different than the
 * previous state.
 */
void buttonHandler_debugStatePrint() {
  static buttonHandler_st previousState;
  static bool firstPass = true;
  // Only print the message if:
  // 1. This the first pass and the value for previousState is unknown.
  // 2. previousState != currentState - this prevents reprinting the same stat.
  if (previousState != buttonHandler_state || firstPass) {
    firstPass = false;     // previousState will be defined, firstPass is false.
    previousState = buttonHandler_state; // keep track of the last state you were in.
    switch(buttonHandler_state) {  // prints info based on the state that you're in.
      case init_st:
        printf("init_st\n\r");
        break;
      case wait_for_touch_st:
        printf("wait_for_touch_st\n\r");
        break;
      case debounce_st:
        printf("debounce_st\n\r");
        break;
      case draw_square_st:
        printf("draw_square_st\n\r");
        break;
      case wait_for_release_st:
        printf("wait_for_release_st\n\r");
        break;
      case release_detected_st:
        printf("release_detected_st\n\r");
        break;
      default:
        printf("Shouldn't have hit this default case\n");
        break;
     }
  }
}

uint8_t buttonHandler_getRegionNumber() {
  // old touch data should have been cleared previously
  int16_t x, y;   // x and y coordinate of the touched point
  uint8_t pressure; // relative touch pressure, variable will be ignored
  // Get the data of the touched point
  display_getTouchedPoint(&x, &y, &pressure);
  // Calculate the region number from the x and y coordinate
  return simonDisplay_computeRegionNumber(x,y);
}

void buttonHandler_enable() {
  enabled = true; // enable the state machine
}

void buttonHandler_disable() {
  enabled = false; // disable the state machine
}

bool buttonHandler_releaseDetected() {
  return (buttonHandler_state == release_detected_st);
}

void buttonHandler_tick() {
  //buttonHandler_debugStatePrint();

  static uint16_t adcTimer = 0;   // counter used to debounce
  static uint8_t currentRegion;
  /////////////////////////////////
  // Perform state action first. //
  /////////////////////////////////
  switch (buttonHandler_state) {
    case init_st:
      adcTimer = 0;
      break;
    case wait_for_touch_st:
      adcTimer = 0;
      break;
    case debounce_st:
      adcTimer++;
      break;
    case draw_square_st:
      break;
    case wait_for_release_st:

      break;
    case release_detected_st:

      break;
    default:
      printf("Button Handler Tick reached default case\n");
      break;
  }

  ////////////////////////////////
  // Perform state update next. //
  ////////////////////////////////
  switch (buttonHandler_state) {
    case init_st:
      if (enabled) {
        buttonHandler_state = wait_for_touch_st;
      }
      else {
        buttonHandler_state = init_st;
      }
      break;
    case wait_for_touch_st:
      if (display_isTouched()) {
        display_clearOldTouchData();
        buttonHandler_state = debounce_st;
      }
      else {
        buttonHandler_state = wait_for_touch_st;
      }
      break;
    case debounce_st:
      // Wait for the time for the ADC to settle
      if (adcTimer >= ADC_WAIT) {
        if (display_isTouched()) {  // if display is still touched, draw
          buttonHandler_state = draw_square_st;
        }
      }
      else if (!display_isTouched()) {
        buttonHandler_state = wait_for_touch_st;
      }
      // Otherwise, wait here
      else {
        buttonHandler_state = debounce_st;
      }
      break;
    case draw_square_st:
      currentRegion = buttonHandler_getRegionNumber();
      simonDisplay_drawSquare(currentRegion, false);  // only draw square once
      buttonHandler_state = wait_for_release_st;
      break;
    case wait_for_release_st:
      // Once use lets go, erase the big square and redraw the original button
      if (!display_isTouched()) {
        simonDisplay_drawSquare(currentRegion, true);
        simonDisplay_drawButton(currentRegion);
        buttonHandler_state = release_detected_st;
      }
      else {
        buttonHandler_state = wait_for_release_st;
      }
      break;
    case release_detected_st:
      if (!enabled) {
        buttonHandler_state = init_st;
      }
      else {
        buttonHandler_state = release_detected_st;
      }
      break;
    default:
      printf("Button Handler Tick reached default case\n");
      break;
  }
}

#define RUN_TEST_TERMINATION_MESSAGE1 "buttonHandler_runTest()"
#define RUN_TEST_TERMINATION_MESSAGE2 "terminated."
#define RUN_TEST_TEXT_SIZE 2
// buttonHandler_runTest(int16_t touchCount) runs the test until
// the user has touched the screen touchCount times. It indicates
// that a button was pushed by drawing a large square while
// the button is pressed and then erasing the large square and
// redrawing the button when the user releases their touch.

void buttonHandler_runTest(int16_t touchCountArg) {
  int16_t touchCount = 0;             // Keep track of the number of touches.
  display_init();                     // Always have to init the display.
  display_fillScreen(DISPLAY_BLACK);  // Clear the display.
  simonDisplay_drawAllButtons();      // Draw the four buttons.
  buttonHandler_enable();
  while (touchCount < touchCountArg) {  // Loop here while touchCount is less than the touchCountArg
    buttonHandler_tick();               // Advance the state machine.
    utils_msDelay(1);			// Wait here for 1 ms.
    if (buttonHandler_releaseDetected()) {  // If a release is detected, then the screen was touched.
      touchCount++;                         // Keep track of the number of touches.
      printf("button released: %d\n\r", buttonHandler_getRegionNumber());  // Get the region number that was touched.
      buttonHandler_disable();             // Interlocked behavior: handshake with the button handler (now disabled).
      utils_msDelay(1);                     // wait 1 ms.
      buttonHandler_tick();                 // Advance the state machine.
      buttonHandler_enable();               // Interlocked behavior: enable the buttonHandler.
      utils_msDelay(1);                     // wait 1 ms.
      buttonHandler_tick();                 // Advance the state machine.
    }
  }
  display_fillScreen(DISPLAY_BLACK);			// clear the screen.
  display_setTextSize(RUN_TEST_TEXT_SIZE);		// Set the text size.
  display_setCursor(0, display_height()/2);		// Move the cursor to a rough center point.
  display_println(RUN_TEST_TERMINATION_MESSAGE1);	// Print the termination message on two lines.
  display_println(RUN_TEST_TERMINATION_MESSAGE2);
}
