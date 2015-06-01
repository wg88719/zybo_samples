//*****************************************************************************
// Luke Hsiao
// 20 May 2015
// Implementation of the SM to verify that the sequence of user touches
// matches the computer-generated sequence.
//*****************************************************************************

#include <stdio.h>
#include "verifySequence.h"
#include "buttons.h"
#include "buttonHandler.h"
#include "globals.h"
#include "simonDisplay.h"
#include "supportFiles/display.h"
#include "supportFiles/utils.h"

enum verifySequence_states {
  init_st,              // wait for verifySequence_enable()
  wait_for_timeout_st,  // wait here until user touches, or there is a timeout
  verification_st,      // check if the user touched the correct region
  wait_for_release_st,  // allow the user to stop pressing a button before timing the next one
  complete_st           // wait for enable==false
} verifySequence_state = init_st;

static bool enabled = false;
static bool isTimeOutError = false;
static bool isUserInputError = false;

void verifySequence_enable() {
  enabled = true;
}

void verifySequence_disable() {
  enabled = false;
}

bool verifySequence_isTimeOutError() {
  return isTimeOutError;
}

bool verifySequence_isUserInputError() {
  return isUserInputError;
}

bool verifySequence_isComplete() {
  return (verifySequence_state == complete_st);
}

void verifySequence_tick() {
  static uint16_t index = 0;
  static uint16_t timeOutTimer = 0; // counter to track whether a timeout occurs

  /////////////////////////////////
  // Perform state action first. //
  /////////////////////////////////
  switch (verifySequence_state) {
    case init_st:
      isTimeOutError = false;
      isUserInputError = false;
      index = 0;
      timeOutTimer = 0;
      break;
    case wait_for_timeout_st:
      timeOutTimer++;
      buttonHandler_enable();
      break;
    case wait_for_release_st:
      break;
    case verification_st:
      break;

    case complete_st:

      break;
    default:
      printf("Default case hit in verifySequence tick.\n");
      break;
  }

  ////////////////////////////////
  // Perform state update next. //
  ////////////////////////////////
  switch (verifySequence_state) {
    case init_st:
      if (enabled) {
        buttonHandler_enable();
        verifySequence_state = wait_for_timeout_st;
      }
      else {
        verifySequence_state = init_st;
      }
      break;
    case wait_for_timeout_st:
      if(display_isTouched()) {
        timeOutTimer = 0;
        verifySequence_state = wait_for_release_st;
      }
      // If the use waits too long before doing anything
      else if (timeOutTimer >= WAIT_TIMEOUT)
      {
        //printf("userTouchTimer: %d", userTouchTimer);
        isTimeOutError = true;
        timeOutTimer = 0;
        verifySequence_state = complete_st;
      }
      else {
        verifySequence_state = wait_for_timeout_st;
      }
      break;
    case wait_for_release_st:
      if (buttonHandler_releaseDetected())
      {
        buttonHandler_disable();
        verifySequence_state = verification_st;
      }
      else {
        verifySequence_state = wait_for_release_st;
      }
      break;
    case verification_st:
      uint8_t tempSequenceValue, tempRegionNumber;
      tempSequenceValue = globals_getSequenceValue(index);
      tempRegionNumber = buttonHandler_getRegionNumber();

      if(tempSequenceValue == tempRegionNumber) { //user touched the correct square
        // If that was the last square
        if (globals_getSequenceIterationLength() == (index + 1)) { //because index starts at 0 and length at 1
          verifySequence_state = complete_st;
        }
        else {
          index++;
          verifySequence_state = wait_for_timeout_st;
        }
      }
      else {
        isUserInputError = true;
        //printf("USER INPUT ERROR");
        verifySequence_state = complete_st;
      }
      break;
    case complete_st:
      if (!enabled) {
        verifySequence_state = init_st;
      }
      else {
        verifySequence_state = complete_st;
      }
      break;
    default:
      printf("Default case hit in verifySequence tick.\n");
      break;
  }
}

#define MESSAGE_X 0
#define MESSAGE_Y (display_width()/4)
#define MESSAGE_TEXT_SIZE 2
#define MESSAGE_STARTING_OVER
// Prints the instructions that the user should follow when
// testing the verifySequence state machine.
// Takes an argument that specifies the length of the sequence so that
// the instructions are tailored for the length of the sequence.
// This assumes a simple incrementing pattern so that it is simple to
// instruct the user.
void verifySequence_printInstructions(uint8_t length, bool startingOver) {
  display_fillScreen(DISPLAY_BLACK);	    // Clear the screen.
  display_setTextSize(MESSAGE_TEXT_SIZE);   // Make it readable.
  display_setCursor(MESSAGE_X, MESSAGE_Y);  // Rough center.
  if (startingOver) {	                    // Print a message if you start over.
    display_fillScreen(DISPLAY_BLACK);	    // Clear the screen if starting over.
    display_setTextColor(DISPLAY_WHITE);    // Print whit text.
    display_println("Starting Over. ");
  }
  display_println("Tap: ");
  display_println();
  switch (length) {
  case 1:
    display_println("red");
    break;
  case 2:
    display_println("red, yellow ");
    break;
  case 3:
    display_println("red, yellow, blue ");
    break;
  case 4:
    display_println("red, yellow, blue, green ");
    break;
  default:
    break;
  }
  display_println("in that order.");
  display_println();
  display_println("hold BTN0 to quit.");
}

