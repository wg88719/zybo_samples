//*****************************************************************************
// Luke Hsiao
// 29 April 2015
// Driver file for interacting with the switches
//*****************************************************************************

#include <stdio.h>
#include <xil_io.h>
#include "switches.h"
#include "xparameters.h"
#include "supportFiles/leds.h"

int switches_init() {
  int status = SWITCHES_INIT_STATUS_FAIL; // Initialize as FAIL

  //write 1s to the tri-state driver to set buttons as input
  Xil_Out32(XPAR_GPIO_SLIDE_SWITCHES_BASEADDR + TRISTATE_OFFSET, // address
            TRISTATE_SET_AS_INPUT);  // value to write

  status = SWITCHES_INIT_STATUS_OK; // Set to OK
  return status;
}

int32_t switches_read() {
  //read the value of the swithces
  u32 switchValues = Xil_In32(XPAR_GPIO_SLIDE_SWITCHES_BASEADDR + VALUE_OFFSET);

  // Zero out all bits except relevant switch values
  switchValues = switchValues & BOTTOM_4_BITS;

  //cast switches value to an int32_t and return
  return (int32_t)switchValues;
}

void switches_runTest() {
  // Do an initial read of switch values
  int32_t sw_value = switches_read();

  // Run the test while all 4 switches are not all on simultaneously
  while (sw_value != ALL_SW_ON) {
    // Write the switch values to their corresponding LEDs
    leds_write(sw_value);

    // Poll for a new value of the switches
    sw_value = switches_read();
  }

  // Turn of LEDs to signal completion when all switches are on
  leds_write(0);  // Turn off all LEDS
}
