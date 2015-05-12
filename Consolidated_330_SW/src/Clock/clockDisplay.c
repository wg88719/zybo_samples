//*****************************************************************************
// Luke Hsiao
// 4 May 2015
// Driver file for displaying the clock
//*****************************************************************************

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "clockDisplay.h"
#include "xparameters.h"
#include "supportFiles/display.h"
#include "supportFiles/utils.h"
#include "intervalTimer.h"

// Global variables for tracking time
// Used signed ints to utilize negative values as a signal to rollover
static int8_t hours = 0;  // track hours
static int8_t minutes = 0;  // track minutes
static int8_t seconds = 0;  // track seconds

static char current_time[NUM_CHARS]; // char array representing the current time
static char old_time[NUM_CHARS]; // char array representing the previous time


//*****************************************************************************
// Helper Functions for displaying the clock

/**
 * The touchscreen is divided into 6 rectangular regions numbered 0-5 as shown:
 * +--+---+---+---+---+---+---+---+---+--+
 * |  |   |   |   |   |   |   |   |   |  |
 * +--+===+===+===+===+===+===+===+===+--+
 * |  |   0   |   |   1   |   |   2   |  |
 * +--+---+---+---+---+---+---+---+---+--+
 * |  | H | H | : | M | M | : | S | S |  |
 * +--+---+---+---+---+---+---+---+---+--+
 * |  |   3   |   |   4   |   |   5   |  |
 * +--+===+===+===+===+===+===+===+===+--+
 * |  |   |   |   |   |   |   |   |   |  |
 * +--+---+---+---+---+---+---+---+---+--+
 * where each region contains a rectangle. This function returns which region
 * was pressed by the user.
 * @param  x The x-coordinate of the point pressed by the user
 * @param  y The y-coordinate of the point pressed by the user
 * @return   The number representing the region pressed or REGION_ERR for other
 */
int8_t clockDisplay_getInputRegion(int16_t x, int16_t y) {
  // Error condition for values off the screen
  if (x < 0 || y < 0 || x > display_width() || y > display_height()) {
    return CLOCKDISPLAY_REGION_ERR;
  }

  // Parse through the UP arrows
  if (y < CLOCKDISPLAY_ROW_1 && y > CLOCKDISPLAY_ROW_0) {
    // Check Region 0
    if (x < CLOCKDISPLAY_COLUMN_2 && x > CLOCKDISPLAY_COLUMN_0) {
      return CLOCKDISPLAY_REGION_0;
    }
    // Check Region 1
    if (x < CLOCKDISPLAY_COLUMN_5 && x > CLOCKDISPLAY_COLUMN_3) {
      return CLOCKDISPLAY_REGION_1;
    }
    // Check Region 2
    if (x < CLOCKDISPLAY_COLUMN_8 && x > CLOCKDISPLAY_COLUMN_6) {
      return CLOCKDISPLAY_REGION_2;
    }
  }

  // Parse through the DOWN arrows
  if (y < CLOCKDISPLAY_ROW_3 && y > CLOCKDISPLAY_ROW_2) {
    // Check Region 3
    if (x < CLOCKDISPLAY_COLUMN_2 && x > CLOCKDISPLAY_COLUMN_0) {
      return CLOCKDISPLAY_REGION_3;
    }
    // Check Region 4
    if (x < CLOCKDISPLAY_COLUMN_5 && x > CLOCKDISPLAY_COLUMN_3) {
      return CLOCKDISPLAY_REGION_4;
    }
    // Check Region 5
    if (x < CLOCKDISPLAY_COLUMN_8 && x > CLOCKDISPLAY_COLUMN_6) {
      return CLOCKDISPLAY_REGION_5;
    }
  }
  // If not in any of these regions, return REGION_ERR
  return CLOCKDISPLAY_REGION_ERR;
}

/**
 * Draws the specified character at the specified location on the screen
 * @param index The column index of the clock to draw the character at
 * @param c     The character to draw
 */
