//*****************************************************************************
// Luke Hsiao
// 20 May 2015
// Interface of the SM to control the Simon game at the top level.
//*****************************************************************************

#ifndef SIMONCONTROL_H_
#define SIMONCONTROL_H_

#define SIMONCONTROL_INIT_SEQ_LENGTH  4  // initial length of the sequence.
#define SIMONCONTROL_SIMON_TEXTSIZE   5
#define SIMONCONTROL_TOUCH_TEXTSIZE   3
#define SIMONCONTROL_NUMREGIONS       4   // # of regions on the screen.
#define SIMONCONTROL_RESET_WAIT       10  // # of ticks to wait before resetting
#define SIMONCONTROL_DISPLAY_WAIT     20  // # of ticks to wait before resetting
#define SIMONCONTROL_BUFFER_SIZE      100



/**
 * Tick function for the high-level control of the Simon game.
 */
void simonControl_tick();

#endif /* SIMONCONTROL_H_ */
