//*****************************************************************************
// Luke Hsiao
// 11 May 2015
// Implementation for drawing Tic Tac Toe
//*****************************************************************************

#include "ticTacToeDisplay.h"

// Global Screen size values
uint16_t x_max;   // Store the maximum x-value of the screen
uint16_t y_max;   // Store the maximum y-value of the screen

// Values of screen positions
uint16_t x_one_third;  // x-value of the left fourth of the screen
uint16_t x_two_third;  // x-value of the middle of the screen
uint16_t y_one_third;  // y-value of right fourth of the screen
uint16_t y_two_third;  // y-value of the top fourth of the screen

void ticTacToeDisplay_init() {
  display_init();  // Initialize display, which sets Rotation = 1 by default
  display_fillScreen(DISPLAY_BLACK); // blank the screen

  // Set the values of the global variables
  x_max = display_width();
  y_max = display_height();

  x_one_third = ONE_THIRD(x_max);
  x_two_third = TWO_THIRD(x_max);
  y_one_third = ONE_THIRD(y_max);
  y_two_third = TWO_THIRD(y_max);
}

void ticTacToeDisplay_drawX(uint8_t row, uint8_t column) {

}

void ticTacToeDisplay_drawO(uint8_t row, uint8_t column) {

}

void ticTacToeDisplay_touchScreenComputeBoardRowColumn(uint8_t* row, uint8_t* column) {
  display_clearOldTouchData();  // clear old data to read current coordinates

}

void ticTacToeDisplay_runTest() {

}

void ticTacToeDisplay_drawBoardLines() {
  // Draw the two VERTICAL lines
  display_drawLine (x_one_third,    // x0
                    0,              // y0
                    x_one_third,    // x1
                    y_max,          // y1
                    DISPALY_GREEN); // color

  display_drawLine (x_two_third,    // x0
                    0,              // y0
                    x_two_third,    // x1
                    y_max,          // y1
                    DISPALY_GREEN); // color

  // Draw the two HORIZONTAL lines
  display_drawLine (0,              // x0
                    y_one_third,    // y0
                    x_max,          // x1
                    y_one_third,    // y1
                    DISPALY_GREEN); // color

  display_drawLine (0,              // x0
                    y_two_third,    // y0
                    x_max,          // x1
                    y_two_third,    // y1
                    DISPALY_GREEN); // color
}
