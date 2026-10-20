#define DYNARRAY_IMPLEMENTATION
#define GENERATOR_IMPLEMENTATION
#include "dynarray.h"
#include "generator.h"

#include "chess.h"
#include "engine.h"
#include "input.h"
#include <stdio.h>

int main() {
  ChessState board = initboard();

  char color = 0;
  ChessMove move;
  struct Engine engine;
  engine_init(&engine);
  while (1) {
    if (color == 1) {
      engine.engine_color = 1;
      engine.current_color = 1;
      engine.current = board;
      ChessMove move = engine_guess(&engine);
      makemove(&board, move);
      color = !color;
      continue;
    }
    printboard(&board, -1);

    printf("%s to move\n", color ? "Black" : "White");

    char move_res = getmove(&move, &board, color);
    if (move_res < 0) {
      printf("invalid move input %i\n", move_res);
      continue;
    }

    if ((move_res = validatemove(&board, move)) != color) {
      printf("invalid move %i\n", move_res);
      continue;
    }

    makemove(&board, move);
    color = !color;
  }

  return 0;
}
