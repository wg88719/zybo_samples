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

char current_time[NUM_CHARS];  // char array representing the current time
char previous_time[NUM_CHARS]; // char array representing the previous time


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
 * @return   The number representing the region pressed
 */
uint8_t clockDisplay_getInputRegion(int16_t x, int16_t y) {

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
  clockDisplay_init();  // Initialize the clock display
}
