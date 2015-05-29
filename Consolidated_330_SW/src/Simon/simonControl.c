//*****************************************************************************
// Luke Hsiao
// 20 May 2015
// Implementation of the SM to control the Simon game at the top level.
//*****************************************************************************

#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdio.h>
#include "globals.h"
#include "simonControl.h"
#include "simonDisplay.h"
#include "flashSequence.h"
#include "verifySequence.h"
#include "supportFiles/display.h"
#include "buttons.h"

enum simonControl_states {
  init_st,            // initial state
  touch_to_start_st,  // wait for a button press
  flash_sequence_st,  // flash the sequence
  verify_sequence_st, // check that the user did the pattern correctly
  display_wait_st,    // user correctly tapped sequence, show message
  keep_playing_st,    // check if the user wants to keep playing
  complete_st         // game is done
} simonControl_state = init_st;



void simonControl_tick() {
  // variable used when adding a random region to the sequence.
  static uint8_t randomRegion = 0;
  static uint16_t totalSequenceLength = 0;
  static uint16_t currentLength = 0;
  static uint16_t waitTimer = 0;
  static uint8_t sequence[GLOBALS_MAX_FLASH_SEQUENCE];

  /////////////////////////////////
  // Perform state action first. //
  /////////////////////////////////
  switch (simonControl_state) {
    case init_st:
      display_init(); // initialize the display
      display_fillScreen(DISPLAY_BLACK);  // Clear the display.
      waitTimer = 0;
      currentLength = 1;
      totalSequenceLength = 0;
      break;
    case touch_to_start_st:

      break;
    case flash_sequence_st:

      break;
    case verify_sequence_st:
      waitTimer = 0;
      break;
    case display_wait_st:
      waitTimer++;
      break;
    case keep_playing_st:
      currentLength = 1;
      waitTimer++;
      break;
    case complete_st:
      waitTimer++;
      break;
    default:
      printf("Default case of SimonControl State Actions hit.\n");
      break;
  }

  ////////////////////////////////
  // Perform state update next. //
  ////////////////////////////////
  switch (simonControl_state) {
    case init_st:
      simonControl_state = touch_to_start_st;
      // Print off the game instructions once during the transition.
      display_setTextSize(SIMONCONTROL_SIMON_TEXTSIZE); // large size for SIMON
      display_setCursor(GLOBALS_ONE_HALF(display_height()),   // y-position
                        GLOBALS_ONE_THIRD(display_width()));  // x-position
      display_println(" SIMON"); // Spaced over to center better.
      display_setTextSize(SIMONCONTROL_TOUCH_TEXTSIZE); // smaller size
      display_println("Touch to start");
      break;
    case touch_to_start_st:
      // Wait in this state until the display is touched
      if (display_isTouched()) {
        // Initialize random number generator based on time
        srand (time(NULL));
        // Generate a random sequence of length 4
        for (totalSequenceLength = 0; totalSequenceLength < SIMONCONTROL_INIT_SEQ_LENGTH; totalSequenceLength++) {
          // Generates a random value in range 0-3.
          sequence[totalSequenceLength] = rand() % SIMONCONTROL_NUMREGIONS;
          // Set the game sequence to the random value
          globals_setSequence(sequence, totalSequenceLength);
        }

        // Set the current sequence to show's length
        globals_getSequenceIterationLength(currentLength);

        // Enabled the flashSequence SM
        flashSequence_enable();
        // Transition to flash sequence.
        simonControl_state = flash_sequence_st;
      }
      else {
        SimonControl_state = touch_to_start_st;
      }
      break;
    case flash_sequence_st:
      // Wait in this state until the sequence is done flashing
      if (flashSequence_completed()) {
        flashSequence_disable();  // stop the flashSequence SM
        verifySequence_drawButtons(); // draw the 4 buttons
        verifySequence_enable();  // start SM to check user input
        simonControl_state = verify_sequence_st;
      }
      else {
        simonControl_state = flash_sequence_st;
      }
      break;
    case verify_sequence_st:
      // Wait here until the sequence has been verified
      if (verifySequence_isComplete()) {
        verifySequence_disable();
        display_fillScreen(DISPLAY_BLACK);  // Clear the display.

        // Check to see if the sequence was incorrect
        if (verifySequence_isTimeOutError() || verifySequence_isUserInputError()) {
          // Print off a failure message
          display_setTextSize(SIMONCONTROL_SIMON_TEXTSIZE); // large size for SIMON
          display_setCursor(GLOBALS_ONE_HALF(display_height()),   // y-position
                            GLOBALS_ONE_THIRD(display_width()));  // x-position
          display_println(" FAIL"); // Spaced over to center better.

          // Game over.
          simonControl_state = complete_st;
        }
        // If the sequence was correct
        else {
          currentLength++; // increment the current sequence length
          // If the user has completed this level, display a message
          if (currentLength >= globals_getSequenceLength()) {
            // Print off a success message
            display_setTextSize(SIMONCONTROL_SIMON_TEXTSIZE); // large size for SIMON
            display_setCursor(GLOBALS_ONE_HALF(display_height()),   // y-position
                              GLOBALS_ONE_THIRD(display_width()));  // x-position
            display_println(" YAY!"); // Spaced over to center better.

            simonControl_state = display_wait_st;
          }
          // Otherwise, increment currentLength by one and flash the sequence
          else {
            globals_setSequenceIterationLength(currentLength);
            // Enabled the flashSequence SM
            flashSequence_enable();
            simonControl_state = flash_sequence_st;
          }
        }
      }
      else {
        simonControl_state = verify_sequence_st;
      }
      break;
    case display_wait_st:
      // Display the YAY! for a moment
      if (waitTimer >= SIMONCONTROL_DISPLAY_WAIT) {
        // Print off the "touch for a new level" message
        display_setTextSize(SIMONCONTROL_TOUCH_TEXTSIZE);
        display_setCursor(GLOBALS_ONE_HALF(display_height()), 0);
        display_println("     Touch for a new level");
        simonControl_state = keep_playing_st;
      }
      else {
        simonControl_state = display_wait_st;
      }
      break;
    case keep_playing_st:
      // Wait here to display the touch for new level message.

      // If the user never touches the screen, reset.
      if (waitTimer >= SIMONCONTROL_DISPLAY_WAIT) {
        display_fillScreen(DISPLAY_BLACK);  // blank the screen
        // Print of the longest sequence accomplished
        display_setTextSize(SIMONCONTROL_TOUCH_TEXTSIZE);
        display_setCursor(GLOBALS_ONE_HALF(display_height()), 0);
        char str[SIMONCONTROL_BUFFER_SIZE];
        sprinft(str, "      Longest Sequence: %d", totalSequenceLength);
        display_println(str);
        waitTimer = 0;
        simonControl_state = complete_st;
      }
      // If the user does touch the screen, increment sequence and flash
      else if (display_isTouched()) {
        //increment our sequence
        randomRegion = rand() % SIMONCONTROL_NUMREGIONS;  // Generate region #
        totalSequenceLength++;	// increment our total sequence length

        // Generate a new sequence
        int i;
        for (i = 0; i < totalSequenceLength; i++) {
          sequence[i] = rand() % SIMONCONTROL_NUMREGIONS;
        }
        globals_setSequence(sequence, totalSequenceLength);
        globals_setSequenceIterationLength(currentLength);	//update our length

        // Flash the sequence
        flashSequence_enable(); 		//enable flashSequence state machine
        simonControl_state = flash;
      }
      else {
        simonControl_state = keep_playing_st;
      }
      break;
    case complete_st:
      // Wait here for a moment, then reset to the instructions
      if (waitTimer >= SIMONCONTROL_RESET_WAIT) {
        waitTimer = 0;
        simonControl_state = init_st;
      }
      else {
        simonControl_state = complete_st;
      }
      break;
    default:
      printf("Default case of SimonControl State Actions hit.\n");
      break;
  }
}
