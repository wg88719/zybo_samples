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
uint8_t hours;  // track hours
uint8_t minutes;  // track minutes
uint8_t seconds;  // track seconds

char[] current_time;  // char array of digits representing the current time
char[] previous_time; // char array of digits representing the previous time


//*****************************************************************************
// Helper Functions for displaying the clock

/**
 * The touchscreen is divided into 6 rectangular regions numbered 0-5 as shown:
 *    +--+---+---+---+--+
 *    |  |   |   |   |  |
 *    +--+---+---+---+--+
 *    |  | 0 | 1 | 2 |  |
 *    +--+---+---+---+--+
 *    |  | H | M | S |  |
 *    +--+---+---+---+--+
 *    |  | 3 | 4 | 5 |  |
 *    +--+---+---+---+--+
 *    |  |   |   |   |  |
 *    +--+---+---+---+--+
 * where each region contains a rectangle. This function returns which region
 * was pressed by the user.
 * @param  x The x-coordinate of the point pressed by the user
 * @param  y The y-coordinate of the point pressed by the user
 * @return   The number representing the region pressed
 */
uint8_t clockDisplay_getInputRegion(int16_t x, int16_t y) {

}


//********************** End Helper Functions *********************************


void clockDisplay_init() {
  display_init(); // Initialize the LCD display
  display_fillScreen(DISPALY_BLACK);  // Blank the screen

  // Draw all components of the display that do not change:
  // Draw all of the user-input triangles:
  // Draw Hour Up Triangle
  display_fillTriangle( COLUMN_0,
                        ROW_2-BUFFER,
                        COLUMN_1,
                        ROW_3,
                        COLUMN_2,
                        ROW_2-BUFFER,
                        DISPLAY_GREEN);

  // Draw Minute Up Triangle

  // Draw Second Up Triangle

  // Draw Hours Down Triangle

  // Draw Minute Down Triangle

  // Draw Second Down Triangle

  // Draw the Clock Character ':' Separators
  display_drawChar( COLUMN_2,
                    ROW_2+TEXT_BLOCK_CENTER_HEIGHT,
                    ':',
                    DISPLAY_GREEN,
                    DISPLAY_BLACK,
                    SCALE_FACTOR);
}

void clockDisplay_updateTimeDisplay(bool forceUpdateAll) {
  if (forceUpdateAll) { // If forceUpdateAll, redraw all clock characters
    // Draw the Clock characters (HH:MM:SS)
    // Draw 1st Hour Digit
    // Draw 2nd Hour Digit
    // Draw 1st Minute Digit
    // Draw 2nd Minute Digit
    // Draw 1st Second Digit
    // Draw 2nd Second Digit
  }
  else {  // Otherwise, just redraw the ones that have changed

  }

}

void clockDisplay_performIncDec() {

}

void clockDisplay_advanceTimeOneSecond() {

}

void clockDisplay_runTest() {

}
