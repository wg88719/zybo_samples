//*****************************************************************************
// Luke Hsiao
// 11 May 2015
// Interface for drawing Tic Tac Toe
//*****************************************************************************

#include <stdint.h>
#include <stdlib.h>

// Constants for specifying row/column values
#define TICTACTOEDISPLAY_ROW_0 0
#define TICTACTOEDISPLAY_ROW_1 1
#define TICTACTOEDISPLAY_ROW_2 2

#define TICTACTOEDISPLAY_COL_0 0
#define TICTACTOEDISPLAY_COL_1 1
#define TICTACTOEDISPLAY_COL_2 2

// Screen Position Macros
#define TICTACTOEDISPLAY_ONE_THIRD(X)   ((X) / 3)  // Divide the given number by 4
#define TICTACTOEDISPLAY_TWO_THIRD(X)   ((X * 2) / 3) // Multiply by 2, then divide by 3 to reduce rounding err.
#define TICTACTOEDISPLAY_ONE_SIXTH(X)  	((X) / 6)    // Divide the given number by 6
#define TICTACTOEDISPLAY_ONE_HALF(X)   	((X) / 2)    // Divide the given number by 2
#define TICTACTOEDISPLAY_FIVE_SIXTH(X)  ((X * 5) / 6) // Multiply by 5 then divide by 6 to reduce rounding err.


#define TICTACTOEDISPLAY_SYMBOL_SIZE 25  // Used as the radius of 'O' and the distance from
            // a side to the center of 'X'

/**
 * Inits the tic-tac-toe display, draws the lines that form the board.
 */
void ticTacToeDisplay_init();

/**
 * Draws an 'X' at the specified row and column.
 * @param row    Row number (0, 1, or 2)
 * @param column Column number (0, 1, or 2)
 */
void ticTacToeDisplay_drawX(uint8_t row, uint8_t column);

/**
 * Draws an 'O' at the specified row and column.
 * @param row    Row number (0, 1 or 2)
 * @param column Column number (0, 1, or 2)
 */
void ticTacToeDisplay_drawO(uint8_t row, uint8_t column);

/**
 * After a touch has been detected and after the proper delay, this sets the
 * row and column arguments according to where the user touched the board.
 * The values return will be (0, 1, or 2).
 * @param row    Pointer to the row variable to modify
 * @param column Pointer to the column variable to modify
 */
void ticTacToeDisplay_touchScreenComputeBoardRowColumn(uint8_t* row, uint8_t* column);

/**
 * Runs a test of the display by doing the following:
 *   1. Draws the board
 *   2. Each time a screen area is touched, the screen will draw an 'X' or an 'O'
 *      depending on whether switch 0 is slid up ('O') or down ('X').
 *   3. When BTN0 is pushed, the screen is cleared.
 *   4. The test is terminated by pressing BTN1.
 */
void ticTacToeDisplay_runTest();

/**
 * Draws the four board lines.
 */
void ticTacToeDisplay_drawBoardLines();
