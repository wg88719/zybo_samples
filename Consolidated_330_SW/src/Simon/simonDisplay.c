//*****************************************************************************
// Luke Hsiao
// 20 May 2015
// Implementation of the Simon display.
//*****************************************************************************

#include <stdio.h>
#include "globals.h"
#include "simonDisplay.h"
#include "buttons.h"
#include "supportFiles/display.h"
#include "supportFiles/utils.h"

int8_t simonDisplay_computeRegionNumber(int16_t x, int16_t y) {
  // Set the values of the sub box dimensions
  uint16_t x_max = display_width();
  uint16_t y_max = display_height();
  uint16_t box_width = GLOBALS_ONE_HALF(x_max);
  uint16_t box_height = GLOBALS_ONE_HALF(y_max);

  // Error condition for values off the screen
  if (x < 0 || y < 0 || x > x_max || y > y_max) {
    return SIMON_DISPLAY_ERROR;  // Invalid touch data
  }

  // Parse through top row
  if (y < box_height) {
    // Check Region 0
    if (x < box_width) {
      return SIMON_DISPLAY_REGION_0;
    }
    // Check Region 1
    else {
      return SIMON_DISPLAY_REGION_1;
    }
  }

  // Parse through bottom row
  else {
    // Check Region 2
    if (x < box_width) {
      return SIMON_DISPLAY_REGION_2;
    }
    // Check Region 3
    else {
      return SIMON_DISPLAY_REGION_3;
    }
  }
}

void simonDisplay_drawButton(uint8_t regionNumber) {
  // Set the values of the sub box dimensions
  uint16_t x_max = display_width();
  uint16_t y_max = display_height();

  // Compute dimensions of the 4 sub regions
  uint16_t box_width = GLOBALS_ONE_HALF(x_max);
  uint16_t box_height = GLOBALS_ONE_HALF(y_max);

  // Compute dimensions of half the button sizes for convenience in drawing
  uint16_t button_half_width = GLOBALS_ONE_HALF(SIMON_DISPLAY_BUTTON_WIDTH);
  uint16_t button_half_height = GLOBALS_ONE_HALF(SIMON_DISPLAY_BUTTON_HEIGHT);

  // Variables to be used in computing the centerpoint of each subregion
  uint16_t origin_x, origin_y;

  // The dimensions of the button is define by the header file
  switch (regionNumber) {
    case SIMON_DISPLAY_REGION_0:
      // Compute the origin point of the box
      origin_x = GLOBALS_ONE_HALF(box_width) - button_half_width;
      origin_y = GLOBALS_ONE_HALF(box_height) - button_half_height;

      // Draw the RED square
      display_fillRect( origin_x,                           // x0
                        origin_y,                           // y0
                        SIMON_DISPLAY_BUTTON_WIDTH,         // width
                        SIMON_DISPLAY_BUTTON_HEIGHT,        // height
                        DISPLAY_RED                         // color
                      );
      break;
    case SIMON_DISPLAY_REGION_1:
      // Compute the origin point of the box
      origin_x = box_width;
      origin_y = 0;
      origin_x = GLOBALS_ONE_HALF(box_width) - button_half_width + origin_x;
      origin_y = GLOBALS_ONE_HALF(box_height) - button_half_height + origin_y;

      // Draw the YELLOW square
      display_fillRect( origin_x,                           // x0
                        origin_y,                           // y0
                        SIMON_DISPLAY_BUTTON_WIDTH,         // width
                        SIMON_DISPLAY_BUTTON_HEIGHT,        // height
                        DISPLAY_YELLOW                      // color
                      );
      break;
    case SIMON_DISPLAY_REGION_2:
      // Compute the origin point of the box
      origin_x = 0;
      origin_y = box_height;
      origin_x = GLOBALS_ONE_HALF(box_width) - button_half_width + origin_x;
      origin_y = GLOBALS_ONE_HALF(box_height) - button_half_height + origin_y;

      // Draw the BLUE square
      display_fillRect( origin_x,                           // x0
                        origin_y,                           // y0
                        SIMON_DISPLAY_BUTTON_WIDTH,         // width
                        SIMON_DISPLAY_BUTTON_HEIGHT,        // height
                        DISPLAY_BLUE                        // color
                      );
      break;
    case SIMON_DISPLAY_REGION_3:
      // Compute the origin point of the box
      origin_x = box_width;
      origin_y = box_height;
      origin_x = GLOBALS_ONE_HALF(box_width) - button_half_width + origin_x;
      origin_y = GLOBALS_ONE_HALF(box_height) - button_half_height + origin_y;

      // Draw the GREEN square
      display_fillRect( origin_x,                           // x0
                        origin_y,                           // y0
                        SIMON_DISPLAY_BUTTON_WIDTH,         // width
                        SIMON_DISPLAY_BUTTON_HEIGHT,        // height
                        DISPLAY_GREEN                       // color
                      );
        break;
    default:
      printf("Draw button default case hit\n"); // shouldn't hit this
      break;
  }
}

