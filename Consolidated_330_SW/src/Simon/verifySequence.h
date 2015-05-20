//*****************************************************************************
// Luke Hsiao
// 20 May 2015
// Interface of the SM to verify that the sequence of user touches
// matches the computer-generated sequence.
//*****************************************************************************

#ifndef VERIFYSEQUENCE_H_
#define VERIFYSEQUENCE_H_

// State machine will run when enabled.
void verifySequence_enable();

// This is part of the interlock. You disable the state-machine and then enable it again.
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

// Standard tick function.
void verifySequence_tick();

// Standard runTest function.
void verifySequence_runTest();

#endif /* VERIFYSEQUENCE_H_ */