// Just clears the screen and draws the four buttons used in Simon.
void verifySequence_drawButtons() {
	display_fillScreen(DISPLAY_BLACK);
	simonDisplay_drawAllButtons();  // Draw the four buttons.
}

// This will set the sequence to a simple sequential pattern.
#define MAX_TEST_SEQUENCE_LENGTH 4  // the maximum length of the pattern
uint8_t verifySequence_testSequence[MAX_TEST_SEQUENCE_LENGTH] = {0, 1, 2, 3};  // A simple pattern.
#define MESSAGE_WAIT_MS 4000  // Display messages for this long.

// Increment the sequence length making sure to skip over 0.
// Used to change the sequence length during the test.
int16_t incrementSequenceLength(int16_t sequenceLength) {
  int16_t value = (sequenceLength + 1) % (MAX_TEST_SEQUENCE_LENGTH+1);
  if (value == 0) value++;
    return value;
}

// Used to select from a variety of informational messages.
enum verifySequence_infoMessage_t {
  user_time_out_e,            // means that the user waited too long to tap a color.
  user_wrong_sequence_e,      // means that the user tapped the wrong color.
  user_correct_sequence_e,    // means that the user tapped the correct sequence.
  user_quit_e                 // means that the user wants to quite.
};

// Prints out informational messages based upon a message type (see above).
void verifySequence_printInfoMessage(verifySequence_infoMessage_t messageType) {
  // Setup text color, position and clear the screen.
  display_setTextColor(DISPLAY_WHITE);
  display_setCursor(MESSAGE_X, MESSAGE_Y);
  display_fillScreen(DISPLAY_BLACK);
  switch(messageType) {
  case user_time_out_e:  // Tell the user that they typed too slowly.
    display_println("Error:");
    display_println();
    display_println("  User tapped sequence");
    display_println("  too slowly.");
    break;
  case user_wrong_sequence_e:  // Tell the user that they tapped the wrong color.
    display_println("Error: ");
    display_println();
    display_println("  User tapped the");
    display_println("  wrong sequence.");
    break;
  case user_correct_sequence_e:  // Tell the user that they were correct.
    display_println("User tapped");
    display_println("the correct sequence.");
    break;
  case user_quit_e:             // Acknowledge that you are quitting the test.
    display_println("quitting runTest().");
    break;
  default:
    break;
  }
}

#define BTN0 1
// Tests the verifySequence state machine.
// It prints instructions to the touch-screen. The user responds by tapping the
// correct colors to match the sequence.
// Users can test the error conditions by waiting too long to tap a color or
// by tapping an incorrect color.
void verifySequence_runTest() {
  display_init();  // Always must do this.
  buttons_init();  // Need to use the push-button package so user can quit.
  int16_t sequenceLength = 1;  // Start out with a sequence length of 1.
  verifySequence_printInstructions(sequenceLength, false);  // Tell the user what to do.
  utils_msDelay(MESSAGE_WAIT_MS);  // Give them a few seconds to read the instructions.
  verifySequence_drawButtons();    // Now, draw the buttons.
  // Set the test sequence and it's length.
  globals_setSequence(verifySequence_testSequence, MAX_TEST_SEQUENCE_LENGTH);
  globals_setSequenceIterationLength(sequenceLength);
  // Enable the verifySequence state machine.
  verifySequence_enable();           // Everything is interlocked, so first enable the machine.
  while (!(buttons_read() & BTN0)) { // Need to hold button until it quits as you might be stuck in a delay.
    // verifySequence uses the buttonHandler state machine so you need to "tick" both of them.
    verifySequence_tick();  // Advance the verifySequence state machine.
    buttonHandler_tick();   // Advance the buttonHandler state machine.
    utils_msDelay(1);       // Wait 1 ms.
    // If the verifySequence state machine has finished, check the result, otherwise just keep ticking both machines.
    if (verifySequence_isComplete()) {
      if (verifySequence_isTimeOutError()) {                      // Was the user too slow?
        verifySequence_printInfoMessage(user_time_out_e);         // Yes, tell the user that they were too slow.
      } else if (verifySequence_isUserInputError()) {             // Did the user tap the wrong color?
        verifySequence_printInfoMessage(user_wrong_sequence_e);   // Yes, tell them so.
      } else {
        verifySequence_printInfoMessage(user_correct_sequence_e); // User was correct if you get here.
      }
      utils_msDelay(MESSAGE_WAIT_MS);                            // Allow the user to read the message.
      sequenceLength = incrementSequenceLength(sequenceLength);  // Increment the sequence.
      globals_setSequenceIterationLength(sequenceLength);  // Set the length for the verifySequence state machine.
      verifySequence_printInstructions(sequenceLength, true);    // Print the instructions.
      utils_msDelay(MESSAGE_WAIT_MS);                            // Let the user read the instructions.
      verifySequence_drawButtons();                              // Draw the buttons.
      verifySequence_disable();                                  // Interlock: first step of handshake.
      verifySequence_tick();                                     // Advance the verifySequence machine.
      utils_msDelay(1);                                          // Wait for 1 ms.
      verifySequence_enable();                                   // Interlock: second step of handshake.
      utils_msDelay(1);                                          // Wait 1 ms.
    }
  }
  verifySequence_printInfoMessage(user_quit_e);  // Quitting, print out an informational message.
}
