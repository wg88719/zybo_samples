//*****************************************************************************
// Luke Hsiao
// 4 May 2015
// Interface for displaying the clock
//
// Details:
//    Note that the area of the touchscreen will be displayed and scaled usering
//    these regions:
//    +--+---+---+---+--+
//    |  |   |   |   |  |
//    +--+---+---+---+--+
//    |  | ▲ | ▲ | ▲ |  |
//    +--+---+---+---+--+
//    |  | H | M | S |  |
//    +--+---+---+---+--+
//    |  | ▼ | ▼ | ▼ |  |
//    +--+---+---+---+--+
//    |  |   |   |   |  |
//    +--+---+---+---+--+
//*****************************************************************************

#include <stdbool.h>

//*****************************************************************************
// User Editable Macros:

// Edit this to change the size of the clock chars. Only set this value to
// 3, 4, 5, or 6. Otherwise, the clock will NOT display correctly.
#define CLOCK_TEXT_SIZE 5

// Values to Initialize clock to on startup
#define INITIAL_HOURS   12
#define INITIAL_MINUTES 59
#define INITIAL_SECONDS 55
//***************** End User Editable Macros **********************************

// Region IDs for the touchscreen
#define REGION_0  0
#define REGION_1  1
#define REGION_2  2
#define REGION_3  3
#define REGION_4  4
#define REGION_5  5


//*****************************************************************************
// Because the size of the arrows and clock characters are scaled based on
// CLOCK_TEXT_SIZE, the macros below perform all of the necessary scaling
// calculations at compile time.

// These macros specify the position of the 8 clock characters (HH:MM:SS)
#define COLUMN_0 CURSOR_START_WIDTH                         // Left edge
#define COLUMN_1 CURSOR_START_WIDTH + (TEXT_BLOCK_WIDTH)    // 1 column over
#define COLUMN_2 CURSOR_START_WIDTH + (TEXT_BLOCK_WIDTH*2)	// 2 columns over
#define COLUMN_3 CURSOR_START_WIDTH + (TEXT_BLOCK_WIDTH*3)	// 3 columns over
#define COLUMN_4 CURSOR_START_WIDTH + (TEXT_BLOCK_WIDTH*4)	// 4 columns over
#define COLUMN_5 CURSOR_START_WIDTH + (TEXT_BLOCK_WIDTH*5)	// 5 columns over
#define COLUMN_6 CURSOR_START_WIDTH + (TEXT_BLOCK_WIDTH*6)	// 6 columns over
#define COLUMN_7 CURSOR_START_WIDTH + (TEXT_BLOCK_WIDTH*7)	// 7 columns over
#define COLUMN_8 CURSOR_START_WIDTH + (TEXT_BLOCK_WIDTH*8)	// 8 columns over

// Unscaled Values based on screen size
#define BOX_HEIGHT  display_height() / 5
#define BOX_WIDTH   display_width() / 5

// Screen Position Macros
#define FOURTH(X)         ((X)/4) // Divide the given number by 4
#define HALF(X)           ((X)/2) // Divide the given number by 2
#define THREE_FOURTHS(X)  (((X) * 3)/4) // Multiply by 3, then divide by 4
//*************** End Scaling Calculation Macros ******************************



/**
 * Initializes the clock display. Should only be called once.
 */
void clockDisplay_init();

/**
 * Updates the time display with the latest time.
 * @param forceUpdateAll If true, refreshes ALL characters rather than just
 *                       the ones that changed.
 */
void clockDisplay_updateTimeDisplay(bool forceUpdateAll);

/**
 * Performs and Increment or decrement depending on which region of the
 * touchscreen is being pressed.
 */
void clockDisplay_performIncDec();

/**
 * Advances the time forward by 1 second.
 */
void clockDisplay_advanceTimeOneSecond();

/**
 * Runs a test that allows a user to visually verify correct clock-display
 * functionality. This function performs the following operations:
 * 	1. Increments Hours
 * 	2. Decrements Hours
 * 	3. Increments Minutes
 * 	4. Decrements Minutes
 * 	5. Increments Seconds
 * 	6. Decrements Seconds
 * 	7. Show the clock keeping time at a a 10x rate for 10 real-world seconds
 */
void clockDisplay_runTest();
