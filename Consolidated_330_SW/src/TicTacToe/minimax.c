//*****************************************************************************
// Luke Hsiao
// 13 May 2015
// Implementation of the minimax algorithm for the AI player in TicTacToe.
//*****************************************************************************

#include <stdio.h>
#include "minimax.h"

static int32_t depth = 0; // global variable to track the depth of our recursion

// global variable used to track the next move choice of the AI
static minimax_move_t choice;

/**
 * Helper function that prints out a texual representation of the internal
 * board variables.
 * @param board A pointer to the variable representing the state of the board.
 */
void printBoard(minimax_board_t* board) {
  printf("\r\n"); // Start on a new line
  int i, j; // variables used in iteration
  for (i = 0; i < MINIMAX_BOARD_ROWS; i++) { // every row
    for (j = 0; j < MINIMAX_BOARD_COLUMNS; j++) { // every column
      if (board->squares[i][j] == MINIMAX_PLAYER_SQUARE) {
        printf("X ");
      }
      else if (board->squares[i][j] == MINIMAX_OPPONENT_SQUARE) {
        printf("O ");
      }
      else if (board->squares[i][j] == MINIMAX_EMPTY_SQUARE) {
        printf("- ");
      }
      else { //error case, should never be hit
        // If this case is hit, print out whatever erroneous value
        // happened to be in the square.
        printf("%d ", board->squares[i][j]);
      }
    }
    printf("\r\n");
  }
}

/**
 * Recursive MiniMax algorithm that computes the best move at the current call's
 * move depth, based on the player.
 * @param  board  A pointer to a representation of the board to compute scores
 *                on. Note that this may be different than the actual current
 *                board.
 * @param  player Whether to evaluate the board as the player or the opponent.
 * @return        The MAX score if player, or MIN score if Opponent.
 */
minimax_score_t minimax(minimax_board_t* board, bool player) {
  // Increment depth at the beginning of this function call
  depth++;

  // Create 2 arrays (indexed the same) to save moves and their score
  // at this level of recursion.
  minimax_move_t moves[MINIMAX_TOTALSQUARES];
  minimax_score_t scores[MINIMAX_TOTALSQUARES];

  // Recursion base case, there has been a win or a draw.
  if (minimax_isGameOver(minimax_computeBoardScore(board))) {
    depth--;  // decrement depth as we return;

    // Since this call is the end case, simply return the board's score
    return minimax_computeBoardScore(board);
  }

  // Otherwise, the recursion is called.
  // This while-loop will generate all possible boards.
  int16_t index = 0;  // used to track the number of score/move pairs.
  int row, col;
  for (row = 0; row < MINIMAX_BOARD_ROWS; row++) {  // For each row
    for (col = 0; col < MINIMAX_BOARD_ROWS; col++) {  // For each column
      // Find an empty square
      if ((*board).squares[row][col] == MINIMAX_EMPTY_SQUARE) {
        // Assign the square to the player or opponent.
        if (player) {
          (*board).squares[row][col] = MINIMAX_PLAYER_SQUARE;
        }
        else {
          (*board).squares[row][col] = MINIMAX_OPPONENT_SQUARE;
        }
        // Make the recursive call that determines the MIN or MAX score
        // based on the player to save to the table.
        minimax_score_t score = minimax(board, !player);

        scores[index] = score;  // Add score to the score table
        moves[index].row = row; // Add move row to the move table
        moves[index].column = col;  // Add move column to the move table

        index++;  // Increment the index into the moves/scores table

        // Undo the change to the board (return the square to empty)
        // prior to next iteration of for-loops.
        (*board).squares[row][col] = MINIMAX_EMPTY_SQUARE;
      }
    }
  }

#ifdef MINIMAX_DEBUG
  // Print out Score Matrix at Top Level if DEBUG mode is enabled.
  // This is useful in visually checking that the score table is correct
  // after all of the recursive calls have been made.
 if (depth == 0) {
   printf("Scores:\n\r");
   int count;
   for (count = 0; count < index; count++) {
     printf("Move (%d, %d) Score: %d\n", moves[count].row, moves[count].column, scores[count]);
   }
 }
 #endif

  // Once here, we have iterated over empty squares at this level.
  // All of the scores and moves have been computed at this level.
  minimax_score_t score = 0; // the value of the score to return
  int16_t tempIndex = 0; // variable used to store the index of the MIN/MAX
  // Now we return the score based upon whether we are computing min or max.
  int i;
  if (player) {
    // If the player is X, we want to find the MAX score
    int16_t highScore = scores[0];  // Initialize to the first score.
    // Iterate over all of the move/score pairs in the table
    for (i = 0; i < index; i++) {
      if (scores[i] > highScore) {
        // Raise the maximum highScore found so far
        highScore = scores[i];
        tempIndex = i;
      }
    }
    choice = moves[tempIndex];  // Get the choice with the MAX score
    score = scores[tempIndex];  // Return the highest score in the table.
  }
  else {
    // If we are looking at 'O', we want to find the MIN score
    int16_t lowScore = scores[0];  // Initialize to the first score.
    // Iterate over all of the move/score pairs in the table
    for (i = 0; i < index; i++) {
      if (scores[i] < lowScore) {
        // Lower the minimum lowScore found so far
        lowScore = scores[i];
        tempIndex = i;
      }
    }
    choice = moves[tempIndex];  // Get the choice with the MIN score
    score = scores[tempIndex];  // Return the lowest score in the table.
  }

  // Done with the recursive call, decrement depth as we return
  depth--;
  return score;
}