void clockDisplay_redrawDigit(uint8_t index, char c) {

  // Draw the character based on the index
  switch (index) {
    case CLOCKDISPLAY_TENS_HRS:   // Tens-digit for Hours
      display_drawChar( CLOCKDISPLAY_COLUMN_0,                      // x
                        CLOCKDISPLAY_ROW_1 + CLOCKDISPLAY_BUFFER,   // y
                        c,                          // char to draw
                        DISPLAY_GREEN,              // color of text
                        DISPLAY_BLACK,              // color of background
                        CLOCKDISPLAY_CLOCK_TEXT_SIZE);     // size of text
      break;
    case CLOCKDISPLAY_ONES_HRS:   // Ones-digit for Hours
      display_drawChar( CLOCKDISPLAY_COLUMN_1,                     // x
                        CLOCKDISPLAY_ROW_1 + CLOCKDISPLAY_BUFFER,  // y
                        c,                          // char to draw
                        DISPLAY_GREEN,              // color of text
                        DISPLAY_BLACK,              // color of background
                        CLOCKDISPLAY_CLOCK_TEXT_SIZE);     // size of text
      break;
    case CLOCKDISPLAY_COLON_1:   // First ':', do nothing
      break;
    case CLOCKDISPLAY_TENS_MINS:   // Tens-digit for Mins
      display_drawChar( CLOCKDISPLAY_COLUMN_3,                      // x
                        CLOCKDISPLAY_ROW_1 + CLOCKDISPLAY_BUFFER,   // y
                        c,                          // char to draw
                        DISPLAY_GREEN,              // color of text
                        DISPLAY_BLACK,              // color of background
                        CLOCKDISPLAY_CLOCK_TEXT_SIZE);     // size of text
      break;
    case CLOCKDISPLAY_ONES_MINS:   // Ones-digit for Mins
      display_drawChar( CLOCKDISPLAY_COLUMN_4,                      // x
                        CLOCKDISPLAY_ROW_1 + CLOCKDISPLAY_BUFFER,   // y
                        c,                          // char to draw
                        DISPLAY_GREEN,              // color of text
                        DISPLAY_BLACK,              // color of background
                        CLOCKDISPLAY_CLOCK_TEXT_SIZE);     // size of text
      break;
    case CLOCKDISPLAY_COLON_2:   // 2nd ':', do nothing
      break;
    case CLOCKDISPLAY_TENS_SECS:   // Tens-digit for Secs
      display_drawChar( CLOCKDISPLAY_COLUMN_6,                      // x
                        CLOCKDISPLAY_ROW_1 + CLOCKDISPLAY_BUFFER,   // y
                        c,                          // char to draw
                        DISPLAY_GREEN,              // color of text
                        DISPLAY_BLACK,              // color of background
                        CLOCKDISPLAY_CLOCK_TEXT_SIZE);     // size of text
      break;
    case CLOCKDISPLAY_ONES_SECS:   // Ones-digit for Secs
      display_drawChar( CLOCKDISPLAY_COLUMN_7,                      // x
                        CLOCKDISPLAY_ROW_1 + CLOCKDISPLAY_BUFFER,   // y
                        c,                          // char to draw
                        DISPLAY_GREEN,              // color of text
                        DISPLAY_BLACK,              // color of background
                        CLOCKDISPLAY_CLOCK_TEXT_SIZE);     // size of text
      break;
    default:  // Otherwise, do nothing
      break;
  }


}

/**
 * Helper function that draws the lines representing the borders of each of
 * the sub boxes. Used for visually verifying the macro calculations.
 */
