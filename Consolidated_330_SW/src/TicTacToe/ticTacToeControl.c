//*****************************************************************************
// Luke Hsiao
// 14 May 2015
// Implementation of the control State Machine for Tic Tac Toe
//*****************************************************************************

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "ticTacToeControl.h"
#include "ticTacToeDisplay.h"
#include "supportFiles/display.h"
#include "supportFiles/arduinoTypes.h"
#include "minimax.h"
#include "buttons.h"

// States for the controller state machine.
enum ticTacToeControl_st {
  init_st,              // Initial state, wait for one tick
  show_instructions_st, // Initial screen to show player instructions
  instruction_wait_st,  // Wait while displaying instructions
  first_move_st,        // Wait for user to go, or let AI go
  ad_timer_running_st,  // Wait for touch ADC to settle
  game_over_st,         // Wait for a restart
  player_turn_st,       // Wait for touch
  computer_turn_st      // Have AI make their move
} currentState = init_st; // Initialize to init_st

/**
 * This is a debug state print routine. It will print names of the states each
 * time tick() is called. It only prints states if they are different than the
 * previous state.
 */
void ticTacToeControl_debugStatePrint() {
  static ticTacToeControl_st previousState;
  static bool firstPass = true;
  // Only print the message if:
  // 1. This the first pass and the value for previousState is unknown.
  // 2. previousState != currentState - this prevents reprinting the same stat.
  if (previousState != currentState || firstPass) {
    firstPass = false;     // previousState will be defined, firstPass is false.
    previousState = currentState; // keep track of the last state you were in.
    switch(currentState) {  // prints info based on the state that you're in.
      case init_st:
        printf("init_st\n\r");
        break;
      case show_instructions_st:
        printf("show_instructions_st\n\r");
        break;
      case instruction_wait_st:
        printf("instruction_wait_st\n\r");
        break;
      case first_move_st:
        printf("first_move_st\n\r");
        break;
      case ad_timer_running_st:
        printf("ad_timer_running_st\n\r");
        break;
      case game_over_st:
        printf("game_over_st\n\r");
        break;
      case player_turn_st:
        printf("player_turn_st\n\r");
        break;
      case computer_turn_st:
        printf("computer_turn_st\n\r");
        break;
      default:
        printf("Shouldn't have hit this default case\n");
        break;
     }
  }
}

/**
 * Helper function that updates the internal variable representing the board
 * and draws the corresponding value to the screen.
 * @param board  Pointer to the curent board.
 * @param row    Row value to update
 * @param column Column value to update
 * @param player Representation of which player is placing a move.
 */
void ticTacToeControl_updateBoard(minimax_board_t* board, uint8_t row, uint8_t column, bool player) {
  if (player) {
    // Update the internal variable.
    (*board).squares[row][column] = MINIMAX_PLAYER_SQUARE;
    // Draw the X to the screen.
    ticTacToeDisplay_drawX(row, column);
  }
  else {
    // Update the internal variable.
    (*board).squares[row][column] = MINIMAX_OPPONENT_SQUARE;
    // Draw the O to the screen.
    ticTacToeDisplay_drawO(row, column);
  }
}

