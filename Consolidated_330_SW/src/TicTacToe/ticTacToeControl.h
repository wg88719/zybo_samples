//*****************************************************************************
// Luke Hsiao
// 14 May 2015
// Interface of the control State Machine for Tic Tac Toe
//*****************************************************************************

#ifndef TICTACTOECONTROL_H_
#define TICTACTOECONTROL_H_

#define TICTACTOECONTROL_INSTR_TEXTSIZE   2  // size of instruction's text

// Constants that control how long the SM waits in various states.
#define TICTACTOECONTROL_INSTRUCTIONTIME  20 // # of ticks to show instructions
#define TICTACTOECONTROL_FIRSTMOVE_WAIT   20 // # of ticks before AI moves.
#define TICTACTOECONTROL_ADC_WAIT          0 // # of additional ticks to wait
                                             // for the ADC to settle

/**
 * Tick function that controls the state machine for TIC-TAC-TOE.
 */
void ticTacToeControl_tick();

#endif /* TICTACTOECONTROL_H_ */
