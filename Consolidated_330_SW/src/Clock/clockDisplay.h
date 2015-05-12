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
// |  |  /\   |   |  /\   |   |   /\  |  |
// +--+---+---+---+---+---+---+---+---+--+
// |  | H | H | : | M | M | : | S | S |  |
// +--+---+---+---+---+---+---+---+---+--+
// |  |  \/   |   |  \/   |   |   \/  |  |
// +--+===+===+===+===+===+===+===+===+--+
// |  |   |   |   |   |   |   |   |   |  |
// +--+---+---+---+---+---+---+---+---+--+
//*****************************************************************************

#include <stdbool.h>

//*****************************************************************************
// User Editable Macros:

// Edit this to change the size of the clock chars. Only set this value to
// 3, 4, 5, or 6. Otherwise, the clock will NOT display correctly.
#define CLOCKDISPLAY_CLOCK_TEXT_SIZE 6

// Values to Initialize clock to on startup
#define CLOCKDISPLAY_INITIAL_HOURS   12
#define CLOCKDISPLAY_INITIAL_MINUTES 59
#define CLOCKDISPLAY_INITIAL_SECONDS 55
//***************** End User Editable Macros **********************************

// Region IDs for the touchscreen
#define CLOCKDISPLAY_REGION_0    0
#define CLOCKDISPLAY_REGION_1    1
#define CLOCKDISPLAY_REGION_2    2
#define CLOCKDISPLAY_REGION_3    3
#define CLOCKDISPLAY_REGION_4    4
#define CLOCKDISPLAY_REGION_5    5
#define CLOCKDISPLAY_REGION_ERR  -1

// Constants for calculating time rollovers
#define CLOCKDISPLAY_MAX_SECS  59
#define CLOCKDISPLAY_MAX_MINS  59
#define CLOCKDISPLAY_MAX_HRS   12  // Since we're displaying a 12-hr clock

//*****************************************************************************
// Because the size of the arrows and clock characters are scaled based on
// CLOCK_TEXT_SIZE, the macros below perform all of the necessary scaling
// calculations at compile time.

#define CLOCKDISPLAY_SCALE_TEXT 7  // Value used to scale the clock text

// OUTER_BOX_HEIGHT and OUTER_BOX_WIDTH represent the dimensions of the box to
// draw the clock in.
#define CLOCKDISPLAY_OUTER_BOX_HEIGHT ((display_height()*CLOCK_TEXT_SIZE)/(SCALE_TEXT))
#define CLOCKDISPLAY_OUTER_BOX_WIDTH  ((display_width()*CLOCK_TEXT_SIZE)/(SCALE_TEXT))

// These marginal values represent the dimension to leave blank on each edge
// when drawing the clock.
#define CLOCKDISPLAY_HEIGHT_MARGIN ((display_height() - (OUTER_BOX_HEIGHT)) / 2)
#define CLOCKDISPLAY_WIDTH_MARGIN  ((display_width() - (OUTER_BOX_WIDTH)) / 2)

// BUFFER represents the spacing between the drawn characters and the
// edges of their designated boxes.
#define CLOCKDISPLAY_BUFFER  (CLOCK_TEXT_SIZE * 2)

// ORIGIN values represent the top-left corner of the scaled box. Arrows and
// clock characters will be drawn with respect to this origin.
#define CLOCKDISPLAY_ORIGIN_X  (WIDTH_MARGIN)
#define CLOCKDISPLAY_ORIGIN_Y  (HEIGHT_MARGIN)

// These macros define the dimension of the sub-boxes in which arrows and
// characters will be drawn. The OUTER_BOX will be segmented 3x3 into rectangles
#define CLOCKDISPLAY_SUB_BOX_HEIGHT  ((OUTER_BOX_HEIGHT) / 3)
#define CLOCKDISPLAY_SUB_BOX_WIDTH   ((OUTER_BOX_WIDTH) / 8)
#define CLOCKDISPLAY_ARROW_HEIGHT    ((SUB_BOX_HEIGHT) / 2)


//*************** End Scaling Calculation Macros ******************************

// These macros specify the position of the 8 clock characters (HH:MM:SS)
#define CLOCKDISPLAY_COLUMN_0 (ORIGIN_X)                     // Left edge
#define CLOCKDISPLAY_COLUMN_1 (ORIGIN_X + (SUB_BOX_WIDTH))   // 1 column over
#define CLOCKDISPLAY_COLUMN_2 (ORIGIN_X + (SUB_BOX_WIDTH*2))	// 2 columns over
#define CLOCKDISPLAY_COLUMN_3 (ORIGIN_X + (SUB_BOX_WIDTH*3))	// 3 columns over
#define CLOCKDISPLAY_COLUMN_4 (ORIGIN_X + (SUB_BOX_WIDTH*4))	// 4 columns over
#define CLOCKDISPLAY_COLUMN_5 (ORIGIN_X + (SUB_BOX_WIDTH*5))	// 5 columns over
#define CLOCKDISPLAY_COLUMN_6 (ORIGIN_X + (SUB_BOX_WIDTH*6))	// 6 columns over
#define CLOCKDISPLAY_COLUMN_7 (ORIGIN_X + (SUB_BOX_WIDTH*7))	// 7 columns over
#define CLOCKDISPLAY_COLUMN_8 (ORIGIN_X + (SUB_BOX_WIDTH*8))	// 8 columns over

// These macros specify the starting position of each row's y-value
#define CLOCKDISPLAY_ROW_0 (ORIGIN_Y)                            // Top of the outer box
#define CLOCKDISPLAY_ROW_1 (ORIGIN_Y + SUB_BOX_HEIGHT)           // 1 row down
#define CLOCKDISPLAY_ROW_2 (ORIGIN_Y + (SUB_BOX_HEIGHT*2))       // 2 rows down
#define CLOCKDISPLAY_ROW_3 (ORIGIN_Y + (SUB_BOX_HEIGHT*3))       // 3 rows down

#define CLOCKDISPLAY_NUM_CHARS 9   // 8 chars for "HH:MM:SS" + 1 for NULL terminator

// Macros for names of the indexes in the char arrays
#define CLOCKDISPLAY_TENS_HRS  0
#define CLOCKDISPLAY_ONES_HRS  1
#define CLOCKDISPLAY_COLON_1   2
#define CLOCKDISPLAY_TENS_MINS 3
#define CLOCKDISPLAY_ONES_MINS 4
#define CLOCKDISPLAY_COLON_2   5
#define CLOCKDISPLAY_TENS_SECS 6
#define CLOCKDISPLAY_ONES_SECS 7

// Macros for making calls to updateTimeDisplay(bool X) more readable
#define CLOCKDISPLAY_UPDATE_ALL        true    // force update all
#define CLOCKDISPLAY_DO_NOT_UPDATE_ALL false   // do not force update all

// Time macros used in runTest() in units of milliseconds
#define CLOCKDISPLAY_TENTH_SECOND            100
#define CLOCKDISPLAY_HALF_SECOND             500
#define CLOCKDISPLAY_TENTHS_IN_TEN_SECONDS   100 // # 10ms in 10 seconds

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
