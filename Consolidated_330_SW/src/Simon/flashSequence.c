//*****************************************************************************
// Luke Hsiao
// 20 May 2015
// Implementation of the SM to flash the sequence the user must mimic.
//*****************************************************************************

#include <stdio.h>
#include "flashSequence.h"
#include "globals.h"
#include "simonDisplay.h"
#include "supportFiles/display.h"
#include "supportFiles/utils.h"

static bool enabled = false; // static variable used in interlock

enum flashSequence_states {
  init_st,           // wait for enabled = true
  draw_square_st,    // draw the current square of the sequence
  wait_st,           // allows the user to actually see what's happening
  complete_st       // signals that the sequence has been shown
} flashSequence_state = init_st;

void flashSequence_enable() {
  enabled = true;
}

void flashSequence_disable() {
  enabled = false;
}

bool flashSequence_completed() {
  return (flashSequence_state == complete_st);
}

void flashSequence_tick() {
  static uint16_t index = 0;
  static uint16_t waitTimer = 0;
  /////////////////////////////////
  // Perform state action first. //
  /////////////////////////////////
  switch (flashSequence_state) {
    case init_st:
      index = 0;
      waitTimer = 0;
      break;
    case draw_square_st:
      waitTimer = 0;
      break;
    case wait_st:
      waitTimer++;
      break;
    case complete_st:

      break;
    default:
      printf("Flash Sequence tick hit default.\n");
      break;
  }

  ////////////////////////////////
  // Perform state update next. //
  ////////////////////////////////
  switch (flashSequence_state) {
    case init_st:
      if (enabled) {
        flashSequence_state = draw_square_st;
      }
      else {
        flashSequence_state = init_st;
      }
      break;
    case draw_square_st:
      uint8_t regionNumber;
      regionNumber = globals_getSequenceValue(index);
      simonDisplay_drawSquare(regionNumber, false); // draw square
      flashSequence_state = wait_st;
      break;
    case wait_st:
      // First, wait the approriate amount of time
      if (waitTimer > FLASH_WAIT) {
        // erase the square
        simonDisplay_drawSquare(globals_getSequenceValue(index), true);

        // Next, check to see if this is the last sequence to show
        uint8_t flashSequenceLength;
        flashSequenceLength = globals_getSequenceIterationLength();
        if (index >= flashSequenceLength - 1) {
          flashSequence_state = complete_st;
        }
        else {
          index++;
          flashSequence_state = draw_square_st;
        }
      }
      // Otherwise, keep waiting
      else {
        flashSequence_state = wait_st;
      }
      break;
    case complete_st:
      if (!enabled) {
        flashSequence_state = init_st;
      }
      else {
        flashSequence_state = complete_st;
      }
      break;
    default:
      printf("Flash Sequence tick hit default.\n");
      break;
  }
}

// This will set the sequence to a simple sequential pattern.
// It starts by flashing the first color, and then increments the index and flashes the first
// two colors and so forth. Along the way it prints info messages to the LCD screen.
#define TEST_SEQUENCE_LENGTH 8	// Just use a short test sequence.
uint8_t flashSequence_testSequence[TEST_SEQUENCE_LENGTH] = {SIMON_DISPLAY_REGION_0,
							    SIMON_DISPLAY_REGION_1,
							    SIMON_DISPLAY_REGION_2,
							    SIMON_DISPLAY_REGION_3,
							    SIMON_DISPLAY_REGION_3,
							    SIMON_DISPLAY_REGION_2,
							    SIMON_DISPLAY_REGION_1,
							    SIMON_DISPLAY_REGION_0};
#define INCREMENTING_SEQUENCE_MESSAGE1 "Incrementing Sequence"	// Info message.
#define RUN_TEST_COMPLETE_MESSAGE "Runtest() Complete"		// Info message.
#define MESSAGE_TEXT_SIZE 2	                                // Make the text easy to see.

// Print the incrementing sequence message.
void flashSequence_printIncrementingMessage() {
  display_fillScreen(DISPLAY_BLACK);// Otherwise, tell the user that you are incrementing the sequence.
  display_setCursor(0, display_height()/2);	    // Roughly centered.
  display_println(INCREMENTING_SEQUENCE_MESSAGE1);  // Print the message.
  utils_msDelay(2000);                              // Hold on for 2 seconds.
  display_fillScreen(DISPLAY_BLACK);		    // Clear the screen.
}

void flashSequence_runTest() {
  display_init();	// We are using the display.
  display_fillScreen(DISPLAY_BLACK);	// Clear the display.
  globals_setSequence(flashSequence_testSequence, TEST_SEQUENCE_LENGTH);	// Set the sequence.
  flashSequence_enable();			        // Enable the flashSequence state machine.
  int16_t sequenceLength = 1;	                        // Start out with a sequence of length 1.
  globals_setSequenceIterationLength(sequenceLength);	// Set the iteration length.
  display_setTextSize(MESSAGE_TEXT_SIZE);	        // Use a standard text size.
  while (1) {	                // Run forever unless you break.
    flashSequence_tick();	// tick the state machine.
    utils_msDelay(1);	// Provide a 1 ms delay.
    if (flashSequence_completed()) {  // When you are done flashing the sequence.
      flashSequence_disable();  // Interlock by first disabling the state machine.
      flashSequence_tick();	// tick is necessary to advance the state.
      utils_msDelay(1);		// don't really need this here, just for completeness.
      flashSequence_enable();	// Finish the interlock by enabling the state machine.
      utils_msDelay(1);	// Wait 1 ms for no good reason.
      sequenceLength++;	// Increment the length of the sequence.
      if (sequenceLength > TEST_SEQUENCE_LENGTH) // Stop if you have done the full sequence.
        break;
      flashSequence_printIncrementingMessage();  // Tell the user that you are going to the next step in the pattern.
      globals_setSequenceIterationLength(sequenceLength);	// Set the length of the pattern.
    }
  }
  // Let the user know that you are finished.
  display_fillScreen(DISPLAY_BLACK);
  display_setCursor(0, display_height()/2);
  display_println(RUN_TEST_COMPLETE_MESSAGE);
}