void minimax_computeNextMove( minimax_board_t* board,
                              bool player,
                              uint8_t* row,
                              uint8_t* column) {
  // Initialize the depth
  depth = MINIMAX_INIT_DEPTH;

#ifdef MINIMAX_DEBUG
  // If DEBUG mode is enabled, print out board and player information
  if (player) {
    printf("Player is placing 'X'");
  }
  else {
    printf("Player is placing 'O'");
  }
  printBoard(board);  // Print the current board to the console for debugging
#endif

  // If the board if empty, just choose any square.
  // This reduced the worst-case tick function execution time from 367ms
  // to ~180ms.
  bool notEmpty = false;
  int i, j;
  for (i = 0; i < MINIMAX_BOARD_ROWS; i++) {  // For each row
    for (j = 0; j < MINIMAX_BOARD_ROWS; j++) {  // For each column
      if ((*board).squares[i][j] != MINIMAX_EMPTY_SQUARE) {
        notEmpty = true;
      }
    }
  }
  if (notEmpty) {
    minimax(board, player);
    *row = choice.row;  // assign the final row choice
    *column = choice.column;  // assign the final column choice
  }
  else {
    // If the board is empty, just choose the first square.
    // In the future, this could be randomized.
    *row = 0;
    *column = 0;
  }

}

bool minimax_isGameOver(minimax_score_t score) {
  // Check if score is the ENDGAME
  if (score == MINIMAX_NOT_ENDGAME) {
    return false;
  }
  else {
    return true;
  }
}

