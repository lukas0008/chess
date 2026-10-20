#include "chess.h"
#include "dynarray.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#define VALID_MOVES 288

char piece_from_alphabetic(char piece) {
  switch (piece) {
  case 'B':
    return PIECE_BISHOP_WHITE;
  case 'b':
    return PIECE_BISHOP_BLACK;
  case 'N':
    return PIECE_KNIGHT_WHITE;
  case 'n':
    return PIECE_KNIGHT_BLACK;
  case 'R':
    return PIECE_ROOK_WHITE;
  case 'r':
    return PIECE_ROOK_BLACK;
  case 'Q':
    return PIECE_QUEEN_WHITE;
  case 'q':
    return PIECE_QUEEN_BLACK;
  case 'K':
    return PIECE_KING_WHITE;
  case 'k':
    return PIECE_KING_BLACK;
  case 'P':
    return PIECE_PAWN_WHITE;
  case 'p':
    return PIECE_PAWN_BLACK;
  case ' ':
    return PIECE_EMPTY;
  default:
    return -1;
  }
}

ChessState initboard() {
  ChessState state = {
      // clang-format off
      .pieces = {
      'R', 'N', 'B', 'Q', 'K', 'B', 'N', 'R',
      'P', 'P', 'P', 'P', 'P', 'P', 'P', 'P',
      ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
      ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
      ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
      ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
      'p', 'p', 'p', 'p', 'p', 'p', 'p', 'p',
      'r', 'n', 'b', 'q', 'k', 'b', 'n', 'r',
      }};
  for (size_t i = 0; i < 64; i++) {
    state.pieces[i] = piece_from_alphabetic(state.pieces[i]);
  }
  // clang-format on
  return state;
}

void printboard(ChessState *state, int from_pos) {
  int color = from_pos >> 6;
  char cangoto[8] = "\0\0\0\0\0\0\0\0";
  if (from_pos >= 0) {
    for (char i = 0; i < 64; i++) {
      if (validatemove(state, (ChessMove){.start = (char)(from_pos & 63),
                                          .end = i}) == color) {
        cangoto[i / 8] |= 1 << (i % 8);
        // printf("cango %i %i", i / 8, i % 8);
      }
    }
  }

  int int_array[120];
  printf("    a   b   c   d   e   f   g   h\n"
         "   ———+———+———+———+———+———+———+———\n");
  for (int i = 7; i >= 0; i--) {
    printf("%d | ", i + 1);
    for (int j = 0; j < 8; j++) {
      char *display_char = "";
      switch (state->pieces[i * 8 + j]) {
      case PIECE_KING_WHITE:
        display_char = CHESS_FLIP_DISPLAY_COLORS ? "♚" : "♔";
        break;
      case PIECE_QUEEN_WHITE:
        display_char = CHESS_FLIP_DISPLAY_COLORS ? "♛" : "♕";
        break;
      case PIECE_ROOK_WHITE:
        display_char = CHESS_FLIP_DISPLAY_COLORS ? "♜" : "♖";
        break;
      case PIECE_BISHOP_WHITE:
        display_char = CHESS_FLIP_DISPLAY_COLORS ? "♝" : "♗";
        break;
      case PIECE_KNIGHT_WHITE:
        display_char = CHESS_FLIP_DISPLAY_COLORS ? "♞" : "♘";
        break;
      case PIECE_PAWN_WHITE:
        display_char = CHESS_FLIP_DISPLAY_COLORS ? "♟" : "♙";
        break;
      case PIECE_KING_BLACK:
        display_char = CHESS_FLIP_DISPLAY_COLORS ? "♔" : "♚";
        break;
      case PIECE_QUEEN_BLACK:
        display_char = CHESS_FLIP_DISPLAY_COLORS ? "♕" : "♛";
        break;
      case PIECE_ROOK_BLACK:
        display_char = CHESS_FLIP_DISPLAY_COLORS ? "♖" : "♜";
        break;
      case PIECE_BISHOP_BLACK:
        display_char = CHESS_FLIP_DISPLAY_COLORS ? "♗" : "♝";
        break;
      case PIECE_KNIGHT_BLACK:
        display_char = CHESS_FLIP_DISPLAY_COLORS ? "♘" : "♞";
        break;
      case PIECE_PAWN_BLACK:
        display_char = CHESS_FLIP_DISPLAY_COLORS ? "♙" : "♟";
        break;
      case PIECE_EMPTY:
        display_char = " ";
        break;
      default:
        display_char = "x";
        break;
      }
      char should = cangoto[i] & (1 << j);
      if (should) {
        printf("\033[44m");
      }
      printf("%s", display_char);
      if (should) {
        printf("\033[0m");
      }
      if (j < 7) {
        printf(" | ");
      }
    }
    printf("\n   ———+———+———+———+———+———+———+———\n");
  }
}

