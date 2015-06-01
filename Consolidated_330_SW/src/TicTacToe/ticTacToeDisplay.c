//*****************************************************************************
// Luke Hsiao
// 11 May 2015
// Implementation for drawing Tic Tac Toe
//*****************************************************************************

#include <stdio.h>
#include "ticTacToeDisplay.h"
#include "buttons.h"
#include "switches.h"
#include "supportFiles/display.h"
#include "supportFiles/utils.h"

/**
 * Helper function that draws an 'X' centered at the given parameters
 * @param center_x  The x coordinate to center the 'X' on.
 * @param center_y  The y coordinate to center the 'X' on.
 */
void ticTacToeDisplay_drawXAtPoint(uint16_t center_x, uint16_t center_y) {
  // Draw the '\' line of the 'X'
  display_drawLine (center_x - TICTACTOEDISPLAY_SYMBOL_SIZE,    // x0
                    center_y - TICTACTOEDISPLAY_SYMBOL_SIZE,    // y0
                    center_x + TICTACTOEDISPLAY_SYMBOL_SIZE,    // x1
                    center_y + TICTACTOEDISPLAY_SYMBOL_SIZE,    // y1
                    DISPLAY_GREEN); // color

  // Draw the '/' lines of the 'X'
  display_drawLine (center_x + TICTACTOEDISPLAY_SYMBOL_SIZE,    // x0
                    center_y - TICTACTOEDISPLAY_SYMBOL_SIZE,    // y0
                    center_x - TICTACTOEDISPLAY_SYMBOL_SIZE,    // x1
                    center_y + TICTACTOEDISPLAY_SYMBOL_SIZE,    // y1
                    DISPLAY_GREEN); // color
}

/**
 * Helper function that draws an 'O' centered at the given parameters
 * @param center_x  The x coordinate to center the 'O' on.
 * @param center_y  The y coordinate to center the 'O' on.
 */
void ticTacToeDisplay_drawOAtPoint(uint16_t center_x, uint16_t center_y) {
  // Draw the 'O'
  display_drawCircle(center_x,                      // x0
                     center_y,                      // y0
                     TICTACTOEDISPLAY_SYMBOL_SIZE,  // radius
                     DISPLAY_GREEN);                // color
}

/**
 * Helper function that returns the center y-coordinate of the given row.
 * @param  row The row to query for it's center y-coordinate. Valid inputs are
 *             TICTACTOEDISPLAY_ROW_0, TICTACTOEDISPLAY_ROW_1, TICTACTOEDISPLAY_ROW_2
 * @return     The y-coordinate of the center of the row.
 */
uint16_t ticTacToeDisplay_findCenterOfRow(uint8_t row) {
  uint16_t y_max = display_height();
  uint16_t row_0_y = TICTACTOEDISPLAY_ONE_SIXTH(y_max);  // Y-coordinate of the center of row 0
  uint16_t row_1_y = TICTACTOEDISPLAY_ONE_HALF(y_max);   // Y-coordinate of the center of row 1
  uint16_t row_2_y = TICTACTOEDISPLAY_FIVE_SIXTH(y_max); // Y-coordinate of the center of row 2
  // Return the pre-calculated value of the center y-coordinate of the row
  switch (row) {
    case TICTACTOEDISPLAY_ROW_0:
      return row_0_y;
    case TICTACTOEDISPLAY_ROW_1:
      return row_1_y;
    case TICTACTOEDISPLAY_ROW_2:
      return row_2_y;
    default:
    printf("Error, invalid row number!!\n\r");
      return 0;
  }
}

/**
 * Helper function that returns the center x-coordinate of the given column.
 * @param  column The column to query for it's center x-coordinate. Valid inputs are
 *                TICTACTOEDISPLAY_COL_0, TICTACTOEDISPLAY_COL_1, TICTACTOEDISPLAY_COL_2.
 * @return        The x-coordinate of the center of the row.
 */