void clockDisplay_drawLines() {
  // Draw Vertical Lines of each column in the drawing grid
  display_drawLine( CLOCKDISPLAY_COLUMN_0,  // x0
                    CLOCKDISPLAY_ROW_0,     // y0
                    CLOCKDISPLAY_COLUMN_0,  // x1
                    CLOCKDISPLAY_ROW_3,     // y1
                    DISPLAY_RED);
  display_drawLine( CLOCKDISPLAY_COLUMN_1,  // x0
                    CLOCKDISPLAY_ROW_0,     // y0
                    CLOCKDISPLAY_COLUMN_1,  // x1
                    CLOCKDISPLAY_ROW_3,     // y1
                    DISPLAY_RED);
  display_drawLine( CLOCKDISPLAY_COLUMN_2,  // x0
                    CLOCKDISPLAY_ROW_0,     // y0
                    CLOCKDISPLAY_COLUMN_2,  // x1
                    CLOCKDISPLAY_ROW_3,     // y1
                    DISPLAY_RED);
  display_drawLine( CLOCKDISPLAY_COLUMN_3,  // x0
                    CLOCKDISPLAY_ROW_0,     // y0
                    CLOCKDISPLAY_COLUMN_3,  // x1
                    CLOCKDISPLAY_ROW_3,     // y1
                    DISPLAY_RED);
  display_drawLine( CLOCKDISPLAY_COLUMN_4,  // x0
                    CLOCKDISPLAY_ROW_0,     // y0
                    CLOCKDISPLAY_COLUMN_4,  // x1
                    CLOCKDISPLAY_ROW_3,     // y1
                    DISPLAY_RED);
  display_drawLine( CLOCKDISPLAY_COLUMN_5,  // x0
                    CLOCKDISPLAY_ROW_0,     // y0
                    CLOCKDISPLAY_COLUMN_5,  // x1
                    CLOCKDISPLAY_ROW_3,     // y1
                    DISPLAY_RED);
  display_drawLine( CLOCKDISPLAY_COLUMN_6,  // x0
                    CLOCKDISPLAY_ROW_0,     // y0
                    CLOCKDISPLAY_COLUMN_6,  // x1
                    CLOCKDISPLAY_ROW_3,     // y1
                    DISPLAY_RED);
  display_drawLine( CLOCKDISPLAY_COLUMN_7,  // x0
                    CLOCKDISPLAY_ROW_0,     // y0
                    CLOCKDISPLAY_COLUMN_7,  // x1
                    CLOCKDISPLAY_ROW_3,     // y1
                    DISPLAY_RED);
  display_drawLine( CLOCKDISPLAY_COLUMN_8,  // x0
                    CLOCKDISPLAY_ROW_0,     // y0
                    CLOCKDISPLAY_COLUMN_8,  // x1
                    CLOCKDISPLAY_ROW_3,     // y1
                    DISPLAY_RED);

  // Draw Horizontal Lines of each row in the drawing grid
  display_drawLine( CLOCKDISPLAY_COLUMN_0,  // x0
                    CLOCKDISPLAY_ROW_0,     // y0
                    CLOCKDISPLAY_COLUMN_8,  // x1
                    CLOCKDISPLAY_ROW_0,     // y1
                    DISPLAY_RED);
  display_drawLine( CLOCKDISPLAY_COLUMN_0,  // x0
                    CLOCKDISPLAY_ROW_1,     // y0
                    CLOCKDISPLAY_COLUMN_8,  // x1
                    CLOCKDISPLAY_ROW_1,     // y1
                    DISPLAY_RED);
  display_drawLine( CLOCKDISPLAY_COLUMN_0,  // x0
                    CLOCKDISPLAY_ROW_2,     // y0
                    CLOCKDISPLAY_COLUMN_8,  // x1
                    CLOCKDISPLAY_ROW_2,     // y1
                    DISPLAY_RED);
  display_drawLine( CLOCKDISPLAY_COLUMN_0,  // x0
                    CLOCKDISPLAY_ROW_3,     // y0
                    CLOCKDISPLAY_COLUMN_8,  // x1
                    CLOCKDISPLAY_ROW_3,     // y1
                    DISPLAY_RED);
}

