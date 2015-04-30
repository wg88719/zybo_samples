//*****************************************************************************
// Luke Hsiao
// 29 April 2015
// Driver file for interacting with the buttons
//*****************************************************************************

#include <stdio.h>
#include "buttons.h"
#include "xparameters.h"
#include "supportFiles/display.h"

//************************* Helper Functions **********************************

// Helper function to read GPIO registers.
// @param offset offset from the base address to read from
int32_t buttons_readGpioRegister(int32_t offset) {
  // Casted to (uint32_t *) to match data types
  uint32_t *ptr = (uint32_t *)XPAR_GPIO_PUSH_BUTTONS_BASEADDR + offset;
  return *ptr;
}

// Helper function to write GPIO registers.
// @param offset offset from the base address to write to
// @param value 32-bit value to write to the register
void buttons_writeGpioRegister(int32_t offset, uint32_t value) {
  // Casted to (uint32_t *) to match data types
  uint32_t *ptr = (uint32_t *)XPAR_GPIO_PUSH_BUTTONS_BASEADDR + offset;
  *ptr = value;
}

// Helper function to write data to the LCD based on what button is pressed, 
// blanks the screen otherwise.
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

//*********************** End Helper Functions ********************************

int buttons_init() {
  int status = BUTTONS_INIT_STATUS_FAIL;
  
  //write 1s to the tri-state driver to set buttons as input
  buttons_writeGpioRegister(TRISTATE_OFFSET, TRISTATE_SET_INPUT);
  
  status = BUTTONS_INIT_STATUS_OK;
  return status;
}

int32_t buttons_read() {
  int32_t registerValue = buttons_readGpioRegister(VALUE_OFFSET);

  // Zero out all values but the bottom 4
  registerValue = registerValue & BOTTOM_4_BITS;

  return registerValue;
}

void buttons_runTest() {
  display_setRotation(0); // set rotation to zero
  display_fillScreen(DISPLAY_BLACK); // blank the screen

  // Do an initial read of button values
  int32_t buttonValues = buttons_read();

  while (buttonValues != ALL_BTNS_ON) {
    buttons_write_LCD(buttonValues);

    // Poll new value of buttons
    buttonValues = buttons_read();
  }

  // After all buttons are pushed simultaneously, finish test.
  display_fillScreen(DISPLAY_BLACK);  // blank screen
  display_setCursor(0,0);  // reset cursor to origin
  display_println("Button Test Finished!");  // print that the test is complete
}
