//*****************************************************************************
// Luke Hsiao
// 20 May 2015
// Implementation of global functions.
//*****************************************************************************

#include "globals.h"

static uint8_t globals_sequence[GLOBALS_MAX_FLASH_SEQUENCE];
static uint8_t globals_sequenceLength = 0;
static uint8_t globals_currentSequenceLength = 0;

void globals_setSequence(const uint8_t sequence[], uint16_t length) {
  for (int i = 0; i < length; i++)
  {
    globals_sequence[i] = sequence[i];
  }
  globals_sequenceLength = length;
}

uint8_t globals_getSequenceValue(uint16_t index) {
  return globals_sequence[index];
}

uint16_t globals_getSequenceLength() {
  return globals_sequenceLength;
}

void globals_setSequenceValue(uint16_t index, uint8_t value) {
  globals_sequence[index] = value;
}

void globals_setSequenceIterationLength(uint16_t length) {
  globals_currentSequenceLength = length;
}


uint16_t globals_getSequenceIterationLength() {
  return globals_currentSequenceLength;
}
