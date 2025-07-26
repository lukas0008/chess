#include "chess.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char is_abc_move_worthy(char c) { return c >= 'a' && c <= 'h'; }
char is_num_move_worthy(char c) { return c >= '1' && c <= '8'; }
char is_move_worthy(char c[2]) {
  return is_abc_move_worthy(c[0]) && is_num_move_worthy(c[1]);
}

ChessState initboard() {
  ChessState state = {
      // clang-format off
      .pieces = {
      'r', 'b', 'n', 'q', 'k', 'b', 'n', 'r',
      'p', 'p', 'p', 'p', 'p', 'p', 'p', 'p',
      ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
      ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
      ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
      ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
      'P', 'P', 'P', 'P', 'P', 'P', 'P', 'P',
      'R', 'B', 'N', 'Q', 'K', 'B', 'N', 'R',
      }};
  // clang-format on
  return state;
}

void printboard(ChessState *state, char from_pos) {
  char color = from_pos >> 6;
  char cangoto[8] = "\0\0\0\0\0\0\0\0";
  if (color == 0 || color == 1) {
    for (char i = 0; i < 64; i++) {
      if (validatemove(state, (ChessMove){.start = from_pos & 63, .end = i}) ==
          color) {
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
      case 'K':
        display_char = "♔";
        break;
      case 'Q':
        display_char = "♕";
        break;
      case 'R':
        display_char = "♖";
        break;
      case 'B':
        display_char = "♗";
        break;
      case 'N':
        display_char = "♘";
        break;
      case 'P':
        display_char = "♙";
        break;
      case 'k':
        display_char = "♚";
        break;
      case 'q':
        display_char = "♛";
        break;
      case 'r':
        display_char = "♜";
        break;
      case 'b':
        display_char = "♝";
        break;
      case 'n':
        display_char = "♞";
        break;
      case 'p':
        display_char = "♟";
        break;
      case ' ':
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

char parsemove(ChessMove *move, char *str_move) {
  size_t str_len = strlen(str_move);
  if (str_len < 2 || str_len > 5) {
    return -1;
  }

  char offset = str_len == 5;
  if (!is_move_worthy(str_move)) {
    return -2;
  }

  char start = str_move[0] - 'a' + (str_move[1] - '1') * 8;
  if (str_len < 4 || !is_move_worthy(&str_move[2 + offset])) {
    return start;
  }
  char end = str_move[2 + offset] - 'a' + (str_move[3 + offset] - '1') * 8;

  move->start = start;
  move->end = end;

  return 0;
}

void makemove(ChessState *state, ChessMove move) {
  // state->pieces[move.start] ^= state->pieces[move.end];
  // state->pieces[move.end] ^= state->pieces[move.start];
  // state->pieces[move.start] ^= state->pieces[move.end];
  state->pieces[move.end] = state->pieces[move.start];
  state->pieces[move.start] = ' ';
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
    if (move.end - move.start == 8 && board->pieces[move.end] == ' ') {
      return color;
    } else if (move.end - move.start == 16 && move.start / 8 == 1 &&
               board->pieces[move.start + 8] == ' ' &&
               board->pieces[move.end] == ' ') {
      return color;
    } else if (((move.end - move.start == 7 && move.start % 8 != 0) ||
                (move.end - move.start == 9 && move.start % 8 != 7)) &&
               board->pieces[move.end] != ' ') {
      return color;
    } else {
      // also add en passant in the future
      return -1;
    }
    break;
  case PIECE_PAWN_BLACK:
    if (move.end - move.start == -8 && board->pieces[move.end] == ' ') {
      return color;
    } else if (move.end - move.start == -16 && move.start / 8 == 6 &&
               board->pieces[move.start - 8] == ' ' &&
               board->pieces[move.end] == ' ') {
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
      if (board->pieces[current_pos] != ' ') {
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
      if (board->pieces[current_pos] != ' ') {
        return -1;
      }
      current_row += row_step;
      current_col += col_step;
    }

    return color;
  } break;
  case PIECE_KING_BLACK:
  case PIECE_KING_WHITE:
    if (abs((move.end - move.start) / 8) <= 1 &&
        abs((move.end - move.start) % 8) <= 1 && move.end != move.start) {
      return color;
    } else {
      return -1;
    }
    break;
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
    } else {
      return -1;
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
        if (board->pieces[current_pos] != ' ') {
          return -1;
        }
        current_row += row_step;
        current_col += col_step;
      }

      return color;
    } else {
      return -1;
    }
  } break;
  default:
    return -1;
  }

  return -1;
}