//********************** End Helper Functions *********************************


void clockDisplay_init() {
  display_init(); // Initialize the LCD display
  display_fillScreen(DISPLAY_BLACK);  // Blank the screen

  // Initialize the global variables
  seconds = CLOCKDISPLAY_INITIAL_SECONDS;
  minutes = CLOCKDISPLAY_INITIAL_MINUTES;
  hours = CLOCKDISPLAY_INITIAL_HOURS;

  // Initialize the current_time to the initial value
  sprintf(current_time, "%2d:%02d:%02d", hours, minutes, seconds);

  //////////////////////////////////////////////////////////////
  // Draw all components of the display that do not change:
  //////////////////////////////////////////////////////////////

  // Draw all of the user-input triangles:

  // Draw Hour Up Triangle
  display_fillTriangle( CLOCKDISPLAY_COLUMN_0,                            // x0
                        CLOCKDISPLAY_ROW_1,                               // y0
                        CLOCKDISPLAY_COLUMN_2,                            // x1
                        CLOCKDISPLAY_ROW_1,                               // y1
                        CLOCKDISPLAY_COLUMN_1,                            // x2
                        CLOCKDISPLAY_ROW_0 + CLOCKDISPLAY_ARROW_HEIGHT,   // y2
                        DISPLAY_GREEN);         // color

  // Draw Minute Up Triangle
display_fillTriangle( CLOCKDISPLAY_COLUMN_3,                              // x0
                        CLOCKDISPLAY_ROW_1,                               // y0
                        CLOCKDISPLAY_COLUMN_5,                            // x1
                        CLOCKDISPLAY_ROW_1,                               // y1
                        CLOCKDISPLAY_COLUMN_4,                            // x2
                        CLOCKDISPLAY_ROW_0 + CLOCKDISPLAY_ARROW_HEIGHT,   // y2
                        DISPLAY_GREEN);         // color

  // Draw Second Up Triangle
  display_fillTriangle( CLOCKDISPLAY_COLUMN_6,                            // x0
                        CLOCKDISPLAY_ROW_1,                               // y0
                        CLOCKDISPLAY_COLUMN_8,                            // x1
                        CLOCKDISPLAY_ROW_1,                               // y1
                        CLOCKDISPLAY_COLUMN_7,                            // x2
                        CLOCKDISPLAY_ROW_0 + CLOCKDISPLAY_ARROW_HEIGHT,   // y2
                        DISPLAY_GREEN);         // color

  // Draw Hours Down Triangle
  display_fillTriangle( CLOCKDISPLAY_COLUMN_0,                            // x0
                        CLOCKDISPLAY_ROW_2,                               // y0
                        CLOCKDISPLAY_COLUMN_2,                            // x1
                        CLOCKDISPLAY_ROW_2,                               // y1
                        CLOCKDISPLAY_COLUMN_1,                            // x2
                        CLOCKDISPLAY_ROW_3 - CLOCKDISPLAY_ARROW_HEIGHT,   // y2
                        DISPLAY_GREEN);         // color

  // Draw Minute Down Triangle
  display_fillTriangle( CLOCKDISPLAY_COLUMN_3,                            // x0
                        CLOCKDISPLAY_ROW_2,                               // y0
                        CLOCKDISPLAY_COLUMN_5,                            // x1
                        CLOCKDISPLAY_ROW_2,                               // y1
                        CLOCKDISPLAY_COLUMN_4,                            // x2
                        CLOCKDISPLAY_ROW_3 - CLOCKDISPLAY_ARROW_HEIGHT,   // y2
                        DISPLAY_GREEN);         // color

  // Draw Second Down Triangle
  display_fillTriangle( CLOCKDISPLAY_COLUMN_6,                            // x0
                        CLOCKDISPLAY_ROW_2,                               // y0
                        CLOCKDISPLAY_COLUMN_8,                            // x1
                        CLOCKDISPLAY_ROW_2,                               // y1
                        CLOCKDISPLAY_COLUMN_7,                            // x2
                        CLOCKDISPLAY_ROW_3 - CLOCKDISPLAY_ARROW_HEIGHT,   // y2
                        DISPLAY_GREEN);         // color

  // Draw the Clock Character ':' Separators
  display_drawChar( CLOCKDISPLAY_COLUMN_2,                      // x
                    CLOCKDISPLAY_ROW_1 + CLOCKDISPLAY_BUFFER,   // y
                    ':',                        // char to draw
                    DISPLAY_GREEN,              // color of text
                    DISPLAY_BLACK,              // color of background
                    CLOCKDISPLAY_CLOCK_TEXT_SIZE);           // size of text

  display_drawChar( CLOCKDISPLAY_COLUMN_5,                      // x
                    CLOCKDISPLAY_ROW_1 + CLOCKDISPLAY_BUFFER,   // y
                    ':',                        // char to draw
                    DISPLAY_GREEN,              // color of text
                    DISPLAY_BLACK,              // color of background
                    CLOCKDISPLAY_CLOCK_TEXT_SIZE);           // size of text

  // Draw the rest of the normal digits
  clockDisplay_updateTimeDisplay(UPDATE_ALL);
}

