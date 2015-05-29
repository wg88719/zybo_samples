//*****************************************************************************
// Luke Hsiao
// 20 May 2015
// Interface of the SM to verify that the sequence of user touches
// matches the computer-generated sequence.
//*****************************************************************************

#ifndef VERIFYSEQUENCE_H_
#define VERIFYSEQUENCE_H_

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
