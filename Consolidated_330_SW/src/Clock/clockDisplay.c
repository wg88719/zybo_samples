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

// Global variables for tracking time
// Used signed ints to utilize negative values as a signal to rollover
int8_t hours = 0;  // track hours
int8_t minutes = 0;  // track minutes
int8_t seconds = 0;  // track seconds

char current_time[NUM_CHARS];   // char array representing the current time
char old_time[NUM_CHARS]; // char array representing the previous time


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
    return REGION_ERR;
  }

  // Parse through the UP arrows
  if (y < ROW_1 && y > ROW_0) {
    // Check Region 0
    if (x < COLUMN_2 && x > COLUMN_0) {
      return REGION_0;
    }
    // Check Region 1
    if (x < COLUMN_5 && x > COLUMN_3) {
      return REGION_1;
    }
    // Check Region 2
    if (x < COLUMN_8 && x > COLUMN_6) {
      return REGION_2;
    }
  }

  // Parse through the DOWN arrows
  if (y < ROW_3 && y > ROW_2) {
    // Check Region 3
    if (x < COLUMN_2 && x > COLUMN_0) {
      return REGION_3;
    }
    // Check Region 4
    if (x < COLUMN_5 && x > COLUMN_3) {
      return REGION_4;
    }
    // Check Region 5
    if (x < COLUMN_8 && x > COLUMN_6) {
      return REGION_5;
    }
  }

  // If not in any of these regions, return REGION_ERR
  return REGION_ERR;
}

/**
 * Draws the specified character at the specified location on the screen
 * @param index The column index of the clock to draw the character at
 * @param c     The character to draw
 */
void clockDisplay_redrawDigit(uint8_t index, char c) {
  // Draw the character based on the index
  switch (index) {
    case TENS_HRS:   // Tens-digit for Hours
      display_drawChar( COLUMN_0,                   // x
                        ROW_1 + BUFFER,             // y
                        c,                          // char to draw
                        DISPLAY_GREEN,              // color of text
                        DISPLAY_BLACK,              // color of background
                        CLOCK_TEXT_SIZE);           // size of text
      break;
    case ONES_HRS:   // Ones-digit for Hours
      display_drawChar( COLUMN_1,                   // x
                        ROW_1 + BUFFER,             // y
                        c,                          // char to draw
                        DISPLAY_GREEN,              // color of text
                        DISPLAY_BLACK,              // color of background
                        CLOCK_TEXT_SIZE);           // size of text
      break;
    case COLON_1:   // First ':', do nothing
      break;
    case TENS_MINS:   // Tens-digit for Mins
      display_drawChar( COLUMN_3,                   // x
                        ROW_1 + BUFFER,             // y
                        c,                          // char to draw
                        DISPLAY_GREEN,              // color of text
                        DISPLAY_BLACK,              // color of background
                        CLOCK_TEXT_SIZE);           // size of text
      break;
    case ONES_MINS:   // Ones-digit for Mins
      display_drawChar( COLUMN_4,                   // x
                        ROW_1 + BUFFER,             // y
                        c,                          // char to draw
                        DISPLAY_GREEN,              // color of text
                        DISPLAY_BLACK,              // color of background
                        CLOCK_TEXT_SIZE);           // size of text
      break;
    case COLON_2:   // 2nd ':', do nothing
      break;
    case TENS_SECS:   // Tens-digit for Secs
      display_drawChar( COLUMN_6,                   // x
                        ROW_1 + BUFFER,             // y
                        c,                          // char to draw
                        DISPLAY_GREEN,              // color of text
                        DISPLAY_BLACK,              // color of background
                        CLOCK_TEXT_SIZE);           // size of text
      break;
    case ONES_SECS:   // Ones-digit for Secs
      display_drawChar( COLUMN_7,                   // x
                        ROW_1 + BUFFER,             // y
                        c,                          // char to draw
                        DISPLAY_GREEN,              // color of text
                        DISPLAY_BLACK,              // color of background
                        CLOCK_TEXT_SIZE);           // size of text
      break;
    default:  // Otherwise, do nothing
      break;
  }
}

/**
 * Helper function that draws the lines representing the borders of each of
 * the sub boxes. Used for Debugging.
 */
