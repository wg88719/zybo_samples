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
#define INTERVALTIMER_TIMER0 0
#define INTERVALTIMER_TIMER1 1
#define INTERVALTIMER_TIMER2 2

// Status code to return for ERRORs
#define INTERVALTIMER_TIMER_ERROR 0xFFFFFFFF

// Register Offsets
#define INTERVALTIMER_TCSR0_OFFSET  0x00
#define INTERVALTIMER_TLR0_OFFSET   0x04
#define INTERVALTIMER_TCR0_OFFSET   0x08
#define INTERVALTIMER_TCSR1_OFFSET  0x10
#define INTERVALTIMER_TLR1_OFFSET   0x14
#define INTERVALTIMER_TCR1_OFFSET   0x18

// Control Bit Indexes
#define INTERVALTIMER_ENABLE_ENT0_MASK  0x00000080
#define INTERVALTIMER_CLEAR_ENT0_MASK   0xFFFFFF7F
#define INTERVALTIMER_ENABLE_LOAD0_MASK 0x00000020
#define INTERVALTIMER_ENABLE_CASC_MASK  0x00000800

// Width of each register in the timer
#define INTERVALTIMER_REGISTER_WIDTH 32

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
