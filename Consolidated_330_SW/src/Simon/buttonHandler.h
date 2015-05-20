//*****************************************************************************
// Luke Hsiao
// 20 May 2015
// Interface of the SM to handle drawing buttons and detecting touches.
//*****************************************************************************

#ifndef BUTTONHANDLER_H_
#define BUTTONHANDLER_H_

/**
 * Get the simon region numbers.
 * See the source code for the region numbering scheme.
 * @return The region number that was pressed.
 */
uint8_t buttonHandler_getRegionNumber();

/**
 * Turn on the state machine. Part of the interlock.
 */
void buttonHandler_enable();

/**
 * Turn off the state machine. Part of the interlock.
 */
void buttonHandler_disable();

// Other state machines can call this function to see if the user has stopped touching the pad.
/**
 * Other state machines can call this function to see if the user has stopped
 * touching the pad.
 * @return TRUE = user has stopped touching the pad. FALSE = otherwise.
 */
bool buttonHandler_releaseDetected();

/**
 * Tick function for the buttonHandler State Machine.
 */
void buttonHandler_tick();

/**
 * Tests the functionality of the buttonHandler state machine. Runs until
 * the user has touched the screen touchCound times. It indicates that a
 * button was pushed by drawing a large square while the button is pressed
 * and then erasing the large square and redrawing the button when the
 * user releases their touch.
 * @param touchCount The number of touches before exiting runTest
 */
void buttonHandler_runTest(int16_t touchCount);

#endif /* BUTTONHANDLER_H_ */
