//*****************************************************************************
// Luke Hsiao
// 29 April 2015
// Interface for switches driver
//*****************************************************************************

#ifndef SWITCHES_H_
#define SWITCHES_H_

#include <stdint.h>
#include <stdlib.h>

#define SWITCHES_INIT_STATUS_OK 1
#define SWITCHES_INIT_STATUS_FAIL 0
#define TRISTATE_OFFSET 4
#define TRISTATE_SET_INPUT 0xF
#define SW0_MASK 0x1
#define SW1_MASK 0x2
#define SW2_MASK 0x4
#define SW3_MASK 0x8
#define ALL_SW_ON 0xF
#define BOTTOM_4_BITS 0xF

// Initializes the SWITCHES driver software and hardware. Returns one of the STATUS values defined above.
int switches_init();

// Returns the current value of all 4 SWITCHESs as the lower 4 bits of the returned value.
// bit3 = SW3, bit2 = SW2, bit1 = SW1, bit0 = SW0.
int32_t switches_read();

// Runs a test of the switches. As you slide the switches, LEDs directly above the switches will illuminate.
// The test will run until all switches are slid upwards. When all 4 slide switches are slid upward,
// this function will return.
void switches_runTest();

#endif /* SWITCHES_H_ */
