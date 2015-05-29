//*****************************************************************************
// Luke Hsiao
// 20 May 2015
// Interface of global functions, macros, and constants used between SMs.
//*****************************************************************************

#ifndef GLOBALS_H_
#define GLOBALS_H_

#include <stdbool.h>
#include <stdint.h>

// large so it could be used for a splash screen.
#define GLOBALS_MAX_FLASH_SEQUENCE 1000


// Screen Position Macros
#define GLOBALS_ONE_THIRD(X)    ((X) / 3)  // Divide the given number by 4
#define GLOBALS_ONE_HALF(X)     ((X) / 2)  // Divide the given number by 2

/**
 * This is the length of the complete sequence at maximum length.
 * The contents of sequence array is copied into the global variable.
 * @param sequence The value to copy into the global sequence variable.
 * @param length   The length to set the complete sequence to.
 */
void globals_setSequence(const uint8_t sequence[], uint16_t length);

/**
 * Gets the value of the sequence at the given index.
 * @param  index The index of the sequence to query.
 * @return       The value of the sequence at index.
 */
uint8_t globals_getSequenceValue(uint16_t index);

/**
 * Returns the current length of the sequence.
 * @return The length of the current sequence.
 */
uint16_t globals_getSequenceLength();

/**
 * Sets the length of the sequence that is currently being worked on.
 * @param length The value to set the length to.
 */
void globals_setSequenceIterationLength(uint16_t length);

/**
 * Sets the length of the sequence of the total sequence
 * @param length The value to set the length to.
 */
void globals_setTotalSequenceLength(uint16_t length);

/**
 * Length of the sequence that you are currently working on.
 * @return [description]
 */
uint16_t globals_getSequenceIterationLength();

#endif /* GLOBALS_H_ */