void makemove(ChessState *state, ChessMove move) {
  // state->pieces[move.start] ^= state->pieces[move.end];
  // state->pieces[move.end] ^= state->pieces[move.start];
  // state->pieces[move.start] ^= state->pieces[move.end];
  state->pieces[move.end] = state->pieces[move.start];
  state->pieces[move.start] = PIECE_EMPTY;
}

char piececolor(char piece) {
  switch (piece) {
  case PIECE_BISHOP_BLACK:
  case PIECE_KNIGHT_BLACK:
  case PIECE_ROOK_BLACK:
  case PIECE_QUEEN_BLACK:
  case PIECE_KING_BLACK:
  case PIECE_PAWN_BLACK:
    return 1;
    break;
  case PIECE_BISHOP_WHITE:
  case PIECE_KNIGHT_WHITE:
  case PIECE_ROOK_WHITE:
  case PIECE_QUEEN_WHITE:
  case PIECE_KING_WHITE:
  case PIECE_PAWN_WHITE:
    return 0;
    break;
  default:
    return -1;
  }
}

/// Checks if a move is valid, -1 invalid, 0 valid for white, 1 valid for black
char validatemove(ChessState *board, ChessMove move) {
  char color = piececolor(board->pieces[move.start]);
  if (piececolor(board->pieces[move.end]) == color) {
    return -1;
  }

  switch (board->pieces[move.start]) {
  case PIECE_PAWN_WHITE:
    if (move.end - move.start == 8 && board->pieces[move.end] == PIECE_EMPTY) {
      return color;
    } else if (move.end - move.start == 16 && move.start / 8 == 1 &&
               board->pieces[move.start + 8] == PIECE_EMPTY &&
               board->pieces[move.end] == PIECE_EMPTY) {
      return color;
    } else if (((move.end - move.start == 7 && move.start % 8 != 0) ||
                (move.end - move.start == 9 && move.start % 8 != 7)) &&
               board->pieces[move.end] != PIECE_EMPTY) {
      return color;
    } else {
      // also add en passant in the future
      return -1;
    }
    break;
  case PIECE_PAWN_BLACK:
    if (move.end - move.start == -8 && board->pieces[move.end] == PIECE_EMPTY) {
      return color;
    } else if (move.end - move.start == -16 && move.start / 8 == 6 &&
               board->pieces[move.start - 8] == PIECE_EMPTY &&
               board->pieces[move.end] == PIECE_EMPTY) {
      return color;
    } else if (((move.end - move.start == -9 && move.start % 8 != 0) ||
                (move.end - move.start == -7 && move.start % 8 != 7)) &&
               board->pieces[move.end] != PIECE_EMPTY) {
      return color;
    } else {
      // also add en passant in the future
      return -1;
    }
    break;
  case PIECE_ROOK_BLACK:
  case PIECE_ROOK_WHITE: {
    char start_row = move.start / 8;
    char start_col = move.start % 8;
    int end_row = move.end / 8;
    int end_col = move.end % 8;

    if (start_row != end_row && start_col != end_col) {
      return -1;
    }

    int row_step = (end_row > start_row) ? 1 : (end_row < start_row) ? -1 : 0;
    int col_step = (end_col > start_col) ? 1 : (end_col < start_col) ? -1 : 0;

    int current_row = start_row + row_step;
    int current_col = start_col + col_step;

    while (current_row != end_row || current_col != end_col) {
      int current_pos = current_row * 8 + current_col;
      if (board->pieces[current_pos] != PIECE_EMPTY) {
        return -1;
      }
      current_row += row_step;
      current_col += col_step;
    }

    return color;
  } break;
  case PIECE_BISHOP_WHITE:
  case PIECE_BISHOP_BLACK: {
    char start_row = move.start / 8;
    char start_col = move.start % 8;
    int end_row = move.end / 8;
    int end_col = move.end % 8;

    if (abs(end_row - start_row) != abs(end_col - start_col)) {
      return -1;
    }

    int row_step = (end_row > start_row) ? 1 : (end_row < start_row) ? -1 : 0;
    int col_step = (end_col > start_col) ? 1 : (end_col < start_col) ? -1 : 0;

    int current_row = start_row + row_step;
    int current_col = start_col + col_step;

    while (current_row != end_row || current_col != end_col) {
      int current_pos = current_row * 8 + current_col;
      if (board->pieces[current_pos] != PIECE_EMPTY) {
        return -1;
      }
      current_row += row_step;
      current_col += col_step;
    }

    return color;
  } break;
  case PIECE_KING_BLACK:
  case PIECE_KING_WHITE: {
    int start_row = move.start / 8;
    int start_col = move.start % 8;
    int end_row = move.end / 8;
    int end_col = move.end % 8;

    if (abs(end_row - start_row) <= 1 && abs(end_col - start_col) <= 1 &&
        move.end != move.start) {
      return color;
    }
  } break;
  case PIECE_KNIGHT_BLACK:
  case PIECE_KNIGHT_WHITE: {
    int start_row = (int)move.start / 8;
    int start_col = (int)move.start % 8;
    int end_row = move.end / 8;
    int end_col = move.end % 8;
    // printf("%i  %i->%i(%i) %i->%i(%i)\n", move.end - move.start, start_row,
    //        end_row, abs(end_row - start_row), start_col, end_col,
    //        abs(end_col - start_col));
    if ((abs(end_row - start_row) == 2 && abs(end_col - start_col) == 1) ||
        (abs(end_row - start_row) == 1 && abs(end_col - start_col) == 2)) {
      // printf("yay\n");
      return color;
    }
  } break;
  case PIECE_QUEEN_BLACK:
  case PIECE_QUEEN_WHITE: {
    char start_row = move.start / 8;
    char start_col = move.start % 8;
    int end_row = move.end / 8;
    int end_col = move.end % 8;

    if (abs(end_row - start_row) == abs(end_col - start_col) ||
        end_row == start_row || end_col == start_col) {
      int row_step = (end_row > start_row) ? 1 : (end_row < start_row) ? -1 : 0;
      int col_step = (end_col > start_col) ? 1 : (end_col < start_col) ? -1 : 0;

      int current_row = start_row + row_step;
      int current_col = start_col + col_step;

      while (current_row != end_row || current_col != end_col) {
        int current_pos = current_row * 8 + current_col;
        if (board->pieces[current_pos] != PIECE_EMPTY) {
          return -1;
        }
        current_row += row_step;
        current_col += col_step;
      }

      return color;
    }
  } break;
  default:
    return -1;
  }

  return -1;
}