void ticTacToeControl_tick() {
  // Uncomment the line below to visually see state transitions in console.
  //ticTacToeControl_debugStatePrint();

  // Variables representing the timers that will be used
  static uint32_t adTimer = 0; // Time to wait for touch-controller ADC to settle.
  static uint32_t firstMoveTimer = 0;  // Time to give the user before AI moves.
  static uint32_t instructionTimer = 0; // Time to show instructions before showing board

  static uint8_t row, column;   // Variables to track rows/columns
  static minimax_board_t board; // Variable to store the current play board
  static bool player_first;     // Variable to show who is set to make the first move

  /////////////////////////////////
  // Perform state action first. //
  /////////////////////////////////
  switch(currentState) {
    case init_st:
      break;
    case show_instructions_st:
      // Initialize the display and print the instructions to the screen
      display_init();
      display_fillScreen(DISPLAY_BLACK);
      display_setTextSize(TICTACTOECONTROL_INSTR_TEXTSIZE);
      display_println("\n\nTouch a square to play 'X'\n\r\n\r    -or-\n\r\n\rwait for the computer and play 'O'");

      instructionTimer = 0; // reset instruction timer
      break;
    case instruction_wait_st:
      firstMoveTimer = 0; // reset timer before AI makes a move.
      instructionTimer++; // increment counter to display instructions.
      break;
    case first_move_st:
      // Reset the internal board variable to ALL EMPTY.
      minimax_initBoard(&board);

      // Initialize the row/column variables to 0.
      row = 0;
      column = 0;

      // Initialize the player_first to be false.
      player_first = false;

      instructionTimer = 0; // Reset instructionTimer

      // Increment counter to wait for player to make a move.
      firstMoveTimer++;
      break;
    case ad_timer_running_st: // increment adTimer
      adTimer++;
      break;
    case game_over_st:
      firstMoveTimer = 0;
      break;
    case player_turn_st:
      // Do nothing
      break;
    case computer_turn_st: // set rateTimer to 0
      // Have the AI make it's move
      minimax_computeNextMove(&board, !player_first, &row, &column);  // compute move
      // Update the screen and variables with the AI's move.
      ticTacToeControl_updateBoard(&board, row, column, !player_first); // record move
      break;
     default: // Shouldn't hit this state ever.
      printf("ticTacToeControl_tick state action: hit default\n\r");
      break;
  }
  ////////////////////////////////
  // Perform state update next. //
  ////////////////////////////////
  switch(currentState) {
    case init_st:
      currentState = show_instructions_st;  // First, show instructions
      break;
    case show_instructions_st:
      currentState = instruction_wait_st; // Display instructions for a time.
      break;
    case instruction_wait_st:
      // Wait for instructions to be shown for a period of time
      if (instructionTimer >= TICTACTOECONTROL_INSTRUCTIONTIME) {
        currentState = first_move_st; // Move to waiting for the first move
        ticTacToeDisplay_init();  // Initialize the board
        firstMoveTimer = 0; // Reset first move timer
      }
      else {
        currentState = instruction_wait_st;
      }
      break;
    case first_move_st:  // wait to determine player or AI first
      // If the display is touched, move on to let ADC settle
      if (display_isTouched()) {  // player wants to move first
        display_clearOldTouchData();  // clear old data to read current coordinates
        currentState = ad_timer_running_st;
        player_first = true;  // player will be playing X
      }
      // Otherwise, if player never touches the screen, have AI go first
      else if (firstMoveTimer >= TICTACTOECONTROL_FIRSTMOVE_WAIT) {
        player_first = false; // AI goes first, player is 'O'
        currentState = computer_turn_st;
      }
      else {
        currentState = first_move_st; // Otherwise keep waiting here
      }
      break;
    case ad_timer_running_st: // increment adTimer
      // The display must be let go after ADC settles before action is taken
      if (!display_isTouched() && adTimer >= TICTACTOECONTROL_ADC_WAIT) {
        // Get the touched point after waiting
        ticTacToeDisplay_touchScreenComputeBoardRowColumn(&row, &column);
        // Register the touch and update the board.
        ticTacToeControl_updateBoard(&board, row, column, player_first);
        // If that was the final move, go to gameover.
        if (minimax_isGameOver(minimax_computeBoardScore(&board, player_first))) {
          currentState = game_over_st;
        }
        // Otherwise, let the computer have its turn.
        else {
          currentState = computer_turn_st;
        }
      }
      // Otherwise, stay in this state
      else {
        currentState = ad_timer_running_st;
      }
      break;
    case game_over_st: // wait for user to push BTN 0 to reset
      if (buttons_read() & BUTTONS_BTN0_MASK) { // if btn0 is pressed
        currentState = first_move_st; // wait for the first move again.
        firstMoveTimer = 0; // Reset first move timer
        ticTacToeDisplay_init();  // Initialize the board
      }
      else {
        currentState = game_over_st;  // otherwise, just wait here.
      }

      break;
    case player_turn_st: // increment rateTimer
      // Allow the player to reset the game during his turn
      if (buttons_read() & BUTTONS_BTN0_MASK) { // if btn0 is pressed
        currentState = first_move_st;
        ticTacToeDisplay_init();  // Initialize the board
      }
      // Or, the the game is over, go to game_over_st
      else if (minimax_isGameOver(minimax_computeBoardScore(&board, player_first))) {
        currentState = game_over_st;
      }
      // Otherwise, allow the player to make his/her move.
      else if (display_isTouched()) {
        ticTacToeDisplay_touchScreenComputeBoardRowColumn(&row, &column);
        // Only update the board if it was an empty spot
        if (board.squares[row][column] == MINIMAX_EMPTY_SQUARE) {
          currentState = ad_timer_running_st; // Let ADC settle then process input
        }
      }
      else {
        currentState = player_turn_st;  // wait for user to make a move
      }
      break;
    case computer_turn_st: // set rateTimer to 0
      // Moore action is performed to determine the AI's move, so we can
      // directly move on to the player's turn. Note that the player_turn_st
      // takes care of checking whether the AI's move puts the board in a
      // game over state.
      currentState = player_turn_st;
      break;
     default: // should never hit this case.
      printf("ticTacToeControl_tick state update: hit default\n\r");
      break;

  }
}
