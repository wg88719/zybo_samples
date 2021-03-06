//*****************************************************************************
// Luke Hsiao
// 13 May 2015
// Interface of the minimax algorithm for the AI player in TicTacToe.
//*****************************************************************************
#ifndef MINIMAX_H_
#define MINIMAX_H_

#include <stdbool.h>
#include <stdint.h>

// Uncomment this line to enable DEBUG output
// #define MINIMAX_DEBUG

// Defines the boundaries of the tic-tac-toe board.
// Note that for the algorithm to properly check diagonals, The dimension of
// rows and columns should be the same.
#define MINIMAX_BOARD_ROWS    3
#define MINIMAX_BOARD_COLUMNS 3

// Calculates the total number of squares on the board for score calculations
#define MINIMAX_TOTALSQUARES ((MINIMAX_BOARD_ROWS) * (MINIMAX_BOARD_COLUMNS))
#define MINIMAX_INIT_DEPTH    -1  // initialize to -1 so that it is 0 at the
                                  // start of the minimax calls

// These are the values in the board to represent who is occupying what square.
#define MINIMAX_USED_SQUARE     3  // Not currently used.
#define MINIMAX_PLAYER_SQUARE   2  // Represents an X
#define MINIMAX_OPPONENT_SQUARE 1  // Represents an O
#define MINIMAX_EMPTY_SQUARE    0

// Scoring for minimax.
#define MINIMAX_PLAYER_WINNING_SCORE    10    // represents that X won
#define MINIMAX_OPPONENT_WINNING_SCORE  -10   // represents that O won
#define MINIMAX_DRAW_SCORE              0     // Draw game
#define MINIMAX_NOT_ENDGAME             -1    // Not an end-game.

// Boards contain just an array of squares. I used a struct to provide additional abstraction
// in case I wanted to add something to the board type.
typedef struct {
  uint8_t squares[MINIMAX_BOARD_ROWS][MINIMAX_BOARD_COLUMNS];  // State of game as passed in.
} minimax_board_t;

// A move is simply a row-column pair.
typedef struct {
  uint8_t row;
  uint8_t column;
} minimax_move_t;

// Define a score type.
typedef int16_t minimax_score_t;

/**
 * This routine itself is not recursive, but will call the recursive minimax
 * function. It computes the row and column of the next move based upon:
 *   1. The current board
 *   2. The player
 * @param board  A representation of the current state of the board.
 * @param player TRUE means computer is 'X', FALSE means computer is 'O'
 * @param row    The address of where to store the row of the AI's move.
 * @param column The address of where to store the col of the AI's move.
 */
void minimax_computeNextMove(minimax_board_t* board, bool player, uint8_t* row, uint8_t* column);

/**
 * Determine whether the game is over by looking at the score.
 * @param  score The current score.
 * @return       TRUE = game is over, FALSE = game isn't over.
 */
bool minimax_isGameOver(minimax_score_t score);

/**
 * Returns the score of the board.
 * @param  board  Representation of the current board.
 * @return        The score of board.
 */
int16_t minimax_computeBoardScore(minimax_board_t* board);

/**
 * Initialize the board as all empty squares.
 * @param board The address of the board to initialize.
 */
void minimax_initBoard(minimax_board_t* board);

#endif /* MINIMAX_H_ */
