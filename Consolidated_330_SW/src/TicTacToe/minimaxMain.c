#include <stdio.h>
#include <stdint.h>
#include "minimax.h"

int main() {
  minimax_board_t board1;  // Board 1 is the main example in the web-tutorial that I use on the web-site.
  board1.squares[0][0] = MINIMAX_OPPONENT_SQUARE;
  board1.squares[0][1] = MINIMAX_EMPTY_SQUARE;
  board1.squares[0][2] = MINIMAX_PLAYER_SQUARE;
  board1.squares[1][0] = MINIMAX_PLAYER_SQUARE;
  board1.squares[1][1] = MINIMAX_EMPTY_SQUARE;
  board1.squares[1][2] = MINIMAX_EMPTY_SQUARE;
  board1.squares[2][0] = MINIMAX_PLAYER_SQUARE;
  board1.squares[2][1] = MINIMAX_OPPONENT_SQUARE;
  board1.squares[2][2] = MINIMAX_OPPONENT_SQUARE;

  minimax_board_t board2;
  board2.squares[0][0] = MINIMAX_OPPONENT_SQUARE;
  board2.squares[0][1] = MINIMAX_EMPTY_SQUARE;
  board2.squares[0][2] = MINIMAX_PLAYER_SQUARE;
  board2.squares[1][0] = MINIMAX_EMPTY_SQUARE;
  board2.squares[1][1] = MINIMAX_EMPTY_SQUARE;
  board2.squares[1][2] = MINIMAX_EMPTY_SQUARE;
  board2.squares[2][0] = MINIMAX_PLAYER_SQUARE;
  board2.squares[2][1] = MINIMAX_EMPTY_SQUARE;
  board2.squares[2][2] = MINIMAX_OPPONENT_SQUARE;

  minimax_board_t board3;
  board3.squares[0][0] = MINIMAX_OPPONENT_SQUARE;
  board3.squares[0][1] = MINIMAX_EMPTY_SQUARE;
  board3.squares[0][2] = MINIMAX_EMPTY_SQUARE;
  board3.squares[1][0] = MINIMAX_OPPONENT_SQUARE;
  board3.squares[1][1] = MINIMAX_EMPTY_SQUARE;
  board3.squares[1][2] = MINIMAX_EMPTY_SQUARE;
  board3.squares[2][0] = MINIMAX_PLAYER_SQUARE;
  board3.squares[2][1] = MINIMAX_EMPTY_SQUARE;
  board3.squares[2][2] = MINIMAX_PLAYER_SQUARE;

  minimax_board_t board4;
  board4.squares[0][0] = MINIMAX_OPPONENT_SQUARE;
  board4.squares[0][1] = MINIMAX_EMPTY_SQUARE;
  board4.squares[0][2] = MINIMAX_EMPTY_SQUARE;
  board4.squares[1][0] = MINIMAX_EMPTY_SQUARE;
  board4.squares[1][1] = MINIMAX_EMPTY_SQUARE;
  board4.squares[1][2] = MINIMAX_EMPTY_SQUARE;
  board4.squares[2][0] = MINIMAX_PLAYER_SQUARE;
  board4.squares[2][1] = MINIMAX_EMPTY_SQUARE;
  board4.squares[2][2] = MINIMAX_PLAYER_SQUARE;

  minimax_board_t board5;
  board5.squares[0][0] = MINIMAX_PLAYER_SQUARE;
  board5.squares[0][1] = MINIMAX_PLAYER_SQUARE;
  board5.squares[0][2] = MINIMAX_EMPTY_SQUARE;
  board5.squares[1][0] = MINIMAX_EMPTY_SQUARE;
  board5.squares[1][1] = MINIMAX_OPPONENT_SQUARE;
  board5.squares[1][2] = MINIMAX_EMPTY_SQUARE;
  board5.squares[2][0] = MINIMAX_EMPTY_SQUARE;
  board5.squares[2][1] = MINIMAX_EMPTY_SQUARE;
  board5.squares[2][2] = MINIMAX_EMPTY_SQUARE;

  minimax_board_t board6;
  board6.squares[0][0] = MINIMAX_EMPTY_SQUARE;
  board6.squares[0][1] = MINIMAX_PLAYER_SQUARE;
  board6.squares[0][2] = MINIMAX_EMPTY_SQUARE;
  board6.squares[1][0] = MINIMAX_EMPTY_SQUARE;
  board6.squares[1][1] = MINIMAX_EMPTY_SQUARE;
  board6.squares[1][2] = MINIMAX_PLAYER_SQUARE;
  board6.squares[2][0] = MINIMAX_OPPONENT_SQUARE;
  board6.squares[2][1] = MINIMAX_OPPONENT_SQUARE;
  board6.squares[2][2] = MINIMAX_PLAYER_SQUARE;

  minimax_board_t board7;
  board7.squares[0][0] = MINIMAX_EMPTY_SQUARE;
  board7.squares[0][1] = MINIMAX_PLAYER_SQUARE;
  board7.squares[0][2] = MINIMAX_EMPTY_SQUARE;
  board7.squares[1][0] = MINIMAX_EMPTY_SQUARE;
  board7.squares[1][1] = MINIMAX_OPPONENT_SQUARE;
  board7.squares[1][2] = MINIMAX_PLAYER_SQUARE;
  board7.squares[2][0] = MINIMAX_EMPTY_SQUARE;
  board7.squares[2][1] = MINIMAX_OPPONENT_SQUARE;
  board7.squares[2][2] = MINIMAX_PLAYER_SQUARE;

  minimax_board_t board8;
  board8.squares[0][0] = MINIMAX_EMPTY_SQUARE;
  board8.squares[0][1] = MINIMAX_PLAYER_SQUARE;
  board8.squares[0][2] = MINIMAX_EMPTY_SQUARE;
  board8.squares[1][0] = MINIMAX_EMPTY_SQUARE;
  board8.squares[1][1] = MINIMAX_PLAYER_SQUARE;
  board8.squares[1][2] = MINIMAX_EMPTY_SQUARE;
  board8.squares[2][0] = MINIMAX_OPPONENT_SQUARE;
  board8.squares[2][1] = MINIMAX_EMPTY_SQUARE;
  board8.squares[2][2] = MINIMAX_EMPTY_SQUARE;

  uint8_t row, column;
  printf("Boards Initialized...\n\r\n\r");
  minimax_computeNextMove(&board1, true, &row, &column);
  printf("next move for board1: (%d, %d)\n\n\n", row, column);
  minimax_computeNextMove(&board2, true, &row, &column);
  printf("next move for board2: (%d, %d)\n\n\n", row, column);
  minimax_computeNextMove(&board3, true, &row, &column);
  printf("next move for board3: (%d, %d)\n\n\n", row, column);
  minimax_computeNextMove(&board4, false, &row, &column);
  printf("next move for board4: (%d, %d)\n\n\n", row, column);
  minimax_computeNextMove(&board5, false, &row, &column);
  printf("next move for board5: (%d, %d)\n\n\n", row, column);
  minimax_computeNextMove(&board6, true, &row, &column);
  printf("next move for board6: (%d, %d)\n\n\n", row, column);
  minimax_computeNextMove(&board7, true, &row, &column);
  printf("next move for board7: (%d, %d)\n\n\n", row, column);
  minimax_computeNextMove(&board8, true, &row, &column);
  printf("next move for board8: (%d, %d)\n\n\n", row, column);
  minimax_computeNextMove(&board6, false, &row, &column);
  printf("next move for board9: (%d, %d)\n\n\n", row, column);

 return 0;
}