void clockDisplay_updateTimeDisplay(bool forceUpdateAll) {

  // If seconds was incremented
  if (seconds > CLOCKDISPLAY_MAX_SECS) {
    seconds = 0;  // rollover to 0

    // If minutes should also be updated
    if (forceUpdateAll) {
      minutes++;  // increment minutes
    }
  }

  // If seconds was decremented
  if (seconds < 0) {
    seconds = CLOCKDISPLAY_MAX_SECS; // set the seconds to MAX value

    // If minutes should also be updated
    if (forceUpdateAll) {
      minutes--;  // decrement minutes
    }
  }

  // If minutes was incremented
  if (minutes > CLOCKDISPLAY_MAX_MINS) {
    minutes = 0;  // rollover to zero

    // If hours should also be updated
    if (forceUpdateAll) {
      hours++;  // increment hours
    }
  }

  // If minutes was decremented
  if (minutes < 0) {
    minutes = CLOCKDISPLAY_MAX_MINS; //set the minutes to 59

    // If hours should also be updated
    if (forceUpdateAll) {
      hours--;  // decrement hours
    }
  }

  // If hours was incremented
  if (hours > CLOCKDISPLAY_MAX_HRS) {
    hours = 1;   // rollover hours
  }

  // If hours was decremented
  if (hours < 1) // MIN is 1, rather than 0 like mins and secs
  {
    hours = CLOCKDISPLAY_MAX_HRS;  // reset to MAX value
  }

  // Update current time
  sprintf(current_time, "%2d:%02d:%02d", hours, minutes, seconds);
  // Draw the time digits only if the times have changed
  uint8_t i;
  for (i = 0; i < CLOCKDISPLAY_NUM_CHARS; i++) {
    // If the digit has changed
    if (current_time[i] != old_time[i]) {
      // Redraw the digit with the current value
      clockDisplay_redrawDigit(i, current_time[i]);
    }
  }

  // Copy current_time into old_time
  strcpy(old_time, current_time);
}

void clockDisplay_performIncDec() {
  int16_t x, y;   // x and y coordinate of the touched point
  uint8_t pressure; // relative touch pressure, variable will be ignored
  int8_t regionID;  // ID of the touched region

  // Get the data of the touched point
  display_getTouchedPoint(&x, &y, &pressure);
  regionID = clockDisplay_getInputRegion(x, y);
  switch(regionID) {
    case CLOCKDISPLAY_REGION_0:  // Hours UP arrow
      hours++;
      break;
    case CLOCKDISPLAY_REGION_1:  // Minutes UP arrow
      minutes++;
      break;
    case CLOCKDISPLAY_REGION_2:  // Seconds UP arrow
      seconds++;
      break;
    case CLOCKDISPLAY_REGION_3:  // Hours DOWN arrow
      hours--;
      break;
    case CLOCKDISPLAY_REGION_4:  // Minutes DOWN arrow
      minutes--;
      break;
    case CLOCKDISPLAY_REGION_5:  // Seconds DOWN arrow
      seconds--;
      break;
    default:
      printf("Please touch an arrow.\n");
      break;
  }

  // Update the clock display without affecting all digits
  clockDisplay_updateTimeDisplay(CLOCKDISPLAY_DO_NOT_UPDATE_ALL);
}

