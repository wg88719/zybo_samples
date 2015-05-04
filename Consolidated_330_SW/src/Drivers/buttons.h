//*****************************************************************************
// Luke Hsiao
// 29 April 2015
// Interface for button driver
//*****************************************************************************

#ifndef BUTTONS_H_
#define BUTTONS_H_

#include <stdint.h>
#include <stdlib.h>

// Status Values
#define BUTTONS_INIT_STATUS_OK 1
#define BUTTONS_INIT_STATUS_FAIL 0
#define SET 1
#define NOT_SET 0

// Values for working with GPIO Tristate
#define VALUE_OFFSET 0
#define TRISTATE_OFFSET 4
#define TRISTATE_SET_AS_INPUT 0xF

// Button Mask Values
#define BTN0_MASK       0x1
#define BTN1_MASK       0x2
#define BTN2_MASK       0x4
#define BTN3_MASK       0x8
#define ALL_BTNS_ON     0xF
#define BOTTOM_4_BITS   0xF

// Initializes the button driver software and hardware.
// Returns one of the defined status values:
//    - BUTTONS_INIT_STATUS_OK
//    - BUTTONS_INIT_STATUS_FAIL
int buttons_init();

// Returns the current value of all 4 buttons as the lower 4 bits of the
// returned value.
// bit3 = BTN3, bit2 = BTN2, bit1 = BTN1, bit0 = BTN0.
int32_t buttons_read();

// Runs a test of the buttons.
// As you push the buttons, graphics and messages will be written to the LCD
// panel. The test will until all 4 pushbuttons are simultaneously pressed.
void buttons_runTest();

#endif /* BUTTONS_H_ */