void simonDisplay_drawAllButtons() {
  // Iterate over all 4 regions and draw the buttons.
  int i;
  for (i = 0; i <= SIMON_DISPLAY_REGION_3; i++) {
    simonDisplay_drawButton(i);
  }
}

void simonDisplay_drawSquare(uint8_t regionNo, bool erase) {
  // Set the values of the sub box dimensions
  uint16_t x_max = display_width();
  uint16_t y_max = display_height();

  // Compute dimensions of the 4 sub regions
  uint16_t box_width = GLOBALS_ONE_HALF(x_max);
  uint16_t box_height = GLOBALS_ONE_HALF(y_max);

  // Variables to be used in computing the centerpoint of each subregion
  uint16_t origin_x, origin_y;

  // The dimensions of the button is define by the header file
  switch (regionNo) {
    case SIMON_DISPLAY_REGION_0:
      // Compute the origin point of the box
      origin_x = 0;
      origin_y = 0;

      if (erase) {
        // Draw BLACK to erase the square
        display_fillRect( origin_x,          // x0
                          origin_y,          // y0
                          box_width,         // width
                          box_height,        // height
                          DISPLAY_BLACK      // color
                        );
      }
      else {
        // Draw the RED square
        display_fillRect( origin_x,          // x0
                          origin_y,          // y0
                          box_width,         // width
                          box_height,        // height
                          DISPLAY_RED        // color
                        );
      }
      break;
    case SIMON_DISPLAY_REGION_1:
      // Compute the origin point of the box
      origin_x = box_width;
      origin_y = 0;

      if (erase) {
        // Draw BLACK to erase the square
        display_fillRect( origin_x,          // x0
                          origin_y,          // y0
                          box_width,         // width
                          box_height,        // height
                          DISPLAY_BLACK      // color
                        );
      }
      else {
        // Draw the YELLOW square
        display_fillRect( origin_x,          // x0
                          origin_y,          // y0
                          box_width,         // width
                          box_height,        // height
                          DISPLAY_YELLOW     // color
                        );
      }
      break;
    case SIMON_DISPLAY_REGION_2:
      // Compute the origin point of the box
      origin_x = 0;
      origin_y = box_height;

      if (erase) {
        // Draw BLACK to erase the square
        display_fillRect( origin_x,          // x0
                          origin_y,          // y0
                          box_width,         // width
                          box_height,        // height
                          DISPLAY_BLACK      // color
                        );
      }
      else {
        // Draw the BLUE square
        display_fillRect( origin_x,          // x0
                          origin_y,          // y0
                          box_width,         // width
                          box_height,        // height
                          DISPLAY_BLUE       // color
                        );
      }
      break;
    case SIMON_DISPLAY_REGION_3:
      // Compute the origin point of the box
      origin_x = box_width;
      origin_y = box_height;

      if (erase) {
        // Draw BLACK to erase the square
        display_fillRect( origin_x,          // x0
                          origin_y,          // y0
                          box_width,         // width
                          box_height,        // height
                          DISPLAY_BLACK      // color
                        );
      }
      else {
        // Draw the GREEN square
        display_fillRect( origin_x,          // x0
                          origin_y,          // y0
                          box_width,         // width
                          box_height,        // height
                          DISPLAY_GREEN      // color
                        );
      }
      break;
    default:
      printf("Draw square default case hit\n"); // shouldn't hit this
      break;
  }
}