ChessMove *list_valid_moves_without_king_check(ChessState *state, char color) {
  ChessMove *move_list;
  vec_init(&move_list);

  for (size_t i = 0; i < 64; i++) {
    if (state->pieces[i] != PIECE_EMPTY) {
      for (size_t j = 0; j < 64; j++) {
        ChessMove move = {.start = (char)i, .end = (char)j};
        if (validatemove(state, move) == color) {
          vec_push(&move_list, &move);
        }
      }
    }
  }

  return move_list;
}

// check if $color's king is in danger
char checkking(ChessState *state, char color) {
  int king_square = -1;

  for (int i = 0; i < 64; i++) {
    if (state->pieces[i] == PIECE_KING_WHITE + color) {
      king_square = i;
      break;
    }
  }

  if (king_square < 0) {
    return 0;
  }

  char attacker_color = !color;

  for (int i = 0; i < 64; i++) {
    if (piececolor(state->pieces[i]) != attacker_color) {
      continue;
    }

    ChessMove attack = {
        .start = (char)i,
        .end = (char)king_square,
    };

    if (validatemove(state, attack) == attacker_color) {
      return 1;
    }
  }

  return 0;
}

ChessMove *list_valid_moves(ChessState *state, char color) {
  ChessMove *moves = list_valid_moves_without_king_check(state, color);
  ChessMove *valid_moves;
  vec_init_len(&valid_moves, vec_len(&moves));
  size_t move_count = vec_len(&moves);
  for (size_t i = 0; i < move_count; i++) {
    ChessMove *move = &moves[i];
    ChessState state_copy = *state;
    makemove(&state_copy, *move);
    if (!checkking(&state_copy, color)) {
      vec_push(&valid_moves, move);
    }
  }
  vec_free(&moves);
  return valid_moves;
}

// check checkmate
// check if $color is checkmated
char check2mate(ChessState *state, char color) {
  ChessMove *valid_moves = list_valid_moves(state, color);
  char res = vec_len(&valid_moves) == 0;
  vec_free(&valid_moves);
  return res;
}
