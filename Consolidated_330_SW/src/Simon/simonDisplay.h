//*****************************************************************************
// Luke Hsiao
// 20 May 2015
// Interface of the Simon display.
//*****************************************************************************

#ifndef SIMONDISPLAY_H_
#define SIMONDISPLAY_H_

#include <stdbool.h>
#include <stdint.h>

// Width, height of the simon "buttons"
#define SIMON_DISPLAY_BUTTON_WIDTH  60
#define SIMON_DISPLAY_BUTTON_HEIGHT 60

// These are the definitions for the regions.
#define SIMON_DISPLAY_REGION_0  0
#define SIMON_DISPLAY_REGION_1  1
#define SIMON_DISPLAY_REGION_2  2
#define SIMON_DISPLAY_REGION_3  3

/**
 * Given coordinates from the touch pad, computes the region number.
 * The entire touch-screen is divided into 4 rectangular regions, numbered 0-3.
 * Each region is drawn with a different color. When a region is pressed, this
 * function returns the region nuber that is used by the other routines.
 * +----------+----------+
 * |          |          |
 * |    0     |     1    |
 * |  (RED)   | (YELLOW) |
 * +----------+----------+
 * |          |          |
 * |    2     |    3     |
 * |  (BLUE)  |  (GREEN) |
 * +----------+----------+
 * @param  x x-coordinate of the touched point.
 * @param  y y-coordinate of the touched point.
 * @return   The region number corresponding to the touched point.
 */
int8_t simonDisplay_computeRegionNumber(int16_t x, int16_t y);

/**
 * Draws a colored "button" that the user can touch.
 * The colored button is centered in the region, but doesn't fill the region.
 * @param regionNumber The region in which to draw the button.
 */
void simonDisplay_drawButton(uint8_t regionNumber);

/**
 * Convenience function that draws all of the buttons.
 */
void simonDisplay_drawAllButtons();

/**
 * Draws a bigger square that completely fills the region. If the erase argument
 * is true, it draws the square as a black background to "erase" it.
 * @param regionNo The number of the region to fill.
 * @param erase    TRUE = erases the square, FALSE = draws the colored square.
 */
void simonDisplay_drawSquare(uint8_t regionNo, bool erase);

/**
 * Runs a brief demonstration of how buttons can be pressed and squares lit up
 * to implement the UI of the Simon game. The routine continues to run until the
 * touchCound has been reached.
 * @param touchCount The number of touches before exiting the test.
 */
void simonDisplay_runTest(uint16_t touchCount);

#endif /* SIMONDISPLAY_H_ */