int16_t minimax_computeBoardScore(minimax_board_t* board) {
  bool emptySquaresExist = false; // flag to track whether there are empties
  // Variables to track the count of symbols in each of the possible
  // ways to win. I will +1 for player's symbols, and -1 for opponents.
  int32_t rowCount[MINIMAX_BOARD_ROWS], colCount[MINIMAX_BOARD_COLUMNS];
  int32_t diag_topleft_bottomright = 0;
  int32_t diag_topright_bottomleft = 0;

  int row, col;
  // Init rowCount and colCount to zeros
  for (row = 0; row < MINIMAX_BOARD_ROWS; row++) {
    rowCount[row] = 0;
  }
  for (col = 0; col < MINIMAX_BOARD_COLUMNS; col++) {
    colCount[col] = 0;
  }

  // This algorithm is designed so that the board is only iterated over ONCE.
  // If X is in a square, a count in incremented, if O is in a square, the count
  // is decremented. This way, if there are 3 X in a row, the count will be 3.
  // Or, if there are 3 Os, the count will be -3. Nothing is added for empty
  // squares.
  for (row = 0; row < MINIMAX_BOARD_ROWS; row++) {
    for (col = 0; col < MINIMAX_BOARD_COLUMNS; col++) {
      // If player's symbol is found, increment all counts
      if ((*board).squares[row][col] == MINIMAX_PLAYER_SQUARE) {
        rowCount[row]++;  // increment the count for the row it is on.
        colCount[col]++;  // increment the count for the col it is on.
        if (row == col) { // if square is on topleft to bottomright diag
          diag_topleft_bottomright++;
        }
        if (col == (MINIMAX_BOARD_ROWS - row - 1)) {  // if on opposite diag
          diag_topright_bottomleft++;
        }
      }
      // If it's an opponent symbol, decrement all counts
      else if ((*board).squares[row][col] == MINIMAX_OPPONENT_SQUARE) {
        rowCount[row]--;  // decrement the count for the row it is on.
        colCount[col]--;  // decrement the count for the col it is on.
        if (row == col) { // if square is on topleft to bottomright diag
          diag_topleft_bottomright--;
        }
        if (col == (MINIMAX_BOARD_ROWS - row - 1)) {  // if on opposite diag
          diag_topright_bottomleft--;
        }
      }
      else {  // If an empty square was found, update the flag.
        emptySquaresExist = true;
      }
    }
  }

  // If we reached here, we've checked the whole board Now check all of the
  // count values. If any of them equal +3, PLAYER wins. If -3, OPPONENT wins.
  // Note that depth is either added or subtracted in order to allow the AI
  // to play more intelligently in that it will take the immediate WIN, or it
  // will try to prolong it's inevitable LOSS as much as possible.

  // Check row counts
  for (row = 0; row < MINIMAX_BOARD_ROWS; row++) {
    // If the 'X' won...
    if (rowCount[row] == MINIMAX_BOARD_ROWS) {
      return MINIMAX_PLAYER_WINNING_SCORE - depth;
    }
    // If 'O' won...
    else if (rowCount[row] == (-MINIMAX_BOARD_ROWS)) {
      return MINIMAX_OPPONENT_WINNING_SCORE + depth;
    }
  }

  // Check column counts
  for (col = 0; col < MINIMAX_BOARD_COLUMNS; col++) {
    // If the 'X' won...
    if (colCount[col] == MINIMAX_BOARD_COLUMNS) {
      return MINIMAX_PLAYER_WINNING_SCORE - depth;
    }
    // If 'O' won...
    else if (colCount[col] == (-MINIMAX_BOARD_COLUMNS)) {
      return MINIMAX_OPPONENT_WINNING_SCORE + depth;
    }
  }

  // Check Diagonal counts
  // Note that this makes the assumption that # ROWS = # COLUMNS
  // if 'X' won
  if (diag_topleft_bottomright == MINIMAX_BOARD_COLUMNS ||
      diag_topright_bottomleft == MINIMAX_BOARD_COLUMNS) {
    return MINIMAX_PLAYER_WINNING_SCORE - depth;
  }
  // If 'O' won...
  else if ( diag_topleft_bottomright == (-MINIMAX_BOARD_COLUMNS) ||
            diag_topright_bottomleft == (-MINIMAX_BOARD_COLUMNS)) {
    return MINIMAX_OPPONENT_WINNING_SCORE + depth;
  }

  if (emptySquaresExist) {
    return MINIMAX_NOT_ENDGAME;
  }
  else {
    // If none of the players won, and the board is full, it was a draw.
    return MINIMAX_DRAW_SCORE;
  }
}

void minimax_initBoard(minimax_board_t* board) {
  int row, col;
  // Iterate over all the rows
  for (row = 0; row < MINIMAX_BOARD_ROWS; row++) {
    // Iterate over all the columns
    for (col = 0; col < MINIMAX_BOARD_COLUMNS; col++) {
      (*board).squares[row][col] = MINIMAX_EMPTY_SQUARE;  // set square to EMPTY
    }
  }
}
