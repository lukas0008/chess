#include "engine.h"
#include "chess.h"
#include "dynarray.h"
#include "hashmap.h"
#include <stddef.h>
#include <stdint.h>
#include <string.h>

static int piece_score_map[12 * 64];
static const int position_score_map[6][64] = {
    /* Bishop */
    {-20, -10, -10, -10, -10, -10, -10, -20, -10, 5,   0,   0,   0,
     0,   5,   -10, -10, 10,  10,  10,  10,  10,  10,  -10, -10, 0,
     10,  10,  10,  10,  0,   -10, -10, 5,   5,   10,  10,  5,   5,
     -10, -10, 0,   5,   10,  10,  5,   0,   -10, -10, 0,   0,   0,
     0,   0,   0,   -10, -20, -10, -10, -10, -10, -10, -10, -20},
    /* Knight */
    {-50, -40, -30, -30, -30, -30, -40, -50, -40, -20, 0,   5,   5,
     0,   -20, -40, -30, 5,   10,  15,  15,  10,  5,   -30, -30, 0,
     15,  20,  20,  15,  0,   -30, -30, 5,   15,  20,  20,  15,  5,
     -30, -30, 0,   10,  15,  15,  10,  0,   -30, -40, -20, 0,   0,
     0,   0,   -20, -40, -50, -40, -30, -30, -30, -30, -40, -50},
    /* Rook */
    {0,  0,  0,  5,  5,  0,  0,  0,  -5, 0, 0, 0, 0, 0, 0, -5,
     -5, 0,  0,  0,  0,  0,  0,  -5, -5, 0, 0, 0, 0, 0, 0, -5,
     -5, 0,  0,  0,  0,  0,  0,  -5, -5, 0, 0, 0, 0, 0, 0, -5,
     5,  10, 10, 10, 10, 10, 10, 5,  0,  0, 0, 0, 0, 0, 0, 0},
    /* Queen */
    {-20, -10, -10, -5,  -5,  -10, -10, -20, -10, 0,   5,   0,  0,
     0,   0,   -10, -10, 5,   5,   5,   5,   5,   0,   -10, 0,  0,
     5,   5,   5,   5,   0,   -5,  -5,  0,   5,   5,   5,   5,  0,
     -5,  -10, 0,   5,   5,   5,   5,   0,   -10, -10, 0,   0,  0,
     0,   0,   0,   -10, -20, -10, -10, -5,  -5,  -10, -10, -20},
    /* King (middle game) */
    {20,  30,  10,  0,   0,   10,  30,  20,  20,  20,  0,   0,   0,
     0,   20,  20,  -10, -20, -20, -20, -20, -20, -20, -10, -20, -30,
     -30, -40, -40, -30, -30, -20, -30, -40, -40, -50, -50, -40, -40,
     -30, -30, -40, -40, -50, -50, -40, -40, -30, -30, -40, -40, -50,
     -50, -40, -40, -30, -30, -40, -40, -50, -50, -40, -40, -30},
    /* Pawn */
    {0,  0,  0,   0,  0,  0,   0,  0,  5,  10, 10, -20, -20, 10, 10, 5,
     5,  -5, -10, 0,  0,  -10, -5, 5,  0,  0,  0,  20,  20,  0,  0,  0,
     5,  5,  10,  25, 25, 10,  5,  5,  10, 10, 20, 30,  30,  20, 10, 10,
     50, 50, 50,  50, 50, 50,  50, 50, 0,  0,  0,  0,   0,   0,  0,  0},
};

static const int piece_values[6] = {
    330, 320, 500, 900, 0, 100,
};

void engine_init(struct Engine *engine) {}

int engine_score_immediate(struct Engine *engine) {
  int score = 0;
  for (size_t i = 0; i < 64; i++) {
    int piece = engine->current.pieces[i];
    if (piece == PIECE_EMPTY) {
      continue;
    }

    int piece_type = (piece - PIECE_BISHOP_WHITE) / 2;
    score += position_score_map[piece_type][i] * (piece % 2 == 0 ? 1 : -1);

    int color_multiplier = piece % 2 == 0 ? 1 : -1;
    score += piece_values[piece_type] * color_multiplier;
  }
  return score;
}

static int search(struct Engine *engine, int depth, int alpha, int beta) {
  if (depth == 0) {
    int immediate_score = engine_score_immediate(engine);
    return ((int)engine->engine_color * -2 + 1) * immediate_score;
  }

  ChessMove *moves = list_valid_moves(&engine->current, engine->current_color);

  if (vec_len(&moves) == 0) {
    int in_check = checkking(&engine->current, engine->current_color);
    vec_free(&moves);

    if (!in_check) {
      return 0;
    }

    return engine->current_color == engine->engine_color ? -1000000 - depth
                                                         : 1000000 + depth;
  }

  int maximizing = engine->current_color == engine->engine_color;
  int best_score = maximizing ? INT32_MIN : INT32_MAX;

  size_t move_count = vec_len(&moves);
  for (size_t i = 0; i < move_count; i++) {
    ChessMove *move = &moves[i];
    struct Engine child = *engine;

    makemove(&child.current, *move);
    child.current_color = !child.current_color;

    int score = search(&child, depth - 1, alpha, beta);

    if (maximizing) {
      if (score > best_score) {
        best_score = score;
      }
      if (best_score > alpha) {
        alpha = best_score;
      }
    } else {
      if (score < best_score) {
        best_score = score;
      }
      if (best_score < beta) {
        beta = best_score;
      }
    }

    if (beta <= alpha) {
      break;
    }
  }

  vec_free(&moves);
  return best_score;
}

#define SEARCH_DEPTH 4
int engine_score(struct Engine *engine) {
  return search(engine, SEARCH_DEPTH, INT32_MIN, INT32_MAX);
}

ChessMove engine_guess(struct Engine *engine) {
  ChessMove *moves = list_valid_moves(&engine->current, engine->current_color);

  ChessMove bestMove;
  int best_score = INT32_MIN;

  size_t move_count = vec_len(&moves);
  for (size_t i = 0; i < move_count; i++) {
    ChessMove *move = &moves[i];
    struct Engine engine_copy = *engine;
    engine_copy.current_color = !engine_copy.current_color;
    makemove(&engine_copy.current, *move);
    int s = engine_score(&engine_copy);

    if (s > best_score) {
      best_score = s;
      bestMove = *move;
    }
  }

  vec_free(&moves);

  return bestMove;
}