void clockDisplay_drawLines() {
  // Draw Vertical Lines
  display_drawLine(COLUMN_0, ROW_0, COLUMN_0, ROW_3, DISPLAY_RED);
  display_drawLine(COLUMN_1, ROW_0, COLUMN_1, ROW_3, DISPLAY_RED);
  display_drawLine(COLUMN_2, ROW_0, COLUMN_2, ROW_3, DISPLAY_RED);
  display_drawLine(COLUMN_3, ROW_0, COLUMN_3, ROW_3, DISPLAY_RED);
  display_drawLine(COLUMN_4, ROW_0, COLUMN_4, ROW_3, DISPLAY_RED);
  display_drawLine(COLUMN_5, ROW_0, COLUMN_5, ROW_3, DISPLAY_RED);
  display_drawLine(COLUMN_6, ROW_0, COLUMN_6, ROW_3, DISPLAY_RED);
  display_drawLine(COLUMN_7, ROW_0, COLUMN_7, ROW_3, DISPLAY_RED);
  display_drawLine(COLUMN_8, ROW_0, COLUMN_8, ROW_3, DISPLAY_RED);

  // Draw Horizontal Lines
  display_drawLine(COLUMN_0, ROW_0, COLUMN_8, ROW_0, DISPLAY_RED);
  display_drawLine(COLUMN_0, ROW_1, COLUMN_8, ROW_1, DISPLAY_RED);
  display_drawLine(COLUMN_0, ROW_2, COLUMN_8, ROW_2, DISPLAY_RED);
  display_drawLine(COLUMN_0, ROW_3, COLUMN_8, ROW_3, DISPLAY_RED);
}

//********************** End Helper Functions *********************************


void clockDisplay_init() {
  display_init(); // Initialize the LCD display
  display_fillScreen(DISPLAY_BLACK);  // Blank the screen

  // Initialize the global variables
  seconds = INITIAL_SECONDS;
  minutes = INITIAL_MINUTES;
  hours = INITIAL_HOURS;

  printf("BUFFER: %d\n", BUFFER);
  printf("ORIGIN_X: %d\n", ORIGIN_X);
  printf("ORIGIN_Y: %d\n", ORIGIN_Y);

  // Initialize the current_time to the initial value
  sprintf(current_time, "%2d:%02d:%02d", hours, minutes, seconds);

  //////////////////////////////////////////////////////////////
  // Draw all components of the display that do not change:
  //////////////////////////////////////////////////////////////

  // Draw all of the user-input triangles:

  // Draw Hour Up Triangle
  display_fillTriangle( COLUMN_0,               // x0
                        ROW_1,                  // y0
                        COLUMN_2,               // x1
                        ROW_1,                  // y1
                        COLUMN_1,               // x2
                        ROW_0 + ARROW_HEIGHT,   // y2
                        DISPLAY_GREEN);         // color

  // Draw Minute Up Triangle
  display_fillTriangle( COLUMN_3,               // x0
                        ROW_1,                  // y0
                        COLUMN_5,               // x1
                        ROW_1,                  // y1
                        COLUMN_4,               // x2
                        ROW_0 + ARROW_HEIGHT,   // y2
                        DISPLAY_GREEN);         // color

  // Draw Second Up Triangle
  display_fillTriangle( COLUMN_6,               // x0
                        ROW_1,                  // y0
                        COLUMN_8,               // x1
                        ROW_1,                  // y1
                        COLUMN_7,               // x2
                        ROW_0 + ARROW_HEIGHT,   // y2
                        DISPLAY_GREEN);         // color

  // Draw Hours Down Triangle
  display_fillTriangle( COLUMN_0,               // x0
                        ROW_2,                  // y0
                        COLUMN_2,               // x1
                        ROW_2,                  // y1
                        COLUMN_1,               // x2
                        ROW_3 - ARROW_HEIGHT,   // y2
                        DISPLAY_GREEN);         // color

  // Draw Minute Down Triangle
  display_fillTriangle( COLUMN_3,               // x0
                        ROW_2,                  // y0
                        COLUMN_5,               // x1
                        ROW_2,                  // y1
                        COLUMN_4,               // x2
                        ROW_3 - ARROW_HEIGHT,   // y2
                        DISPLAY_GREEN);         // color

  // Draw Second Down Triangle
  display_fillTriangle( COLUMN_6,               // x0
                        ROW_2,                  // y0
                        COLUMN_8,               // x1
                        ROW_2,                  // y1
                        COLUMN_7,               // x2
                        ROW_3 - ARROW_HEIGHT,   // y2
                        DISPLAY_GREEN);         // color

  // Draw the Clock Character ':' Separators
  display_drawChar( COLUMN_2,                   // x
                    ROW_1 + BUFFER,             // y
                    ':',                        // char to draw
                    DISPLAY_GREEN,              // color of text
                    DISPLAY_BLACK,              // color of background
                    CLOCK_TEXT_SIZE);           // size of text

  display_drawChar( COLUMN_5,                   // x
                    ROW_1 + BUFFER,             // y
                    ':',                        // char to draw
                    DISPLAY_GREEN,              // color of text
                    DISPLAY_BLACK,              // color of background
                    CLOCK_TEXT_SIZE);           // size of text

  //clockDisplay_drawLines();
}

