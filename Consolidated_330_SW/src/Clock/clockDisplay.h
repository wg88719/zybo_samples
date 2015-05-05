//*****************************************************************************
// Luke Hsiao
// 4 May 2015
// Interface for displaying the clock
//*****************************************************************************

#include <stdbool.h>

// Edit this to change the size of the clock chars
// Only set this value to 3, 4, 5, or 6.
#define CLOCK_TEXT_SIZE 5

/**
 * Initializes the clock display. Should only be called once.
 */
void clockDisplay_init();

/**
 * Updates the time display with the latest time.
 * @param forceUpdateAll [description]
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
