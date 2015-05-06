//*****************************************************************************
// Luke Hsiao
// 4 May 2015
// Driver file for displaying the clock
//*****************************************************************************

#include <stdio.h>
#include "clockDisplay.h"
#include "xparameters.h"
#include "supportFiles/display.h"

// Global variables for tracking time
// Used signed ints to utilize negative values as a signal to rollover
int8_t hours;  // track hours
int8_t minutes;  // track minutes
int8_t seconds;  // track seconds

char current_time[NUM_CHARS];  // char array representing the current time
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
 * @return   The number representing the region pressed or -1 for other
 */
int8_t clockDisplay_getInputRegion(int16_t x, int16_t y) {

}

/**
 * Decrements the global seconds variable and handles rollover cases
 */
void clockDisplay_decrementSec() {

}

/**
 * Increments the global seconds variable and handles rollover cases
 */
void clockDisplay_incrementSec() {

}

/**
 * Decrements the global minutes variable and handles rollover cases
 */
void clockDisplay_decrementMin() {

}

/**
 * Increments the global minutes variable and handles rollover cases
 */
void clockDisplay_incrementMin() {

}

/**
 * Decrements the global hours variable and handles rollover cases
 */
void clockDisplay_decrementHour() {

}

/**
 * Increments the global hours variable and handles rollover cases
 */
void clockDisplay_incrementHour() {

}

/**
 * Draws the specified character at the specified location on the screen
 * @param index The column index of the clock to draw the character at
 * @param c     The character to draw
 */
void clockDisplay_redrawDigit(uint8_t index, char c) {
  // Draw the character based on the index
  switch (index) {
    case 0:   // Tens-digit for Hours
      display_drawChar( COLUMN_0,                   // x
                        ROW_1,                      // y
                        c,                          // char to draw
                        DISPLAY_GREEN,              // color of text
                        DISPLAY_BLACK,              // color of background
                        CLOCK_TEXT_SIZE);           // size of text
      break;
    case 1:   // Ones-digit for Hours
      display_drawChar( COLUMN_1,                   // x
                        ROW_1,                      // y
                        c,                          // char to draw
                        DISPLAY_GREEN,              // color of text
                        DISPLAY_BLACK,              // color of background
                        CLOCK_TEXT_SIZE);           // size of text
      break;
    case 2:   // First ':', do nothing
      break;
    case 3:   // Tens-digit for Mins
      display_drawChar( COLUMN_3,                   // x
                        ROW_1,                      // y
                        c,                          // char to draw
                        DISPLAY_GREEN,              // color of text
                        DISPLAY_BLACK,              // color of background
                        CLOCK_TEXT_SIZE);           // size of text
      break;
    case 4:   // Ones-digit for Mins
      display_drawChar( COLUMN_4,                   // x
                        ROW_1,                      // y
                        c,                          // char to draw
                        DISPLAY_GREEN,              // color of text
                        DISPLAY_BLACK,              // color of background
                        CLOCK_TEXT_SIZE);           // size of text
      break;
    case 5:   // 2nd ':', do nothing
      break;
    case 6:   // Tens-digit for Secs
      display_drawChar( COLUMN_6,                   // x
                        ROW_1,                      // y
                        c,                          // char to draw
                        DISPLAY_GREEN,              // color of text
                        DISPLAY_BLACK,              // color of background
                        CLOCK_TEXT_SIZE);           // size of text
      break;
    case 7:   // Ones-digit for Secs
      display_drawChar( COLUMN_7,                   // x
                        ROW_1,                      // y
                        c,                          // char to draw
                        DISPLAY_GREEN,              // color of text
                        DISPLAY_BLACK,              // color of background
                        CLOCK_TEXT_SIZE);           // size of text
      break;
    default:  // Otherwise, do nothing
      break;
  }
}

//********************** End Helper Functions *********************************


void clockDisplay_init() {
  display_init(); // Initialize the LCD display
  display_fillScreen(DISPLAY_BLACK);  // Blank the screen

  //////////////////////////////////////////////////////////////
  // Draw all components of the display that do not change:
  //////////////////////////////////////////////////////////////

  // Draw all of the user-input triangles:

  // Draw Hour Up Triangle
  display_fillTriangle( COLUMN_0,               // x0
                        ROW_1 - BUFFER,         // y0
                        COLUMN_2,               // x1
                        ROW_1 - BUFFER,         // y1
                        COLUMN_1,               // x2
                        ROW_0 + ARROW_HEIGHT,   // y2
                        DISPLAY_GREEN);         // color

  // Draw Minute Up Triangle
  display_fillTriangle( COLUMN_3,               // x0
                        ROW_1 - BUFFER,         // y0
                        COLUMN_5,               // x1
                        ROW_1 - BUFFER,         // y1
                        COLUMN_4,               // x2
                        ROW_0 + ARROW_HEIGHT,   // y2
                        DISPLAY_GREEN);         // color

  // Draw Second Up Triangle
  display_fillTriangle( COLUMN_6,               // x0
                        ROW_1 - BUFFER,         // y0
                        COLUMN_8,               // x1
                        ROW_1 - BUFFER,         // y1
                        COLUMN_5,               // x2
                        ROW_0 + ARROW_HEIGHT,   // y2
                        DISPLAY_GREEN);         // color

  // Draw Hours Down Triangle
  display_fillTriangle( COLUMN_0,               // x0
                        ROW_2 + BUFFER,         // y0
                        COLUMN_2,               // x1
                        ROW_2 + BUFFER,         // y1
                        COLUMN_1,               // x2
                        ROW_3 - ARROW_HEIGHT,   // y2
                        DISPLAY_GREEN);         // color

  // Draw Minute Down Triangle
  display_fillTriangle( COLUMN_3,               // x0
                        ROW_2 + BUFFER,         // y0
                        COLUMN_5,               // x1
                        ROW_1 - BUFFER,         // y1
                        COLUMN_4,               // x2
                        ROW_3 - ARROW_HEIGHT,   // y2
                        DISPLAY_GREEN);         // color

  // Draw Second Down Triangle
  display_fillTriangle( COLUMN_6,               // x0
                        ROW_2 + BUFFER,         // y0
                        COLUMN_8,               // x1
                        ROW_2 + BUFFER,         // y1
                        COLUMN_5,               // x2
                        ROW_3 - ARROW_HEIGHT,   // y2
                        DISPLAY_GREEN);         // color

  // Draw the Clock Character ':' Separators
  display_drawChar( COLUMN_2,                   // x
                    ROW_1,                      // y
                    ':',                        // char to draw
                    DISPLAY_GREEN,              // color of text
                    DISPLAY_BLACK,              // color of background
                    CLOCK_TEXT_SIZE);           // size of text

  display_drawChar( COLUMN_5,                   // x
                    ROW_1,                      // y
                    ':',                        // char to draw
                    DISPLAY_GREEN,              // color of text
                    DISPLAY_BLACK,              // color of background
                    CLOCK_TEXT_SIZE);           // size of text
}

void clockDisplay_updateTimeDisplay(bool forceUpdateAll) {

  // TODO update the global variables? Or do I do this somewhere else?

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

}

void clockDisplay_advanceTimeOneSecond() {

}

void clockDisplay_runTest() {
  clockDisplay_init();  // Initialize the clock display
}
