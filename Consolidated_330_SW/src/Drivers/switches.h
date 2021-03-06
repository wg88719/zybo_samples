//*****************************************************************************
// Luke Hsiao
// 29 April 2015
// Interface for switches driver
//*****************************************************************************

#ifndef SWITCHES_H_
#define SWITCHES_H_

#include <stdint.h>
#include <stdlib.h>

// Status values
#define SWITCHES_INIT_STATUS_OK 1
#define SWITCHES_INIT_STATUS_FAIL 0

// Constants for working with GPIO Tristate
#define SWITCHES_VALUE_OFFSET 0
#define SWITCHES_TRISTATE_OFFSET 4
#define SWITCHES_TRISTATE_SET_AS_INPUT 0xF  // value to turn tristate to input

// Mask Values for Switches
#define SWITCHES_SW0_MASK      0x1
#define SWITCHES_SW1_MASK      0x2
#define SWITCHES_SW2_MASK      0x4
#define SWITCHES_SW3_MASK      0x8
#define SWITCHES_ALL_SW_ON     0xF
#define SWITCHES_BOTTOM_4_BITS 0xF

// Initializes the SWITCHES driver software and hardware.
// Returns one of the defined status values:
//    - SWITCHES_INIT_STATUS_OK
//    - SWITCHES_INIT_STATUS_FAIL
int switches_init();

// Returns the current value of all 4 SWITCHESs as the lower 4 bits of the
// returned value.
// bit3 = SW3, bit2 = SW2, bit1 = SW1, bit0 = SW0.
int32_t switches_read();

// Runs a test of the switches. As you slide the switches, LEDs directly above
// the switches will illuminate. The test will run until all switches are slid
// upwards. When all 4 slide switches are slid upward,this function will return.
void switches_runTest();

#endif /* SWITCHES_H_ */
