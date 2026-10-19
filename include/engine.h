#include "chess.h"

struct Engine {
  ChessState current;
  char engine_color;
  char current_color;
};

void engine_init(struct Engine *engine);
ChessMove engine_guess(struct Engine *engine);
