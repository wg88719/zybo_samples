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
  victory_wait_st,    // user correctly tapped sequence, give them a break
  keep_playing_st,    // check if the user wants to keep playing
  increment_st,       // add a button to sequence
  complete_st         // game is done
} simonControl_state = init_st;