#define TOUCH_PANEL_ANALOG_PROCESSING_DELAY_IN_MS 60 // in ms
#define MAX_STR 255
#define TEXT_SIZE 2
// Runs a brief demonstration of how buttons can be pressed and squares lit up to implement the user
// interface of the Simon game. The routine will continue to run until the touchCount has been reached, e.g.,
// the user has touched the pad touchCount times.

// I used a busy-wait delay (utils_msDelay) that uses a for-loop and just blocks until the time has passed.
// When you implement the game, you CANNOT use this function as we discussed in class. Implement the delay
// using the non-blocking state-machine approach discussed in class.
void simonDisplay_runTest(uint16_t touchCount) {
  display_init();  // Always initialize the display.
  char str[MAX_STR];   // Enough for some simple printing.
  uint8_t regionNumber;
  uint16_t touches = 0;
  // Write an informational message and wait for the user to touch the LCD.
  display_fillScreen(DISPLAY_BLACK);        // clear the screen.
  display_setCursor(0, display_height()/2); //
  display_setTextSize(TEXT_SIZE);
  display_setTextColor(DISPLAY_RED, DISPLAY_BLACK);
  sprintf(str, "Touch and release to start the Simon demo.");
  display_println(str);
  display_println();
  sprintf(str, "Demo will terminate after %d touches.", touchCount);
  display_println(str);
  while (!display_isTouched());       // Wait here until the screen is touched.
  while (display_isTouched());        // Now wait until the touch is released.
  display_fillScreen(DISPLAY_BLACK);  // Clear the screen.
  simonDisplay_drawAllButtons();      // Draw all of the buttons.
  bool touched = false;  	      // Keep track of when the pad is touched.
  int16_t x, y;  		      // Use these to keep track of coordinates.
  uint8_t z;      		      // This is the relative touch pressure.
  while (touches < touchCount) {  // Run the loop according to the number of touches passed in.
    if (!display_isTouched() && touched) {         // user has stopped touching the pad.
      simonDisplay_drawSquare(regionNumber, true); // Erase the square.
      simonDisplay_drawButton(regionNumber);	   // DISPLAY_REDraw the button.
      touched = false;															// Released the touch, set touched to false.
    } else if (display_isTouched() && !touched) {   // User started touching the pad.
      touched = true;                               // Just touched the pad, set touched = true.
      touches++;  																	// Keep track of the number of touches.
      display_clearOldTouchData();  // Get rid of data from previous touches.
      // Must wait this many milliseconds for the chip to do analog processing.
      utils_msDelay(TOUCH_PANEL_ANALOG_PROCESSING_DELAY_IN_MS);
      display_getTouchedPoint(&x, &y, &z);                  // After the wait, get the touched point.
      regionNumber = simonDisplay_computeRegionNumber(x, y);// Compute the region number.
      simonDisplay_drawSquare(regionNumber, false);	    // Draw the square (erase = false).
    }
  }
  // Done with the demo, write an informational message to the user.
  display_fillScreen(DISPLAY_BLACK);        // clear the screen.
  display_setCursor(0, display_height()/2); // Place the cursor in the middle of the screen.
  display_setTextSize(2);                   // Make it readable.
  display_setTextColor(DISPLAY_RED, DISPLAY_BLACK);  // red is foreground color, black is background color.
  sprintf(str, "Simon demo terminated");    // Format a string using sprintf.
  display_println(str);                     // Print it to the LCD.
  sprintf(str, "after %d touches.", touchCount);  // Format the rest of the string.
  display_println(str);  // Print it to the LCD.
}