void clockDisplay_advanceTimeOneSecond() {
  seconds++;  // advance time by 1 second
  clockDisplay_updateTimeDisplay(CLOCKDISPLAY_UPDATE_ALL); // update all digits
}

void clockDisplay_runTest() {
  printf("\n\nStarting Clock Display Test...\n");
  clockDisplay_init();  // Initialize the clock display
  clockDisplay_updateTimeDisplay(CLOCKDISPLAY_UPDATE_ALL);

  // Increment Hours
  while(!display_isTouched()); // wait for touch to initiate runTest
  while (display_isTouched()); // wait for user to let go of touchscreen
  while(!display_isTouched()) // increment until user presses touchscreen again
  {
    hours++;
    clockDisplay_updateTimeDisplay(CLOCKDISPLAY_DO_NOT_UPDATE_ALL);
    utils_msDelay(CLOCKDISPLAY_HALF_SECOND);
  }
  while(display_isTouched()); //wait for user to let go of touchscreen

  // Decrement Hours
  while(!display_isTouched()) // decrement until screen is touched again
  {
    hours--;
    clockDisplay_updateTimeDisplay(CLOCKDISPLAY_DO_NOT_UPDATE_ALL);
    utils_msDelay(CLOCKDISPLAY_HALF_SECOND);
  }
  while(display_isTouched()); //wait for user to let go of touchscreen

  // Increment Minutes
  while(!display_isTouched()) // increment until user presses touchscreen again
  {
    minutes++;
    clockDisplay_updateTimeDisplay(CLOCKDISPLAY_DO_NOT_UPDATE_ALL);
    utils_msDelay(CLOCKDISPLAY_HALF_SECOND);
  }
  while(display_isTouched()); //wait for user to let go of touchscreen

  // Decrement Minutes
  while(!display_isTouched()) // decrement until screen is touched again
  {
    minutes--;
    clockDisplay_updateTimeDisplay(CLOCKDISPLAY_DO_NOT_UPDATE_ALL);
    utils_msDelay(CLOCKDISPLAY_HALF_SECOND);
  }
  while(display_isTouched()); //wait for user to let go of touchscreen

  // Increment Seconds
  while(!display_isTouched()) // increment until user presses touchscreen again
  {
    seconds++;
    clockDisplay_updateTimeDisplay(CLOCKDISPLAY_DO_NOT_UPDATE_ALL);
    utils_msDelay(CLOCKDISPLAY_HALF_SECOND);
  }
  while(display_isTouched()); //wait for user to let go of touchscreen

  // Decrement Seconds
  while(!display_isTouched()) // decrement until screen is touched again
  {
    seconds--;
    clockDisplay_updateTimeDisplay(CLOCKDISPLAY_DO_NOT_UPDATE_ALL);
    utils_msDelay(CLOCKDISPLAY_HALF_SECOND);
  }
  while(display_isTouched()); //wait for user to let go of touchscreen

  // Run clock at 10x normal speed for 10 seconds
  int i;
  for (i = 0; i < CLOCKDISPLAY_TENTHS_IN_TEN_SECONDS; i++) {
    clockDisplay_advanceTimeOneSecond();
    utils_msDelay(CLOCKDISPLAY_TENTH_SECOND);
  }
  printf("\n\nClock Display Test FINISHED!!!\n");
}
