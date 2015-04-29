//*****************************************************************************
// Luke Hsiao
// 29 April 2015
// Driver file for interacting with the buttons
//*****************************************************************************

#include <stdio.h>
#include "buttons.h"
#include "supportFiles/leds.h"
#include "xparameters.h"
#include "supportFiles/display.h"

#define DEBUG  // Comment this line to remove the rest code from the executable

// Helper function to read GPIO registers.
int32_t buttons_readGpioRegister(int32_t offset) {
  // Note that you have to include a cast (uint32_t *) to keep the compiler happy.
  uint32_t *ptr = (uint32_t *) XPAR_GPIO_PUSH_BUTTONS_BASEADDR  + offset;
  return *ptr;
}

// Helper function to write GPIO registers.
void buttons_writeGpioRegister(int32_t offset, uint32_t value) {
  // Note that you have to include a cast (uint32_t *) to keep the compiler happy.
  uint32_t *ptr = (uint32_t *) XPAR_GPIO_PUSH_BUTTONS_BASEADDR  + offset;
  *ptr = value;
}

// Initialize the buttons by setting them as inputs
int buttons_init() {
  int status = BUTTONS_INIT_STATUS_FAIL;
  //write 1s to the tri-state driver to set buttons as input
  buttons_writeGpioRegister(TRISTATE_OFFSET, TRISTATE_SET_INPUT);
  status = BUTTONS_INIT_STATUS_OK;
  return status;
}

// Read the value of the buttons
int32_t buttons_read() {
  int32_t registerValue = buttons_readGpioRegister(0);

  // Zero out all values but the bottom 4
  registerValue = registerValue & 0xF;

  return registerValue;
}


#ifdef DEBUG

// Writes data to the LCD based on what button is pressed, blanks the screen otherwise
void buttons_write_LCD(int32_t buttons) {

  // Screen size values
  uint16_t x_max = display_width();
  uint16_t y_max = display_height();

  // Values of screen positions
  uint16_t x_fourth = x_max/4;
  uint16_t x_half = x_max/2;
  uint16_t x_three_fourths = (x_max * 3) / 4;
  uint16_t y_fourth = y_max/4;
  uint16_t y_half = y_max/2;
  uint16_t y_three_fourths = (y_max * 3) / 4;

  // If BTN 0 is pressed
  if (buttons & BTN0_MASK) {
    display_fillRect(0, 0, x_fourth, y_fourth, DISPLAY_RED);
    display_setCursor(x_fourth, y_fourth); // top left corner
    display_println("  BUTTON 0");
  }
  else {
    // Blank the used portion of the screen
    display_fillRect(0, 0, x_half, y_half, DISPLAY_BLACK);
  }

  // If BTN 1 is pressed
  if (buttons & BTN1_MASK) {
    display_fillRect(x_half, 0, x_fourth, y_fourth, DISPLAY_GREEN);
    display_setCursor(x_three_fourths, y_fourth); // top right corner
    display_println("  BUTTON 1");
  }
  else {
    // Blank the used portion of the screen
    display_fillRect(x_half, 0, x_half, y_half, DISPLAY_BLACK);
  }

  // If BTN 2 is pressed
  if (buttons & BTN2_MASK) {
    display_fillRect(0, y_half, x_fourth, y_fourth, DISPLAY_YELLOW);
    display_setCursor(x_fourth, y_three_fourths); // top right corner
    display_println("  BUTTON 2");
  }
  else {
    // Blank the used portion of the screen
    display_fillRect(0, y_half, x_half, y_half, DISPLAY_BLACK);
  }

  // If BTN 3 is pressed
  if (buttons & BTN3_MASK) {
    display_fillRect(x_half, y_half, x_fourth, y_fourth, DISPLAY_WHITE);
    display_setCursor(x_three_fourths, y_three_fourths); // top right corner
    display_println("  BUTTON 3");
  }
  else {
    // Blank the used portion of the screen
    display_fillRect(x_half, y_half, x_half, y_half, DISPLAY_BLACK);
  }
}

void buttons_runTest() {
  display_setRotation(0);
  display_fillScreen(DISPLAY_BLACK); // blank the screen

  int32_t buttonValues = buttons_read();

  while (buttonValues != ALL_BTNS_ON) {
    buttons_write_LCD(buttonValues);

    // Pull new values of buttons
    buttonValues = buttons_read();
  }

  display_fillScreen(DISPLAY_BLACK);
  display_setCursor(0,0);
  display_println("Button Test Finished!");
}

#endif
