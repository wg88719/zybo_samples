//*****************************************************************************
// Luke Hsiao
// 11 May 2015
// Interface for drawing Tic Tac Toe
//*****************************************************************************

// Contants for specifying row/column values
#define ROW_0 0
#define ROW_1 1
#define ROW_2 2

#define COL_0 0
#define COL_1 1
#define COL_2 2

// Screen Position Macros
#define ONE_THIRD(X)      ((X)/3) // Divide the given number by 4
#define TWO_THIRDS(X)     ((X * 2)/3) // Divide the given number by 2

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
 * 	1. Draws the board
 * 	2. Each time a screen area is touched, the screen will draw an 'X' or an 'O'
 * 		 depending on whether switch 0 is slid up ('O') or down ('X').
 * 	3. When BTN0 is pushed, the screen is cleared.
 * 	4. The test is terminated by pressing BTN1.
 */
void ticTacToeDisplay_runTest();

/**
 * Draws the four board lines.
 */
void ticTacToeDisplay_drawBoardLines();
