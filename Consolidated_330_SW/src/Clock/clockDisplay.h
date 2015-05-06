//*****************************************************************************
// Luke Hsiao
// 4 May 2015
// Interface for displaying the clock
//
// Details:
//    Note that the area of the touchscreen will be displayed and scaled usering
//    these regions:
// +--+---+---+---+---+---+---+---+---+--+
// |  |   |   |   |   |   |   |   |   |  |
// +--+===+===+===+===+===+===+===+===+--+
// |  |   ▲   |   |   ▲   |   |   ▲   |  |
// +--+---+---+---+---+---+---+---+---+--+
// |  | H | H | : | M | M | : | S | S |  |
// +--+---+---+---+---+---+---+---+---+--+
// |  |   ▼   |   |   ▼   |   |   ▼   |  |
// +--+===+===+===+===+===+===+===+===+--+
// |  |   |   |   |   |   |   |   |   |  |
// +--+---+---+---+---+---+---+---+---+--+
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

// Constants for calculating time rollovers
#define MAX_SECS  59
#define MAX_MINS  59
#define MAX_HRS   12  // Since we're displaying a 12-hr clock

//*****************************************************************************
// Because the size of the arrows and clock characters are scaled based on
// CLOCK_TEXT_SIZE, the macros below perform all of the necessary scaling
// calculations at compile time.

#define CLOCK_TEXT_MAX 6  // The maximum value CLOCK_TEXT_SIZE may take

// Scaled Values based on screen size. In other words, the largest the clock
// will be drawn is as if the full screen was available.
// For example: if CLOCK_TEXT_SIZE = 6, then
//    OUTER_BOX_HEIGHT = display_height() * 6 / 6 = display_height()
// which represents the largest value OUTER_BOX_HEIGHT will take.

// OUTER_BOX_HEIGHT and OUTER_BOX_WIDTH represent the dimensions of the box to
// draw the clock in.
#define OUTER_BOX_HEIGHT ((display_height()*CLOCK_TEXT_SIZE)/(CLOCK_TEXT_MAX))
#define OUTER_BOX_WIDTH  ((display_width()*CLOCK_TEXT_SIZE)/(CLOCK_TEXT_MAX))

// These marginal values represent the dimenion to leave blank on each edge
// when drawing the clock.
#define HEIGHT_MARGIN ((display_height() - (OUTER_BOX_HEIGHT)) / 2)
#define WIDTH_MARGIN  ((display_width() - (OUTER_BOX_WIDTH)) / 2)

// BUFFER represents the spacing between the drawn characters/arrows and the
// edges of their designated boxes.
#define BUFFER  (CLOCK_TEXT_SIZE * 2)

// ORIGIN values represent the top-left corner of the scaled box. Arrows and
// clock characters will be drawn with respect to this origin.
#define ORIGIN_X  (WIDTH_MARGIN)
#define ORIGIN_Y  (HEIGHT_MARGIN)

// These macros define the dimension of the sub-boxes in which arrows and
// characters will be drawn. The OUTER_BOX will be segmented 3x3 into rectangles
#define SUB_BOX_HEIGHT  ((OUTER_BOX_HEIGHT) / 3)
#define SUB_BOX_WIDTH   ((OUTER_BOX_WIDTH) / 8)
#define ARROW_HEIGHT    ((SUB_BOX_HEIGHT) / 2)


//*************** End Scaling Calculation Macros ******************************

// These macros specify the position of the 8 clock characters (HH:MM:SS)
#define COLUMN_0 (ORIGIN_X)                     // Left edge
#define COLUMN_1 (ORIGIN_X + (SUB_BOX_WIDTH))   // 1 column over
#define COLUMN_2 (ORIGIN_X + (SUB_BOX_WIDTH*2))	// 2 columns over
#define COLUMN_3 (ORIGIN_X + (SUB_BOX_WIDTH*3))	// 3 columns over
#define COLUMN_4 (ORIGIN_X + (SUB_BOX_WIDTH*4))	// 4 columns over
#define COLUMN_5 (ORIGIN_X + (SUB_BOX_WIDTH*5))	// 5 columns over
#define COLUMN_6 (ORIGIN_X + (SUB_BOX_WIDTH*6))	// 6 columns over
#define COLUMN_7 (ORIGIN_X + (SUB_BOX_WIDTH*7))	// 7 columns over
#define COLUMN_8 (ORIGIN_X + (SUB_BOX_WIDTH*8))	// 8 columns over

// These macros specify the starting position of each row's y-value
#define ROW_0 (ORIGIN_Y)                            // Top of the outer box
#define ROW_1 (ORIGIN_Y + SUB_BOX_HEIGHT)           // 1 row down
#define ROW_2 (ORIGIN_Y + (SUB_BOX_HEIGHT*2))       // 2 rows down
#define ROW_3 (ORIGIN_Y + (SUB_BOX_HEIGHT*3))       // 3 rows down

#define NUM_CHARS 9   // 8 chars for HH:MM:SS + 1 for NULL terminator

/**
 * Initializes the clock display. Should only be called once.
 */
void clockDisplay_init();

/**
 * Updates the time display with the latest time.
 * @param forceUpdateAll If true, refreshes ALL characters rather than just
 *                       the ones that are being updated.
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