void clockDisplay_updateTimeDisplay(bool forceUpdateAll) {

  // TODO update the global variables? Or do I do this somewhere else?

  // If seconds was incremented
  if (seconds > MAX_SECS) {
    seconds = 0;  // rollover to 0

    // If minutes should also be updated
    if (forceUpdateAll) {
      minutes++;  // increment minutes
    }
  }

  // If seconds was decremented
  if (seconds < 0) {
    seconds = MAX_SECS; // set the seconds to MAX value

    // If minutes should also be updated
    if (forceUpdateAll) {
      minutes--;  // decrement minutes
    }
  }

  // If minutes was incremented
  if (minutes > MAX_MINS) {
    minutes = 0;  // rollover to zero

    // If hours should also be updated
    if (forceUpdateAll) {
      hours++;  // increment hours
    }
  }

  // If minutes was decremented
  if (minutes < 0) {
    minutes = MAX_MINS; //set the minutes to 59

    // If hours should also be updated
    if (forceUpdateAll) {
      hours--;  // decrement hours
    }
  }

  // If hours was incremented
  if (hours > MAX_HRS) {
    hours = 1;   // rollover hours
  }

  // If hours was decremented
  if (hours < 1) // MIN is 1, rather than 0 like mins and secs
  {
    hours = MAX_HRS;  // reset to MAX value
  }

  // Update current time
  sprintf(current_time, "%2d:%02d:%02d", hours, minutes, seconds);

  // Draw the time digits only if the times have changed
  uint8_t i;
  for (i = 0; i < NUM_CHARS; i++) {
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
    case REGION_0:  // Hours UP arrow
      hours++;
      break;
    case REGION_1:  // Minutes UP arrow
      minutes++;
      break;
    case REGION_2:  // Seconds UP arrow
      seconds++;
      break;
    case REGION_3:  // Hours DOWN arrow
      hours--;
      break;
    case REGION_4:  // Minutes DOWN arrow
      minutes--;
      break;
    case REGION_5:  // Seconds DOWN arrow
      seconds--;
      break;
    default:
      printf("Please touch an arrow.\n");
      break;
  }

  // Update the clock display without affecting all digits
  clockDisplay_updateTimeDisplay(DO_NOT_UPDATE_ALL);
}

void clockDisplay_advanceTimeOneSecond() {
  seconds++;  // advance time by 1 second
  clockDisplay_updateTimeDisplay(UPDATE_ALL); // update all digits
}

void clockDisplay_runTest() {
  printf("\n\nStarting Clock Display Test...\n");
  clockDisplay_init();  // Initialize the clock display
  clockDisplay_updateTimeDisplay(UPDATE_ALL);

  // Increment Hours
  while(!display_isTouched()); // wait for touch to initiate runTest
  while (display_isTouched()); // wait for user to let go of touchscreen
  while(!display_isTouched()) // increment until user presses touchscreen again
  {
    hours++;
    clockDisplay_updateTimeDisplay(DO_NOT_UPDATE_ALL);
    utils_msDelay(500);
  }
  while(display_isTouched()); //wait for user to let go of touchscreen

  // Decrement Hours
  while(!display_isTouched()) // decrement until screen is touched again
  {
    hours--;
    clockDisplay_updateTimeDisplay(DO_NOT_UPDATE_ALL);
    utils_msDelay(500);
  }
  while(display_isTouched()); //wait for user to let go of touchscreen

  // Increment Minutes
  while(!display_isTouched()) // increment until user presses touchscreen again
  {
    minutes++;
    clockDisplay_updateTimeDisplay(DO_NOT_UPDATE_ALL);
    utils_msDelay(500);
  }
  while(display_isTouched()); //wait for user to let go of touchscreen

  // Decrement Minutes
  while(!display_isTouched()) // decrement until screen is touched again
  {
    minutes--;
    clockDisplay_updateTimeDisplay(DO_NOT_UPDATE_ALL);
    utils_msDelay(500);
  }
  while(display_isTouched()); //wait for user to let go of touchscreen

  // Increment Seconds
  while(!display_isTouched()) // increment until user presses touchscreen again
  {
    seconds++;
    clockDisplay_updateTimeDisplay(DO_NOT_UPDATE_ALL);
    utils_msDelay(500);
  }
  while(display_isTouched()); //wait for user to let go of touchscreen

  // Decrement Seconds
  while(!display_isTouched()) // decrement until screen is touched again
  {
    seconds--;
    clockDisplay_updateTimeDisplay(DO_NOT_UPDATE_ALL);
    utils_msDelay(500);
  }
  while(display_isTouched()); //wait for user to let go of touchscreen

  // Run clock at 10x normal speed for 10 seconds
  int i;
  for (i = 0; i < TENTHS_IN_TEN_SECONDS; i++) {
    clockDisplay_advanceTimeOneSecond();
    utils_msDelay(100);
  }
}
