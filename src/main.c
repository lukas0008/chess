#include "chess.h"
#include "terminal.h"
#include <stdio.h>
#include <string.h>

int main() {
  ChessState board = initboard();
  printboard(&board, (char)(2 << 6));

  char color = 0;
  while (1) {
    printf("%s to move\n", color ? "Black" : "White");
    char input[20];
    get_input(input, 20);
    printf("%s\n", input);
    if (strcmp(input, "q") == 0 || strcmp(input, "quit") == 0 ||
        strcmp(input, "exit") == 0) {
      printf("exiting\n");
      return 0;
    }

    ChessMove move;
    char move_res = parsemove(&move, input);
    if (move_res) {
      if (move_res < 0) {
        printf("invalid move input %i\n", move_res);
        continue;
      }
      printboard(&board, (char)(color << 6) | move_res);
      continue;
    }

    // printf("s: %u e: %u\n", move.start, move.end);

    if (validatemove(&board, move) != color) {
      printf("invalid move\n");
      continue;
    }
    makemove(&board, move);

    printboard(&board, (char)(2 << 6));
    color = !color;
  }

  return 0;
}
