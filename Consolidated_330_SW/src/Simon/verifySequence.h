//*****************************************************************************
// Luke Hsiao
// 20 May 2015
// Interface of the SM to verify that the sequence of user touches
// matches the computer-generated sequence.
//*****************************************************************************

#ifndef VERIFYSEQUENCE_H_
#define VERIFYSEQUENCE_H_

#define BTN0 1
// This will set the sequence to a simple sequential pattern.
#define MAX_TEST_SEQUENCE_LENGTH 4  // the maximum length of the pattern
#define MESSAGE_WAIT_MS 4000  // Display messages for this long.
#define MESSAGE_X 0
#define MESSAGE_Y (display_width()/4)
#define MESSAGE_TEXT_SIZE 2
#define MESSAGE_STARTING_OVER

#define WAIT_TIMEOUT 3000 // ticks to wait before signaling a TIMEOUT

/**
 * Enables this state machine.
 */
void verifySequence_enable();

/**
 * Disables the state machine. This is used as part of the interlock. This SM
 * can be disabled and then reenabled.
 */
void verifySequence_disable();

/**
 * Used to detect if there has been a time-out error.
 * @return TRUE = time-out has occurred. FALSE otherwise.
 */
bool verifySequence_isTimeOutError();

/**
 * Used to detect if the user tapped the incorrect sequence.
 * @return TRUE = User input is incorrect. FALSE = otherwise.
 */
bool verifySequence_isUserInputError();

/**
 * Used to detect if the verifySequence SM has finished verifying.
 * @return TRUE = sequence verification complete. FALSE = otherwise.
 */
bool verifySequence_isComplete();

/**
 * Tick function that verifies the sequence.
 */
void verifySequence_tick();

/**
 * Test function that verifies the correctness of the verifySequence SM.
 */
void verifySequence_runTest();

#endif /* VERIFYSEQUENCE_H_ */