uint16_t ticTacToeDisplay_findCenterOfColumn(uint8_t column) {
  uint16_t x_max = display_width();
  uint16_t col_0_x = TICTACTOEDISPLAY_ONE_SIXTH(x_max);  // X-coordinate of the center of column 0
  uint16_t col_1_x = TICTACTOEDISPLAY_ONE_HALF(x_max);   // X-coordinate of the center of column 1
  uint16_t col_2_x = TICTACTOEDISPLAY_FIVE_SIXTH(x_max); // X-coordinate of the center of column 2
  // Return the pre-calculated value of the center y-coordinate of the row
  switch (column) {
    case TICTACTOEDISPLAY_COL_0:
      return col_0_x;
    case TICTACTOEDISPLAY_COL_1:
      return col_1_x;
    case TICTACTOEDISPLAY_COL_2:
      return col_2_x;
    default:
    printf("Error, invalid column number!!\n\r");
      return 0;
  }
}

/**
 * Helper function that clears all the Xs and Os from the board.
 */
void ticTacToeDisplay_clearScreen() {
  display_fillScreen(DISPLAY_BLACK);  // blank the screen
  ticTacToeDisplay_drawBoardLines();  // redraw the lines
}

 /**
  * Helper function that takes an x and y coordinate and returns the
  * respective row/column.
  * @param x      x-coordinate of the touched point
  * @param y      y-coordinate of the touched point
  * @param row    Address to the row variable to store computed value in
  * @param column Address to the column variable to store computed value in
  */
void ticTacToeDisplay_getInputRegion(int16_t x, int16_t y, uint8_t* row, uint8_t* column) {
  // Set the values of the sub box dimensions
  uint16_t x_max = display_width();
  uint16_t y_max = display_height();
  uint16_t box_width = TICTACTOEDISPLAY_ONE_THIRD(x_max);
  uint16_t box_height = TICTACTOEDISPLAY_ONE_THIRD(y_max);

  // Error condition for values off the screen
  if (x < 0 || y < 0 || x > x_max || y > y_max) {
    return;  // Invalid touch data
  }

  // Parse through row 0
  if (y < (TICTACTOEDISPLAY_ROW_1*box_height) && y > TICTACTOEDISPLAY_ROW_0) {
  *row = TICTACTOEDISPLAY_ROW_0;
    // Check column 0
    if (x < (TICTACTOEDISPLAY_COL_1 * box_width) && x > TICTACTOEDISPLAY_COL_0) {
      *column = TICTACTOEDISPLAY_COL_0;
    }
    // Check column 1
    else if (x < (TICTACTOEDISPLAY_COL_2 * box_width)) {
      *column = TICTACTOEDISPLAY_COL_1;
    }
    // Check column 2
    else if (x < x_max) {
      *column = TICTACTOEDISPLAY_COL_2;
    }
  }

  // Parse through row 1
  else if (y < (TICTACTOEDISPLAY_ROW_2*box_height)) {
  *row = TICTACTOEDISPLAY_ROW_1;
    // Check column 0
    if (x < (TICTACTOEDISPLAY_COL_1 * box_width)) {
      *column = TICTACTOEDISPLAY_COL_0;
    }
    // Check column 1
    else if (x < (TICTACTOEDISPLAY_COL_2 * box_width)) {
      *column = TICTACTOEDISPLAY_COL_1;
    }
    // Check column 2
    else if (x < x_max) {
      *column = TICTACTOEDISPLAY_COL_2;
    }
  }

  // Parse through row 2
  else if (y < y_max) {
  *row = TICTACTOEDISPLAY_ROW_2;
    // Check column 0
    if (x < (TICTACTOEDISPLAY_COL_1 * box_width) && x > TICTACTOEDISPLAY_COL_0) {
      *column = TICTACTOEDISPLAY_COL_0;
    }
    // Check column 1
    else if (x < (TICTACTOEDISPLAY_COL_2 * box_width)) {
      *column = TICTACTOEDISPLAY_COL_1;
    }
    // Check column 2
    else if (x < x_max) {
      *column = TICTACTOEDISPLAY_COL_2;
    }
  }
}

void ticTacToeDisplay_init() {
  display_init();  // Initialize display, which sets Rotation = 1 by default
  ticTacToeDisplay_clearScreen(); // clear the display and draw lines
}

