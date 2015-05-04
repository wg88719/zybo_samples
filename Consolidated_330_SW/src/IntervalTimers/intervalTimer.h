//*****************************************************************************
// Luke Hsiao
// 1 May 2015
// Interface for AXI Interval Timers
//*****************************************************************************
#ifndef INTERVALTIMER_H_
#define INTERVALTIMER_H_

#include <stdint.h>
#include <stdlib.h>

// Timer Identifying Constants
#define TIMER0 0
#define TIMER1 1
#define TIMER2 2

// Status code to return for ERRORs
#define TIMER_ERROR 0xFFFFFFFF

// Register Offsets
#define TCSR0_OFFSET  0x00
#define TLR0_OFFSET   0x04
#define TCR0_OFFSET   0x08
#define TCSR1_OFFSET  0x10
#define TLR1_OFFSET   0x14
#define TCR1_OFFSET   0x18

// Control Bit Indexes
#define ENABLE_ENT0_MASK  0x00000080
#define CLEAR_ENT0_MASK   0xFFFFFF7F
#define ENABLE_LOAD0_MASK 0x00000020
#define ENABLE_CASC_MASK  0x00000800

// Width of each register in the timer
#define REGISTER_WIDTH 32

// Starts the specified timer
// @param timerNumber must be {0, 1, 2}, otherwise and error occurs
// @return 0 on success, or TIMER_ERROR
uint32_t intervalTimer_start(uint32_t timerNumber);

// Stops the specified timer
// @param timerNumber must be {0, 1, 2}, otherwise and error occurs
// @return 0 on success, or TIMER_ERROR
uint32_t intervalTimer_stop(uint32_t timerNumber);

// Sets the value of the cascaded counter to 0 and initializes the timer
// @param timerNumber must be {0, 1, 2}, otherwise and error occurs
uint32_t intervalTimer_reset(uint32_t timerNumber);

// Clears all control bits, sets timer to cascade and count up.
// @param timerNumber must be {0, 1, 2}, otherwise and error occurs
uint32_t intervalTimer_init(uint32_t timerNumber);

// Initializes all three hardware timers by calling intervalTimer_init() on each
uint32_t intervalTimer_initAll();

// Resets all three hardware timers by calling intervalTimer_reset() on eeach
uint32_t intervalTimer_resetAll();

// Tests all three hardware timers by calling intervalTimer_runTest() on each
uint32_t intervalTimer_testAll();


// Tests the specified timer by:
//  1. Reseting the counter and verifying it was reset.
//  2. Starting the counter and verifying that values change.
//  3. Stopping the counter and varifying that values do NOT change.
//
// @param timerNumber must be {0, 1, 2}, otherwise and error occurs
uint32_t intervalTimer_runTest(uint32_t timerNumber);


// Returns the number of seconds that have transpired since the counter was
// last reset and started.
//
// @param timerNumber must be {0, 1, 2}, otherwise and error occurs
// @param seconds Memory location pointed to by seconds will be modified to
//        return the number of seconds that have transpired since the counter
//        was last reset and started.
uint32_t intervalTimer_getTotalDurationInSeconds(uint32_t timerNumber, double *seconds);


#endif /* INTERVALTIMER_H_ */
