//*****************************************************************************
// Luke Hsiao
// 20 May 2015
// Interface of the SM to flash the sequence the user must mimic.
//*****************************************************************************

#ifndef FLASHSEQUENCE_H_
#define FLASHSEQUENCE_H_

#include <stdbool.h>
#include <stdint.h>

#define TEST_SEQUENCE_LENGTH 8  // Just use a short test sequence.

#define INCREMENTING_SEQUENCE_MESSAGE1 "Incrementing Sequence"  // Info message.
#define RUN_TEST_COMPLETE_MESSAGE "Runtest() Complete"      // Info message.
#define MESSAGE_TEXT_SIZE 2                                   // Make the text easy to see.

#define FLASH_WAIT 500 // ticks to display each sequence value

/**
 * Turns on the state machine. Part of the interlock.
 */
void flashSequence_enable();

/**
 * Turns off the state machine. Part of the interlock.
 */
void flashSequence_disable();

/**
 * Other state machines can call this to determine whether the flash sequence
 * has finished or not.
 * @return TRUE = flash sequence is finished, FALSE = otherwise.
 */
bool flashSequence_completed();

/**
 * Tick function controlling the sequence of flashes.
 */
void flashSequence_tick();

/**
 * Tests the flash sequence state machine.
 */
void flashSequence_runTest();

#endif /* FLASHSEQUENCE_H_ */