void ticTacToeDisplay_drawX(uint8_t row, uint8_t column) {
  uint16_t center_x, center_y;
  // find the center y-coord of the given row
  center_y = ticTacToeDisplay_findCenterOfRow(row);

  // find the center x-coord of the given column
  center_x = ticTacToeDisplay_findCenterOfColumn(column);

  // Draw an 'X' centered at the given points.
  ticTacToeDisplay_drawXAtPoint(center_x, center_y);
}

void ticTacToeDisplay_drawO(uint8_t row, uint8_t column) {
  uint16_t center_x, center_y;
  // find the center y-coord of the given row
  center_y = ticTacToeDisplay_findCenterOfRow(row);

  // find the center x-coord of the given column
  center_x = ticTacToeDisplay_findCenterOfColumn(column);

  // Draw an 'X' centered at the given points.
  ticTacToeDisplay_drawOAtPoint(center_x, center_y);
}

void ticTacToeDisplay_touchScreenComputeBoardRowColumn(uint8_t* row, uint8_t* column) {
  // old touch data was cleared previously in ticTacToeControl

  int16_t x, y;   // x and y coordinate of the touched point
  uint8_t pressure; // relative touch pressure, variable will be ignored

  // Get the data of the touched point
  display_getTouchedPoint(&x, &y, &pressure);

  // Calculate the row and column from the x and y coordinate
  ticTacToeDisplay_getInputRegion(x, y, row, column);
}

void ticTacToeDisplay_runTest() {
  uint8_t row, column;
  int32_t buttonValue;
  int32_t switchValue;
  printf("\n\nStarting Tic Tac Toe Display Test...\n");

  // Perform all initializations
  buttons_init();
  switches_init();
  ticTacToeDisplay_init();

  buttonValue = buttons_read(); // Do an initial read of button values

  // While BTN 1 isn't pressed, poll buttons, touchscreen, and switches
  while(!(buttonValue & BUTTONS_BTN1_MASK)) {  // While button 1 isn't pressed
    buttonValue = buttons_read();

    // If BTN0 is pressed, clear the screen
    if (buttonValue & BUTTONS_BTN0_MASK) {
      ticTacToeDisplay_clearScreen();
    }

    // If user pressed the touchscreen display
    if (display_isTouched()) {
      while(display_isTouched()); // wait for user to let go of touchscreen

      // Compute which row/column was touched
      ticTacToeDisplay_touchScreenComputeBoardRowColumn(&row, &column);

      // Read switches to determine whether to draw 'O' or 'X'
      switchValue = switches_read();
      if (switchValue & SWITCHES_SW0_MASK) { // If SW0 is HIGH, draw 'O'
        ticTacToeDisplay_drawO(row, column);
      }
      else {  // If SW0 is LOW, draw 'X'
        ticTacToeDisplay_drawX(row, column);
      }
    }
  }
  // Give the user indication that the test is completed.
  printf("\nEnding Tic Tac Toe Display Test...\n");
  display_fillScreen(DISPLAY_WHITE);
}

void ticTacToeDisplay_drawBoardLines() {

  // Calculate the coordinates of the lines to draw
  uint16_t x_max = display_width();
  uint16_t y_max = display_height();
  uint16_t x_one_third = TICTACTOEDISPLAY_ONE_THIRD(x_max);
  uint16_t x_two_third = TICTACTOEDISPLAY_TWO_THIRD(x_max);
  uint16_t y_one_third = TICTACTOEDISPLAY_ONE_THIRD(y_max);
  uint16_t y_two_third = TICTACTOEDISPLAY_TWO_THIRD(y_max);

  // Draw the two VERTICAL lines
  display_drawLine (x_one_third,    // x0
                    0,              // y0
                    x_one_third,    // x1
                    y_max,          // y1
                    DISPLAY_GREEN); // color

  display_drawLine (x_two_third,    // x0
                    0,              // y0
                    x_two_third,    // x1
                    y_max,          // y1
                    DISPLAY_GREEN); // color

  // Draw the two HORIZONTAL lines
  display_drawLine (0,              // x0
                    y_one_third,    // y0
                    x_max,          // x1
                    y_one_third,    // y1
                    DISPLAY_GREEN); // color

  display_drawLine (0,              // x0
                    y_two_third,    // y0
                    x_max,          // x1
                    y_two_third,    // y1
                    DISPLAY_GREEN); // color
}
